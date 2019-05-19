/*
********************************************************************
*                                                                  *
* lwmf - lightweight media framework                               *
*                                                                  *
* Demo/example program                                             *
*                                                                  *
* (C) 2019 - present by Stefan Kubsch                              *
*                                                                  *
********************************************************************
*/

// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// Disable "unreferenced formal parameter" warning in WinMain
#pragma warning(disable: 4100)

#include <Windows.h>
#include <cstdint>
#include <string>

// Including "lwmf.hpp" is mandatory - this is the main library file!
#include "./include/lwmf.hpp"

// Include the used demo effects
#include "./DemoSources/Metaballs.hpp"
#include "./DemoSources/Plasma.hpp"
#include "./DemoSources/DotTunnel.hpp"
#include "./DemoSources/Fire.hpp"
#include "./DemoSources/Swarm.hpp"
#include "./DemoSources/Landscape.hpp"
#include "./DemoSources/Starfield.hpp"
#include "./DemoSources/VectorCube.hpp"
#include "./DemoSources/Lens.hpp"
#include "./DemoSources/Copperbars.hpp"
#include "./DemoSources/Tunnel.hpp"
#include "./DemoSources/Morph.hpp"
#include "./DemoSources/GouraudShade.hpp"
#include "./DemoSources/RotoZoom.hpp"
#include "./DemoSources/Moiree.hpp"
#include "./DemoSources/Julia.hpp"
#include "./DemoSources/Bobs.hpp"
#include "./DemoSources/PerlinGFX.hpp"

inline std::string FillrateTestString;
inline std::int_fast32_t DemoPart{ 1 };
constexpr std::int_fast32_t MaxDemoPart{ 19 };

std::int_fast32_t WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// Create window and OpenGL context
	lwmf::CreateOpenGLWindow(hInstance, 800, 600, "lwmf demo - switch parts with CURSOR LEFT & RIGHT, ESC to exit!", true);
	// Set VSync: 0 = off, 1 = on
	lwmf::SetVSync(1);
	// Load OpenGL/wgl extensions
	lwmf::InitOpenGLLoader();
	// Init the shaders used for rendering
	lwmf::InitShader();

	// Init raw devices
	lwmf::RegisterRawInputDevice(lwmf::MainWindow, lwmf::HID_MOUSE);
	lwmf::RegisterRawInputDevice(lwmf::MainWindow, lwmf::HID_KEYBOARD);

	// Init the demoparts if neccessary...
	Landscape::Init();
	Lens::Init();
	Copperbars::Init();
	Tunnel::Init();
	Morph::Init();
	GouraudShade::Init();
	RotoZoom::Init();
	Bobs::Init();

	FillrateTestString = "Fillrate test, clearing " + std::to_string(lwmf::ViewportHeight * lwmf::ViewportWidth) + " pixels per frame";

	bool Quit{};

	while (!Quit)
	{
		static MSG Message{};

		while (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
			{
				Quit = true;
				break;
			}

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		switch (DemoPart)
		{
			case 1:
			{
				Metaballs::Draw();
				break;
			}
			case 2:
			{
				Plasma::Draw();
				break;
			}
			case 3:
			{
				DotTunnel::Draw();
				break;
			}
			case 4:
			{
				Fire::Draw();
				break;
			}
			case 5:
			{
				Swarm::Draw();
				break;
			}
			case 6:
			{
				Landscape::Draw();
				break;
			}
			case 7:
			{
				Starfield::Draw();
				break;
			}
			case 8:
			{
				VectorCube::Draw();
				break;
			}
			case 9:
			{
				Lens::Draw();
				break;
			}
			case 10:
			{
				Copperbars::Draw();
				break;
			}
			case 11:
			{
				Tunnel::Draw();
				break;
			}
			case 12:
			{
				Morph::Draw();
				break;
			}
			case 13:
			{
				GouraudShade::Draw();
				break;
			}
			case 14:
			{
				RotoZoom::Draw();
				break;
			}
			case 15:
			{
				Moiree::Draw();
				break;
			}
			case 16:
			{
				Julia::Draw();
				break;
			}
			case 17:
			{
				Bobs::Draw();
				break;
			}
			case 18:
			{
				PerlinGFX::DrawParts();
				break;
			}
			case 19:
			{
				lwmf::ClearPixelBuffer(rand() % 0XFFFFFFFF);
				lwmf::RenderText(FillrateTestString, 10, 10, 0xFFFFFFFF);
				break;
			}
			default: {}
		}

		// Show FPS counter
		lwmf::FPSCounter();
		lwmf::DisplayFPSCounter(10, 20, 0xFFFFFFFF);

		// Bring the pixelbuffer to screen
		lwmf::RenderPixelBuffer();
		lwmf::SwapPixelBuffer();
	}

	lwmf::UnregisterRawInputDevice(lwmf::HID_MOUSE);
	lwmf::UnregisterRawInputDevice(lwmf::HID_KEYBOARD);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_SIZE:
		{
			lwmf::ResizeOpenGLWindow();
			FillrateTestString = "Fillrate test, clearing " + std::to_string(lwmf::ViewportHeight * lwmf::ViewportWidth) + " pixels per frame";
			break;
		}
		case WM_INPUT:
		{
			// Variables for RAW INPUT Buffer
			// RawInputBuffer will be max. 40bytes on 32bit, and 48bytes on 64bit applications
			static std::uint_fast32_t Size{ sizeof(RAWINPUT) };
			static RAWINPUT RawDev[sizeof(RAWINPUT)];
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, RawDev, &Size, sizeof(RAWINPUTHEADER));

			switch (RawDev->header.dwType)
			{
				case RIM_TYPEKEYBOARD:
				{
					if (RawDev->data.keyboard.Message == WM_KEYDOWN || RawDev->data.keyboard.Message == WM_SYSKEYDOWN)
					{
						if (RawDev->data.keyboard.VKey == VK_ESCAPE)
						{
							PostQuitMessage(0);
							break;
						}

						if (RawDev->data.keyboard.VKey == VK_RIGHT)
						{
							DemoPart < MaxDemoPart ? ++DemoPart : DemoPart = 1;
							lwmf::ClearPixelBuffer(0);
							break;
						}

						if (RawDev->data.keyboard.VKey == VK_LEFT)
						{
							DemoPart > 1 ? --DemoPart : DemoPart = MaxDemoPart;
							lwmf::ClearPixelBuffer(0);
						}
					}
				}
			}
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		default: {}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
