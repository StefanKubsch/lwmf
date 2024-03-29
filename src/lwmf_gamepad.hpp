/*
*******************************************************
*                                                     *
* lwmf_gamepad - lightweight media framework          *
*                                                     *
* (C) 2019 - present by Stefan Kubsch                 *
*                                                     *
*******************************************************
*/

#pragma once

#define NOMINMAX
#include <Windows.h>
#include <Xinput.h>
#include <cstdint>
#include <algorithm>
#include <string>
#include <map>
#include <cmath>
#include <utility>

#include "lwmf_general.hpp"
#include "lwmf_logging.hpp"

// Since we use XInput from Windows for the gamepad control, we need to include it!
#pragma comment(lib, "xinput")

namespace lwmf
{


	class Gamepad
	{
	public:
		enum class AnalogButtons : std::int_fast32_t
		{
			LeftStickLeft,
			LeftStickRight,
			LeftStickUp,
			LeftStickDown,
			RightStickLeft,
			RightStickRight,
			RightStickUp,
			RightStickDown,
			LeftTrigger,
			RightTrigger,
			Counter
		};

		struct AnalogMapping
		{
			std::int_fast32_t Key{};
			float Threshold{};
		};

		Gamepad();
		void SetButtons();
		void SetDeadzone(float x, float y);
		bool CheckConnection();
		void Refresh();
		void AddKeyMapping(WORD Button, std::int_fast32_t Key);
		void RemoveKeyMappingByButton(WORD Button);
		void RemoveKeyMapping(std::int_fast32_t Key);
		void AddAnalogKeyMapping(AnalogButtons Button, float Threshold, std::int_fast32_t Key);
		void RemoveAnalogKeyMapping(AnalogButtons Button);
		void DeleteMappings();
		void SetIntervalAll(std::uint_fast32_t Time);
		void SetInterval(WORD Button, std::uint_fast32_t Time);
		void SetAnalogInterval(AnalogButtons Button, std::uint_fast32_t Time);

		std::map<WORD, std::string> Buttons{};
		lwmf::IntPointStruct RightStickPos{};
		lwmf::FloatPointStruct LeftStick{};
		lwmf::FloatPointStruct RightStick{};
		std::int_fast32_t ControllerID{ -1 };
		float RotationXLimit{ 0.01F };
		float Sensitivity{ 0.3F };

	private:
		void SendAnalogKeys(AnalogButtons Button, float Now, float Before, float Threshold, std::int_fast32_t Key);

		XINPUT_STATE State{};
		XINPUT_STATE Previous{};

		std::map<WORD, std::int_fast32_t> KeyMap{};
		std::map<AnalogButtons, AnalogMapping> AnalogMap{};
		std::map<WORD, std::uint32_t> Repeat{};
		std::map<AnalogButtons, std::uint32_t> AnalogRepeat{};
		std::map<WORD, DWORD> LastPress{};
		std::map<AnalogButtons, DWORD> AnalogLastPress{};

		lwmf::FloatPointStruct DeadZone{ 0.3F, 0.3F };
		lwmf::FloatPointStruct PreviousLeftStick{};
		lwmf::FloatPointStruct PreviousRightStick{};
		float PreviousLeftTrigger{};
		float PreviousRightTrigger{};
		float TriggerLeft{};
		float TriggerRight{};
	};

	inline Gamepad::Gamepad()
	{
		SetButtons();
	}

