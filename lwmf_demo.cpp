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
#include <random>

// Including "lwmf.hpp" is mandatory - this is the main library file!
#include "./include/lwmf.hpp"

// "ScreenTexture" is the main render target in our demo!
inline lwmf::TextureStruct ScreenTexture;
inline lwmf::ShaderClass ScreenTextureShader;

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
#include "./DemoSources/Raytracer.hpp"

inline std::string FillrateTestString;
inline std::int_fast32_t DemoPart{};
constexpr std::int_fast32_t MaxDemoPart{ 19 };

std::int_fast32_t WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	lwmf::Logging DemoLog("lwmf_demo.log");

	try
	{
		// Create window and OpenGL context
		lwmf::CreateOpenGLWindow(hInstance, ScreenTexture, 1280, 720, "lwmf demo - switch parts with CURSOR LEFT & RIGHT, ESC to exit!", true);
		// Set VSync: 0 = off, -1 = on (adaptive vsync = smooth as fuck)
		lwmf::SetVSync(-1);
		// Load OpenGL/wgl extensions
		lwmf::InitOpenGLLoader();
		// Check for SSE
		lwmf::CheckForSSESupport();
		// Init the shaders used for rendering
		ScreenTextureShader.LoadShader("Default", ScreenTexture);
		ScreenTextureShader.PrepareLWMFTexture(ScreenTexture, 0, 0);
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
		Raytracer::Init();
	}
	catch (const std::runtime_error&)
	{
		return EXIT_FAILURE;
	}

	FillrateTestString = "Fillrate test, clearing " + std::to_string(ScreenTexture.Size) + " pixels per frame";

	static std::mt19937 Engine;
	static const std::uniform_int_distribution<std::int_fast32_t> Distrib1(0, 0XFFFFFF);

	bool Quit{};

	while (!Quit)
	{
		static MSG Message{};

		while (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
			{
				DemoLog.AddEntry(lwmf::Logging::LogLevels::Info, "MESSAGE: WM_QUIT received...");
				Quit = true;
				break;
			}

			DispatchMessage(&Message);
		}

		switch (DemoPart)
		{
			case 0:
			{
				Metaballs::Draw();
				break;
			}
			case 1:
			{
				Plasma::Draw();
				break;
			}
			case 2:
			{
				DotTunnel::Draw();
				break;
			}
			case 3:
			{
				Fire::Draw();
				break;
			}
			case 4:
			{
				Swarm::Draw();
				break;
			}
			case 5:
			{
				Landscape::Draw();
				break;
			}
			case 6:
			{
				Starfield::Draw();
				break;
			}
			case 7:
			{
				VectorCube::Draw();
				break;
			}
			case 8:
			{
				Lens::Draw();
				break;
			}
			case 9:
			{
				Copperbars::Draw();
				break;
			}
			case 10:
			{
				Tunnel::Draw();
				break;
			}
			case 11:
			{
				Morph::Draw();
				break;
			}
			case 12:
			{
				GouraudShade::Draw();
				break;
			}
			case 13:
			{
				RotoZoom::Draw();
				break;
			}
			case 14:
			{
				Moiree::Draw();
				break;
			}
			case 15:
			{
				Julia::Draw();
				break;
			}
			case 16:
			{
				Bobs::Draw();
				break;
			}
			case 17:
			{
				PerlinGFX::DrawParts();
				break;
			}
			case 18:
			{
				lwmf::ClearTexture(ScreenTexture, Distrib1(Engine));
				lwmf::RenderText(ScreenTexture, FillrateTestString, 10, 10, 0xFFFFFFFF);
				break;
			}
			case 19:
			{
				Raytracer::Draw();
				break;
			}
			default: {}
		}

		// Show FPS counter
		lwmf::FPSCounter();
		lwmf::DisplayFPSCounter(ScreenTexture, 10, 20, 0xFFFFFFFF);

		// Bring the pixelbuffer to screen
		ScreenTextureShader.RenderLWMFTexture(ScreenTexture);
		lwmf::SwapBuffer();
	}

	lwmf::UnregisterRawInputDevice(lwmf::HID_MOUSE);
	lwmf::UnregisterRawInputDevice(lwmf::HID_KEYBOARD);
	lwmf::DeleteOpenGLContext();

	DemoLog.AddEntry(lwmf::Logging::LogLevels::Info, "Exit program...");
	return EXIT_SUCCESS;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_SIZE:
		{
			lwmf::ResizeOpenGLWindow(ScreenTexture);
			FillrateTestString = "Fillrate test, clearing " + std::to_string(ScreenTexture.Size) + " pixels per frame";
			break;
		}
		case WM_INPUT:
		{
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
								DemoPart < MaxDemoPart ? ++DemoPart : DemoPart = 0;
								lwmf::ClearTexture(ScreenTexture, 0);
								break;
							}
							case VK_LEFT:
							{
								DemoPart > 0 ? --DemoPart : DemoPart = MaxDemoPart;
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
