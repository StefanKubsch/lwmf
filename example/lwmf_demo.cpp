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

#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <string>
#include <string_view>
#include <array>
#include <format>
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
// The #defines are used for logging and errorhandling
#define LWMF_LOGGINGENABLED
#define LWMF_THROWEXCEPTIONS
#include "./src/lwmf.hpp"

// "Canvas" is the main render target in our demo!
inline lwmf::TextureStruct Canvas{};

// ...and this is the shader we use to bring "Canvas" to the screen!
inline lwmf::ShaderClass CanvasShader{};

// Init & seed random RNG
inline std::mt19937 RNG(std::random_device{}());

// Control variables for demo flow
inline std::int_fast32_t ActiveDemoPart{};

// Make an instance of lwmf::MP3Player for our background music
inline lwmf::MP3Player Music{};

inline void DisplayInfoBox(const std::string_view Partname)
{
	lwmf::FilledRectangle(Canvas, 0, 0, Canvas.Width, 115, 0x1F1F1F1F, 0x1F1F1F1F);
	lwmf::FPSCounter();
	lwmf::DisplayFPSCounter(Canvas, 10, 20, 0xFFFFFFFF);
	lwmf::RenderText(Canvas, Partname, 10, 10, 0xFFFFFFFF);
	lwmf::RenderText(Canvas, "Music duration in seconds: " + std::format("{:.2f}", Music.GetDuration()), 10, 40, 0xFFFFFFFF);
	lwmf::RenderText(Canvas, "Music position in seconds: " + std::format("{:.2f}", Music.GetPosition()), 10, 50, 0xFFFFFFFF);
	lwmf::Line(Canvas, 0, 115, Canvas.Width, 115, 0xFFFFFFFF);
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
#include "./DemoSources/TextureTest.hpp"
#include "./DemoSources/Circle.hpp"
#include "./DemoSources/Cubes.hpp"
#include "./DemoSources/PixelTest.hpp"
#include "./DemoSources/ThroughputTest.hpp"
#include "./DemoSources/Particles.hpp"
#include "./DemoSources/TextureRotationTest.hpp"
#include "./DemoSources/DLA.hpp"

constexpr std::array<void (*)(), 28> DemoParts
{
	Metaballs::Draw, Plasma::Draw, DotTunnel::Draw, Fire::Draw, Swarm::Draw, Landscape::Draw, Starfield::Draw,
	VectorCube::Draw, Lens::Draw, Copperbars::Draw, Tunnel::Draw, Morph::Draw, GouraudShade::Draw, RotoZoom::Draw, Moiree::Draw,
	Julia::Draw, Bobs::Draw, PerlinGFX::Draw, Raytracer::Draw, Cubes::Draw, Circle::Draw, Particles::Draw, DLA::Draw,
	TextureTest::Draw, TextureRotationTest::Draw, PixelTest::Draw, ThroughputTest::Draw, PrimitivesTest::Draw
};

std::int_fast32_t WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);

	// Here we create our logfile and write it initially to disk
	lwmf::Logging DemoLog("lwmf_demo.log");

	// Save the program instance for further use with lwmf
	lwmf::WindowInstance = hInstance;

	try
	{
		// Create window and OpenGL context
		lwmf::CreateOpenGLWindow(lwmf::WindowInstance, Canvas, 1280, 720, "lwmf demo - switch parts with CURSOR LEFT & RIGHT, ESC to exit!", false);

		// Set VSync: 0 = off, -1 = on (adaptive vsync = smooth as fuck)
		lwmf::SetVSync(-1);

		// Load OpenGL/wgl extensions -> you need to do this AFTER creating the OpenGL context!
		lwmf::InitOpenGLLoader();

		// Check for SSE
		lwmf::CheckForSSESupport();

		// Init the shaders used for rendering
		CanvasShader.LoadShader("Default", Canvas);
		CanvasShader.PrepareLWMFTexture(Canvas, 0, 0);

		// Initial clearance of window (looks better!)
		lwmf::ClearTexture(Canvas, 0x00000000);
		lwmf::ClearBuffer();
		lwmf::SwapBuffer();

		// Init raw devices (we only need keyboard support in our demo!)
		lwmf::RegisterRawInputDevice(lwmf::MainWindow, lwmf::DeviceIdentifier::HID_KEYBOARD);

		// Init audio -> load file from disk
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
		TextureTest::Init();
		Particles::Init();
		TextureRotationTest::Init();
		DLA::Init();
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

		// Call active demo part
		(*DemoParts[ActiveDemoPart])();;

		// Bring the pixelbuffer to screen
		lwmf::ClearBuffer();
		CanvasShader.RenderLWMFTexture(Canvas, false, 1.0F);
		lwmf::SwapBuffer();
	}

	// Clean up everything
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
		case WM_INPUT:
		{
			RAWINPUT RawDev{};
			UINT DataSize{ sizeof(RAWINPUT) };
			const UINT HeaderSize{ sizeof(RAWINPUTHEADER) };
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
								ActiveDemoPart < DemoParts.size() - 1 ? ++ActiveDemoPart : ActiveDemoPart = 0;
								lwmf::ClearTexture(Canvas, 0x00000000);
								(ActiveDemoPart >= 22 && ActiveDemoPart < DemoParts.size()) ? lwmf::SetVSync(0) : lwmf::SetVSync(-1);
								break;
							}
							case VK_LEFT:
							{
								ActiveDemoPart > 0 ? --ActiveDemoPart : ActiveDemoPart = static_cast<std::int_fast32_t>(DemoParts.size()) - 1;
								lwmf::ClearTexture(Canvas, 0x00000000);
								(ActiveDemoPart >= 22 && ActiveDemoPart < DemoParts.size()) ? lwmf::SetVSync(0) : lwmf::SetVSync(-1);
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

	return DefWindowProcA(hWnd, message, wParam, lParam);
}