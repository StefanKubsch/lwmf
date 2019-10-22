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
#include <charconv>

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
inline lwmf::TextureStruct ScreenTexture;
inline lwmf::ShaderClass ScreenTextureShader;

// Init & seed random engine
inline static std::random_device Seed;
inline static std::mt19937 Engine(Seed());

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

inline std::string FillrateTestString;
inline std::int_fast32_t DemoPart{};
constexpr std::int_fast32_t MaxDemoPart{ 21 };

lwmf::MP3Player Music;

std::int_fast32_t WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
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
		// Init raw devices
		lwmf::RegisterRawInputDevice(lwmf::MainWindow, lwmf::DeviceIdentifier::HID_MOUSE);
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
	}
	catch (const std::runtime_error&)
	{
		return EXIT_FAILURE;
	}

	FillrateTestString = "Fillrate test, clearing " + std::to_string(ScreenTexture.Size) + " pixels per frame";

	static const std::uniform_int_distribution<std::int_fast32_t> Distrib1(0, 0XFFFFFF);

	bool Quit{};

	while (!Quit)
	{
		if (!Music.IsPlaying())
		{
			Music.Play();
		}

		static MSG Message{};

		while (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
			{
				DemoLog.AddEntry(lwmf::LogLevel::Info, __FILENAME__, "MESSAGE: WM_QUIT received...");
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
				lwmf::ClearTexture(ScreenTexture, Distrib1(Engine));
				lwmf::RenderText(ScreenTexture, FillrateTestString, 10, 10, 0xFFFFFFFF);
				break;
			}
			case 20:
			{
				PrimitivesTest::Draw();
				break;
			}
			case 21:
			{
				BitmapTest::Draw();
				break;
			}
			default: {}
		}

		// Show FPS counter
		lwmf::FPSCounter();
		lwmf::DisplayFPSCounter(ScreenTexture, 10, 20, 0xFFFFFFFF);

		// Bring the pixelbuffer to screen
		ScreenTextureShader.RenderLWMFTexture(ScreenTexture, false, 1.0F);
		lwmf::SwapBuffer();
	}

	Music.Close();
	lwmf::UnregisterRawInputDevice(lwmf::DeviceIdentifier::HID_MOUSE);
	lwmf::UnregisterRawInputDevice(lwmf::DeviceIdentifier::HID_KEYBOARD);
	lwmf::DeleteOpenGLContext();

	DemoLog.AddEntry(lwmf::LogLevel::Info, __FILENAME__, "Exit program...");

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
								(DemoPart == 19 || DemoPart == 20 || DemoPart == 21) ? lwmf::SetVSync(0) : lwmf::SetVSync(-1);
								break;
							}
							case VK_LEFT:
							{
								DemoPart > 0 ? --DemoPart : DemoPart = MaxDemoPart;
								lwmf::ClearTexture(ScreenTexture, 0);
								(DemoPart == 19 || DemoPart == 20 || DemoPart == 21) ? lwmf::SetVSync(0) : lwmf::SetVSync(-1);
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