	inline void Gamepad::SetButtons()
	{
		Buttons.emplace(std::make_pair<WORD, std::string>(XINPUT_GAMEPAD_A, "A"));
		Buttons.emplace(std::make_pair<WORD, std::string>(XINPUT_GAMEPAD_B, "B"));
		Buttons.emplace(std::make_pair<WORD, std::string>(XINPUT_GAMEPAD_X, "X"));
		Buttons.emplace(std::make_pair<WORD, std::string>(XINPUT_GAMEPAD_Y, "Y"));
		Buttons.emplace(std::make_pair<WORD, std::string>(XINPUT_GAMEPAD_DPAD_LEFT, "DPLeft"));
		Buttons.emplace(std::make_pair<WORD, std::string>(XINPUT_GAMEPAD_DPAD_RIGHT, "DPRight"));
		Buttons.emplace(std::make_pair<WORD, std::string>(XINPUT_GAMEPAD_DPAD_UP, "DPUp"));
		Buttons.emplace(std::make_pair<WORD, std::string>(XINPUT_GAMEPAD_DPAD_DOWN, "DPDown"));
		Buttons.emplace(std::make_pair<WORD, std::string>(XINPUT_GAMEPAD_LEFT_SHOULDER, "LSB"));
		Buttons.emplace(std::make_pair<WORD, std::string>(XINPUT_GAMEPAD_RIGHT_SHOULDER, "RSB"));
		Buttons.emplace(std::make_pair<WORD, std::string>(XINPUT_GAMEPAD_BACK, "Back"));
		Buttons.emplace(std::make_pair<WORD, std::string>(XINPUT_GAMEPAD_START, "Start"));
		Buttons.emplace(std::make_pair<WORD, std::string>(XINPUT_GAMEPAD_LEFT_THUMB, "LT"));
		Buttons.emplace(std::make_pair<WORD, std::string>(XINPUT_GAMEPAD_RIGHT_THUMB, "RT"));
	}

	inline void Gamepad::SetDeadzone(const float x, const float y)
	{
		DeadZone = { x, y };
	}

	inline bool Gamepad::CheckConnection()
	{
		LWMFSystemLog.AddEntry(LogLevel::Info, __FILENAME__, __LINE__, "Searching for XBOX controller...");

		for (std::int_fast32_t i{}; i < XUSER_MAX_COUNT && ControllerID == -1; ++i)
		{
			XINPUT_STATE XInputState{};

			if (XInputGetState(i, &XInputState) == ERROR_SUCCESS)
			{
				ControllerID = i;
				LWMFSystemLog.AddEntry(LogLevel::Info, __FILENAME__, __LINE__, "XBOX controller found.");
			}
			else
			{
				LWMFSystemLog.AddEntry(LogLevel::Info, __FILENAME__, __LINE__, "No XBOX controller found.");
			}
		}

		return ControllerID != -1;
	}

