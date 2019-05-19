/*
***************************************************
*                                                 *
* lwmf_rawinput - lightweight media framework     *
*                                                 *
* (C) 2019 - present by Stefan Kubsch             *
*                                                 *
***************************************************
*/

#pragma once

#include <Windows.h>

namespace lwmf
{


	// Init device identifier
	constexpr USHORT HID_MOUSE{ 2 };
	constexpr USHORT HID_KEYBOARD{ 6 };

	// WindowRect is used to catch mouse in window
	inline RECT WindowRect;

	inline void RegisterRawInputDevice(const HWND hWnd, const USHORT Device)
	{
		RAWINPUTDEVICE RawInputDevice;

		// Define RAWINPUTDEVICE
		RawInputDevice.usUsagePage = 1;
		RawInputDevice.usUsage = Device;
		RawInputDevice.dwFlags = RIDEV_DEVNOTIFY;
		RawInputDevice.hwndTarget = hWnd;

		RegisterRawInputDevices(&RawInputDevice, 1, sizeof(RawInputDevice));
	}

	inline void UnregisterRawInputDevice(const USHORT Device)
	{
		RAWINPUTDEVICE RawInputDevice;

		RawInputDevice.usUsagePage = 1;
		RawInputDevice.usUsage = Device;
		RawInputDevice.dwFlags = RIDEV_REMOVE;
		RawInputDevice.hwndTarget = nullptr;

		RegisterRawInputDevices(&RawInputDevice, 1, sizeof(RAWINPUTDEVICE));
	}

	inline void CatchMouse(const HWND hWnd)
	{
		// Get focus
		SetCapture(hWnd);

		// Hide mouse cursor
		ShowCursor(FALSE);

		// Get & set window boundaries (used for ClipCursor)
		GetClientRect(hWnd, &WindowRect);

		POINT UpperLeft{ WindowRect.left, WindowRect.top };
		POINT LowerRight{ WindowRect.right, WindowRect.bottom };

		MapWindowPoints(hWnd, nullptr, &UpperLeft, 1);
		MapWindowPoints(hWnd, nullptr, &LowerRight, 1);

		WindowRect.left = UpperLeft.x;
		WindowRect.top = UpperLeft.y;
		WindowRect.right = LowerRight.x;
		WindowRect.bottom = LowerRight.y;

		// "catch" mousepointer in window
		ClipCursor(&WindowRect);
	}


} // namespace lwmf