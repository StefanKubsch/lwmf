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

// "ScreenTexture" is the main render target in our demo!
lwmf::TextureStruct ScreenTexture;
lwmf::ShaderClass ScreenTextureShader;

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
	try
	{
		lwmf::CreateOpenGLWindow(hInstance, ScreenTexture, 800, 600, "lwmf demo - switch parts with CURSOR LEFT & RIGHT, ESC to exit!", true);
		// Set VSync: 0 = off, -1 = on (adaptive vsync = smooth as fuck)
		lwmf::SetVSync(-1);
		// Load OpenGL/wgl extensions
		lwmf::InitOpenGLLoader();
		// Init the shaders used for rendering
		ScreenTextureShader.LoadShader("Default", ScreenTexture);
		ScreenTextureShader.PrepareLWMFTexture(ScreenTexture, 0, 0);

		// Init raw devices
		lwmf::RegisterRawInputDevice(lwmf::MainWindow, lwmf::HID_MOUSE);
		lwmf::RegisterRawInputDevice(lwmf::MainWindow, lwmf::HID_KEYBOARD);
	}
	catch (const std::runtime_error&)
	{
		return EXIT_FAILURE;
	}

	// Init the demoparts if neccessary...
	Landscape::Init();
	Lens::Init();
	Copperbars::Init();
	Tunnel::Init();
	Morph::Init();
	GouraudShade::Init();
	RotoZoom::Init();
	Bobs::Init();

	FillrateTestString = "Fillrate test, clearing " + std::to_string(ScreenTexture.Height * ScreenTexture.Width) + " pixels per frame";

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
				lwmf::ClearTexture(ScreenTexture, rand() % 0XFFFFFFFF);
				lwmf::RenderText(ScreenTexture, FillrateTestString, 10, 10, 0xFFFFFFFF);
				break;
			}
			default: {}
		}

		// Show FPS counter
		lwmf::FPSCounter();
		lwmf::DisplayFPSCounter(ScreenTexture, 10, 20, 0xFFFFFFFF);

		// Bring the pixelbuffer to screen
		ScreenTextureShader.RenderLWMFTexture(ScreenTexture);
		SwapBuffers(lwmf::WindowHandle);
	}

	lwmf::UnregisterRawInputDevice(lwmf::HID_MOUSE);
	lwmf::UnregisterRawInputDevice(lwmf::HID_KEYBOARD);

	return EXIT_SUCCESS;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_SIZE:
		{
			lwmf::ResizeOpenGLWindow(ScreenTexture);
			FillrateTestString = "Fillrate test, clearing " + std::to_string(ScreenTexture.Height * ScreenTexture.Width) + " pixels per frame";
			break;
		}
		case WM_INPUT:
		{
			// RawInputBuffer will be max. 40bytes on 32bit, and 48bytes on 64bit applications
			static RAWINPUT RawDev;
			static UINT DataSize{ sizeof(RawDev) };
			static UINT HeaderSize{ sizeof(RAWINPUTHEADER) };
			HRAWINPUT Handle{ reinterpret_cast<HRAWINPUT>(lParam) };
			GetRawInputData(Handle, RID_INPUT, &RawDev, &DataSize, HeaderSize);

			switch (RawDev.header.dwType)
			{
				case RIM_TYPEKEYBOARD:
				{
					if (RawDev.data.keyboard.Message == WM_KEYDOWN || RawDev.data.keyboard.Message == WM_SYSKEYDOWN)
					{
						switch (RawDev.data.keyboard.VKey)
						{
							case VK_ESCAPE:
							{
								PostQuitMessage(0);
								break;
							}
							case VK_RIGHT:
							{
								DemoPart < MaxDemoPart ? ++DemoPart : DemoPart = 1;
								lwmf::ClearTexture(ScreenTexture, 0);
								break;
							}
							case VK_LEFT:
							{
								DemoPart > 1 ? --DemoPart : DemoPart = MaxDemoPart;
								lwmf::ClearTexture(ScreenTexture, 0);
								break;
							}
							default: {}
						}
					}
					break;
				}
				default: {}
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