	inline void Gamepad::Refresh()
	{
		Previous = State;
		PreviousLeftStick = LeftStick;
		PreviousRightStick = RightStick;
		PreviousLeftTrigger = TriggerLeft;
		PreviousRightTrigger = TriggerRight;

		SecureZeroMemory(&State, sizeof(XINPUT_STATE));
		XInputGetState(ControllerID, &State);

		const FloatPointStruct NormalizedL{ std::max(-1.0F, static_cast<float>(State.Gamepad.sThumbLX) / SHRT_MAX), std::max(-1.0F, static_cast<float>(State.Gamepad.sThumbLY) / SHRT_MAX) };

		LeftStick.X = (std::fabs(NormalizedL.X) < DeadZone.X ? 0.0F : (std::fabs(NormalizedL.X) - DeadZone.X) * (NormalizedL.X / std::fabs(NormalizedL.X)));
		LeftStick.Y = (std::fabs(NormalizedL.Y) < DeadZone.Y ? 0.0F : (std::fabs(NormalizedL.Y) - DeadZone.Y) * (NormalizedL.Y / std::fabs(NormalizedL.Y)));

		if (DeadZone.X > 0.0F)
		{
			LeftStick.X *= 1.0F / (1.0F - DeadZone.X);
		}

		if (DeadZone.Y > 0.0F)
		{
			LeftStick.Y *= 1.0F / (1.0F - DeadZone.Y);
		}

		const FloatPointStruct NormalizedR{ std::max(-1.0F, static_cast<float>(State.Gamepad.sThumbRX) / SHRT_MAX), std::max(-1.0F, static_cast<float>(State.Gamepad.sThumbRY) / SHRT_MAX) };

		RightStick.X = (std::fabs(NormalizedR.X) < DeadZone.X ? 0.0F : (std::fabs(NormalizedR.X) - DeadZone.X) * (NormalizedR.X / std::fabs(NormalizedR.X)));
		RightStick.Y = (std::fabs(NormalizedR.Y) < DeadZone.Y ? 0.0F : (std::fabs(NormalizedR.Y) - DeadZone.Y) * (NormalizedR.Y / std::fabs(NormalizedR.Y)));

		if (DeadZone.X > 0.0F)
		{
			RightStick.X *= 1.0F / (1.0F - DeadZone.X);
		}

		if (DeadZone.Y > 0.0F)
		{
			RightStick.Y *= 1.0F / (1.0F - DeadZone.Y);
		}

		TriggerLeft = static_cast<float>(State.Gamepad.bLeftTrigger) / 255.0F;
		TriggerRight = static_cast<float>(State.Gamepad.bRightTrigger) / 255.0F;

		for (const auto& Button : Buttons)
		{
			if ((State.Gamepad.wButtons & Button.first) != 0)
			{
				const DWORD Now{ GetTickCount() };
				const DWORD Last{ (LastPress.find(Button.first) != LastPress.end() ? LastPress.find(Button.first)->second : 0) };
				const std::uint_fast32_t Time{ Repeat.find(Button.first)->second };
				const WORD Mapping{ (KeyMap.find(Button.first) != KeyMap.end() ? static_cast<WORD>(KeyMap.find(Button.first)->second) : static_cast<WORD>(Button.first)) };

				if ((Now - Last >= Time && Time > 0) || Last == 0 || (Time == 0 && (Previous.Gamepad.wButtons & Button.first) == 0))
				{
					LastPress.erase(Button.first);
					LastPress.emplace(std::map<WORD, DWORD>::value_type(Button.first, Now));

					SendMessage(MainWindow, WM_KEYDOWN, Mapping, ((Previous.Gamepad.wButtons & Button.first) == 0 ? 0 << 30 : 1 << 30));
				}
			}

			if (Previous.dwPacketNumber < State.dwPacketNumber)
			{
				if ((State.Gamepad.wButtons & Button.first) == 0 && (Previous.Gamepad.wButtons & Button.first) != 0)
				{
					LastPress.erase(Button.first);
					SendMessage(MainWindow, WM_KEYUP, (KeyMap.find(Button.first) != KeyMap.end() ? static_cast<WORD>(KeyMap.find(Button.first)->second) : static_cast<WORD>(Button.first)), 0);
				}
			}
		}

		for (const auto& Item : AnalogMap)
		{
			const WORD Mapping{ static_cast<WORD>(Item.second.Key) };

			switch (Item.first)
			{
				case AnalogButtons::LeftStickLeft:
				{
					SendAnalogKeys(AnalogButtons::LeftStickLeft, LeftStick.X, PreviousLeftStick.X, -Item.second.Threshold, Mapping);
					break;
				}
				case AnalogButtons::LeftStickRight:
				{
					SendAnalogKeys(AnalogButtons::LeftStickRight, LeftStick.X, PreviousLeftStick.X, Item.second.Threshold, Mapping);
					break;
				}
				case AnalogButtons::LeftStickUp:
				{
					SendAnalogKeys(AnalogButtons::LeftStickUp, LeftStick.Y, PreviousLeftStick.Y, Item.second.Threshold, Mapping);
					break;
				}
				case AnalogButtons::LeftStickDown:
				{
					SendAnalogKeys(AnalogButtons::LeftStickDown, LeftStick.Y, PreviousLeftStick.Y, -Item.second.Threshold, Mapping);
					break;
				}
				case AnalogButtons::RightStickLeft:
				{
					SendAnalogKeys(AnalogButtons::RightStickLeft, RightStick.X, PreviousRightStick.X, -Item.second.Threshold, Mapping);
					break;
				}
				case AnalogButtons::RightStickRight:
				{
					SendAnalogKeys(AnalogButtons::RightStickRight, RightStick.X, PreviousRightStick.X, Item.second.Threshold, Mapping);
					break;
				}
				case AnalogButtons::RightStickUp:
				{
					SendAnalogKeys(AnalogButtons::RightStickUp, RightStick.Y, PreviousRightStick.Y, Item.second.Threshold, Mapping);
					break;
				}
				case AnalogButtons::RightStickDown:
				{
					SendAnalogKeys(AnalogButtons::RightStickDown, RightStick.Y, PreviousRightStick.Y, -Item.second.Threshold, Mapping);
					break;
				}
				case AnalogButtons::LeftTrigger:
				{
					SendAnalogKeys(AnalogButtons::LeftTrigger, TriggerLeft, PreviousLeftTrigger, Item.second.Threshold, Mapping);
					break;
				}
				case AnalogButtons::RightTrigger:
				{
					SendAnalogKeys(AnalogButtons::RightTrigger, TriggerRight, PreviousRightTrigger, Item.second.Threshold, Mapping);
					break;
				}
				default: {}
			}
		}
	}

