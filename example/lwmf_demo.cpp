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

inline std::string FillrateTestString;
inline std::int_fast32_t DemoPart{};
constexpr std::int_fast32_t MaxDemoPart{ 22 };
lwmf::MP3Player Music{};

inline void DisplayInfoBox(const std::string& Partname)
{
	static const std::string MusicDuration{ "Music duration: " + std::to_string(Music.GetDuration()) + " seconds" };

	lwmf::FilledRectangle(ScreenTexture, 0, 0, ScreenTexture.Width, 65, 0x00000000, 0x00000000);

	// Show partname
	lwmf::RenderText(ScreenTexture, Partname, 10, 10, 0xFFFFFFFF);

	// Show FPS counter
	lwmf::FPSCounter();
	lwmf::DisplayFPSCounter(ScreenTexture, 10, 20, 0xFFFFFFFF);

	// Show audio information
	lwmf::RenderText(ScreenTexture, MusicDuration, 10, 40, 0xFFFFFFFF);
	lwmf::RenderText(ScreenTexture, "Music position: " + std::to_string(Music.GetPosition()) + " seconds", 10, 50, 0xFFFFFFFF);
}

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
		// Initial clearance of window (looks better!)
		lwmf::ClearTexture(ScreenTexture, 0x00000000);
		lwmf::ClearBuffer();
		lwmf::SwapBuffer();
		// Init raw devices
		lwmf::RegisterRawInputDevice(lwmf::MainWindow, lwmf::DeviceIdentifier::HID_KEYBOARD);
		// Init audio
		Music.Load("./DemoSFX/Audio.mp3");
		Music.SetVolume(100, 100);

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
	const std::uniform_int_distribution<std::int_fast32_t> Distrib1(0, 0XFFFFFF);
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
				DisplayInfoBox("OpenMP accelerated realtime metaballs");
				break;
			}
			case 1:
			{
				Plasma::Draw();
				DisplayInfoBox("OpenMP accelerated realtime plasma");
				break;
			}
			case 2:
			{
				DotTunnel::Draw();
				DisplayInfoBox("Dotted tunnel");
				break;
			}
			case 3:
			{
				Fire::Draw();
				DisplayInfoBox("OpenMP accelerated realtime fullscreen fire");
				break;
			}
			case 4:
			{
				Swarm::Draw();
				DisplayInfoBox("Realtime particle swarm - 30.000 particles");
				break;
			}
			case 5:
			{
				Landscape::Draw();
				DisplayInfoBox("Textured dotted landscape");
				break;
			}
			case 6:
			{
				Starfield::Draw();
				DisplayInfoBox("OpenMP accelerated 3D starfield - 15.000 stars");
				break;
			}
			case 7:
			{
				VectorCube::Draw();
				DisplayInfoBox("Vector cube - filled polygons");
				break;
			}
			case 8:
			{
				Lens::Draw();
				DisplayInfoBox("Realtime lens");
				break;
			}
			case 9:
			{
				Copperbars::Draw();
				DisplayInfoBox("Copperbars");
				break;
			}
			case 10:
			{
				Tunnel::Draw();
				DisplayInfoBox("OpenMP accelerated textured tunnel");
				break;
			}
			case 11:
			{
				Morph::Draw();
				DisplayInfoBox("Realtime morph - 10.000 dots");
				break;
			}
			case 12:
			{
				GouraudShade::Draw();
				DisplayInfoBox("Realtime torus knot with gouraud shading - 24.000 vertices");
				break;
			}
			case 13:
			{
				RotoZoom::Draw();
				DisplayInfoBox("RotoZoomer");
				break;
			}
			case 14:
			{
				Moiree::Draw();
				DisplayInfoBox("OpenMP accelerated moiree");
				break;
			}
			case 15:
			{
				Julia::Draw();
				DisplayInfoBox("Realtime Julia set");
				break;
			}
			case 16:
			{
				Bobs::Draw();
				DisplayInfoBox("SineScroller and Bobs");
				break;
			}
			case 17:
			{
				PerlinGFX::DrawParts();
				DisplayInfoBox("Multithreaded, Perlin noise generated gfx");
				break;
			}
			case 18:
			{
				Raytracer::Draw();
				DisplayInfoBox("OpenMP accelerated realtime raytracing");
				break;
			}
			case 19:
			{
				lwmf::ClearTexture(ScreenTexture, Distrib1(Engine));
				DisplayInfoBox(FillrateTestString);
				break;
			}
			case 20:
			{
				PrimitivesTest::Draw();
				DisplayInfoBox("Primitives test");
				break;
			}
			case 21:
			{
				BitmapTest::Draw();
				DisplayInfoBox("Bitmap resize & blitting test");
				break;
			}
			case 22:
			{
				Circle::Draw();
				DisplayInfoBox("Perlin Noise Circle/Band");
				break;
			}
			default: {}
		}

		// Bring the pixelbuffer to screen
		ScreenTextureShader.RenderLWMFTexture(ScreenTexture, false, 1.0F);
		lwmf::SwapBuffer();
	}

	Music.Close();
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
