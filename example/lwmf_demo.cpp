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

#include <Windows.h>
#include <cstdint>
#include <string>
#include <random>

// Uncomment to find memory leaks in debug mode
//
// Have a look here:
// https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2013/x98tx3cf(v=vs.120)
//
// #define _CRTDBG_MAP_ALLOC
// #include <stdlib.h>
// #include <crtdbg.h>

// Including "lwmf.hpp" is mandatory - this is the main library file!
#define LWMF_LOGGINGENABLED
#define LWMF_THROWEXCEPTIONS
#include "./src/lwmf.hpp"

// "ScreenTexture" is the main render target in our demo!
inline lwmf::TextureStruct ScreenTexture{};
inline lwmf::ShaderClass ScreenTextureShader{};

// Init & seed random engine
inline std::random_device Seed{};
inline std::mt19937 Engine(Seed());

inline std::int_fast32_t DemoPart{};
constexpr std::int_fast32_t MaxDemoPart{ 25 };
lwmf::MP3Player Music{};

inline std::string CutDoubleToString(const double Value)
{
	std::string Temp{ std::to_string(static_cast<std::int_fast32_t>((Value * 100.0)) / 100.0) };
	return Temp.erase(Temp.find_last_not_of('0') + 1, std::string::npos);
}

inline void DisplayInfoBox(const std::string& Partname)
{
	lwmf::FilledRectangle(ScreenTexture, 0, 0, ScreenTexture.Width, 115, 0x1F1F1F1F, 0x1F1F1F1F);
	lwmf::FPSCounter();
	lwmf::RenderText(ScreenTexture, Partname, 10, 10, 0xFFFFFFFF);
	lwmf::DisplayFPSCounter(ScreenTexture, 10, 20, 0xFFFFFFFF);
	static const std::string DurationString{ "Music duration in seconds: " + CutDoubleToString(Music.GetDuration()) };
	lwmf::RenderText(ScreenTexture, DurationString, 10, 40, 0xFFFFFFFF);
	lwmf::RenderText(ScreenTexture, "Music position in seconds: " + CutDoubleToString(Music.GetPosition()), 10, 50, 0xFFFFFFFF);
	lwmf::Line(ScreenTexture, 0, 115, ScreenTexture.Width, 115, 0xFFFFFFFF);
}

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
#include "./DemoSources/PrimitivesTest.hpp"
#include "./DemoSources/BitmapTest.hpp"
#include "./DemoSources/Circle.hpp"
#include "./DemoSources/Cubes.hpp"
#include "./DemoSources/PixelTest.hpp"
#include "./DemoSources/ThroughputTest.hpp"
#include "./DemoSources/Particles.hpp"

std::int_fast32_t WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);

	lwmf::Logging DemoLog("lwmf_demo.log");
	lwmf::WindowInstance = hInstance;

	try
	{
		// Create window and OpenGL context
		lwmf::CreateOpenGLWindow(lwmf::WindowInstance, ScreenTexture, 1280, 720, "lwmf demo - switch parts with CURSOR LEFT & RIGHT, ESC to exit!", true);
		// Set VSync: 0 = off, -1 = on (adaptive vsync = smooth as fuck)
		lwmf::SetVSync(-1);
		// Load OpenGL/wgl extensions
		lwmf::InitOpenGLLoader();
		// Check for SSE
		lwmf::CheckForSSESupport();
		// Init the shaders used for rendering
		ScreenTextureShader.LoadShader("Default", ScreenTexture);
		ScreenTextureShader.PrepareLWMFTexture(ScreenTexture, 0, 0);
		// Initial clearance of window (looks better!)
		lwmf::ClearTexture(ScreenTexture, 0x00000000);
		lwmf::ClearBuffer();
		lwmf::SwapBuffer();
		// Init raw devices
		lwmf::RegisterRawInputDevice(lwmf::MainWindow, lwmf::DeviceIdentifier::HID_KEYBOARD);
		// Init audio
		Music.Load("./DemoSFX/Audio.mp3");

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
		BitmapTest::Init();
		Particles::Init();
	}
	catch (const std::runtime_error&)
	{
		return EXIT_FAILURE;
	}

	bool Quit{};

	while (!Quit)
	{
		if (!Music.IsFinished())
		{
			Music.Play();
		}

		MSG Message{};

		while (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
			{
				DemoLog.AddEntry(lwmf::LogLevel::Info, __FILENAME__, __LINE__, "MESSAGE: WM_QUIT received...");
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
				Raytracer::Draw();
				break;
			}
			case 19:
			{
				Cubes::Draw();
				break;
			}
			case 20:
			{
				Circle::Draw();
				break;
			}
			case 21:
			{
				Particles::Draw();
				break;
			}
			case 22:
			{
				BitmapTest::Draw();
				break;
			}
			case 23:
			{
				PrimitivesTest::Draw();
				break;
			}
			case 24:
			{
				ThroughputTest::Draw();
				break;
			}
			case 25:
			{
				PixelTest::Draw();
				break;
			}
			default: {}
		}

		// Bring the pixelbuffer to screen
		lwmf::ClearBuffer();
		ScreenTextureShader.RenderLWMFTexture(ScreenTexture, false, 1.0F);
		lwmf::SwapBuffer();
	}

	Music.Close();
	lwmf::UnregisterRawInputDevice(lwmf::DeviceIdentifier::HID_KEYBOARD);
	lwmf::DeleteOpenGLContext();

	DemoLog.AddEntry(lwmf::LogLevel::Info, __FILENAME__, __LINE__, "Exit program...");

	// Uncomment to find memory leaks in debug mode
	//
	// Have a look here:
	// https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2013/x98tx3cf(v=vs.120)
	//
	// _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtDumpMemoryLeaks();

	return EXIT_SUCCESS;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_SIZE:
		{
			if (ScreenTexture.Width > 0 && ScreenTexture.Height > 0)
			{
				lwmf::ResizeOpenGLWindow(ScreenTexture);
			}

			break;
		}
		case WM_INPUT:
		{
			RAWINPUT RawDev{};
			UINT DataSize{ sizeof(RAWINPUT) };
			UINT HeaderSize{ sizeof(RAWINPUTHEADER) };
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
								lwmf::ClearTexture(ScreenTexture, 0x00000000);
								(DemoPart >= 22 && DemoPart <= 25) ? lwmf::SetVSync(0) : lwmf::SetVSync(-1);
								break;
							}
							case VK_LEFT:
							{
								DemoPart > 0 ? --DemoPart : DemoPart = MaxDemoPart;
								lwmf::ClearTexture(ScreenTexture, 0x00000000);
								(DemoPart >= 22 && DemoPart <= 25) ? lwmf::SetVSync(0) : lwmf::SetVSync(-1);
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