	inline void Gamepad::AddKeyMapping(const WORD Button, const std::int_fast32_t Key)
	{
		KeyMap.erase(Button);
		KeyMap.emplace(std::map<WORD, std::int_fast32_t>::value_type(Button, Key));
	}

	inline void Gamepad::RemoveKeyMapping(const std::int_fast32_t Key)
	{
		for (auto it{ KeyMap.begin() }; it != KeyMap.end(); ++it)
		{
			if (it->second == Key)
			{
				KeyMap.erase(it);
				break;
			}
		}
	}

	inline void Gamepad::RemoveKeyMappingByButton(const WORD Button)
	{
		KeyMap.erase(Button);
	}

	inline void Gamepad::AddAnalogKeyMapping(const AnalogButtons Button, const float Threshold, const std::int_fast32_t Key)
	{
		const AnalogMapping AnalogKeyMapping { Key, Threshold };

		AnalogMap.erase(Button);
		AnalogMap.emplace(std::make_pair(Button, AnalogKeyMapping));
	}

	inline void Gamepad::RemoveAnalogKeyMapping(AnalogButtons Button)
	{
		AnalogMap.erase(Button);
	}

	inline void Gamepad::DeleteMappings()
	{
		KeyMap.clear();
		AnalogMap.clear();
	}

	inline void Gamepad::SetIntervalAll(const std::uint_fast32_t Time)
	{
		Repeat.clear();

		for (const auto& Button : Buttons)
		{
			Repeat.emplace(std::map<WORD, std::uint_fast32_t>::value_type(Button.first, Time));
		}

		AnalogRepeat.clear();

		for (std::int_fast32_t i{}; i < static_cast<std::int_fast32_t>(AnalogButtons::Counter); ++i)
		{
			AnalogRepeat.emplace(std::map<AnalogButtons, std::uint_fast32_t>::value_type(static_cast<AnalogButtons>(i), Time));
		}
	}

	inline void Gamepad::SetInterval(const WORD Button, const std::uint_fast32_t Time)
	{
		Repeat.erase(Button);
		Repeat.emplace(std::map<WORD, std::uint_fast32_t>::value_type(Button, Time));
	}

	inline void Gamepad::SetAnalogInterval(const AnalogButtons Button, const std::uint_fast32_t Time)
	{
		AnalogRepeat.erase(Button);
		AnalogRepeat.emplace(std::map<AnalogButtons, std::uint_fast32_t>::value_type(Button, Time));
	}

	inline void Gamepad::SendAnalogKeys(const AnalogButtons Button, const float Now, const float Before, const float Threshold, const std::int_fast32_t Key)
	{
		const bool IsPressed{ (Now >= Threshold && Threshold > 0.0F) || (Now <= Threshold && Threshold < 0.0F) };
		const bool WasPressed{ (Before >= Threshold && Threshold > 0.0F) || (Before <= Threshold && Threshold < 0.0F) };

		if (IsPressed)
		{
			const DWORD TempNow{ GetTickCount() };
			const DWORD TempBefore{ (AnalogLastPress.find(Button) != AnalogLastPress.end() ? AnalogLastPress.find(Button)->second : 0) }; //-V783
			const std::uint_fast32_t Time{ AnalogRepeat.find(Button)->second }; //-V783

			if ((TempNow - TempBefore >= Time && Time > 0) || TempBefore == 0 || (Time == 0 && !WasPressed))
			{
				AnalogLastPress.erase(Button);
				AnalogLastPress.emplace(std::map<AnalogButtons, DWORD>::value_type(Button, TempNow));
				SendMessage(MainWindow, WM_KEYDOWN, static_cast<WPARAM>(Key), (WasPressed ? 1 << 30 : 0 << 30));
			}
		}

		if (Previous.dwPacketNumber < State.dwPacketNumber)
		{
			if (!IsPressed && WasPressed)
			{
				AnalogLastPress.erase(Button);

				SendMessage(MainWindow, WM_KEYUP, static_cast<WPARAM>(Key), 0);
			}
		}
	}


} // namespace lwmf