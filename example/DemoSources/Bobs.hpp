#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <cmath>

namespace Bobs
{


	inline lwmf::TextureStruct Bob1BMP{};
	inline lwmf::TextureStruct Bob2BMP{};

	inline std::array<lwmf::FloatPointStruct, 512> Bob1Coord{};
	inline std::array<lwmf::FloatPointStruct, 512> Bob2Coord{};

	constexpr std::int_fast32_t PatternWidth{ 300 };
	constexpr std::int_fast32_t PatternHeight{ 150 };

	struct Font
	{
		lwmf::TextureStruct CharMapBMP{};
		std::string Text{};
		std::string CharMap{};
		std::vector<std::int_fast32_t> Map{};
		std::int_fast32_t ScrollSpeed{};
		std::int_fast32_t TextLength{};
		std::int_fast32_t Length{};
		std::int_fast32_t CharWidth{};
		std::int_fast32_t CharHeight{};
		std::int_fast32_t CharSpacing{};
		std::int_fast32_t CharOverallWidth{};
	} ScrollFont;

	inline void Init()
	{
		//Init Bobs
		lwmf::LoadBMP(Bob1BMP, "./DemoGFX/Bob1.bmp");
		lwmf::LoadBMP(Bob2BMP, "./DemoGFX/Bob2.bmp");

		for (std::int_fast32_t i{}; i < 512; ++i)
		{
			const float Temp{ (i * 0.703125F) * 0.0174532F };
			const float SinTemp{ std::sinf(Temp) };
			const float CosTemp{ std::cosf(Temp) };

			Bob1Coord[i] = { SinTemp * PatternWidth + PatternWidth, CosTemp * PatternHeight + PatternHeight };
			Bob2Coord[i] = { CosTemp * PatternWidth + PatternWidth, SinTemp * PatternHeight + PatternHeight };
		}

		// Text & Font settings
		lwmf::LoadPNG(ScrollFont.CharMapBMP, "./DemoGFX/SinusScrollerChars.png");
		ScrollFont.Text = "...HELLO FOLKS, THIS IS JUST A LITTLE SINESCROLLER...AND SOME BOBS !!! ENJOY THE DEMO AND USE LWMF FOR YOUR OWN PROJECTS...";
		ScrollFont.CharMap = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?!().,";
		ScrollFont.CharWidth = 37;
		ScrollFont.CharHeight = 39;
		ScrollFont.CharSpacing = 1;
		ScrollFont.ScrollSpeed = 8;
		ScrollFont.CharOverallWidth = ScrollFont.CharWidth + ScrollFont.CharSpacing;
		ScrollFont.TextLength = static_cast<std::int_fast32_t>(ScrollFont.Text.length());
		ScrollFont.Length = ScrollFont.TextLength * ScrollFont.CharOverallWidth;
		ScrollFont.Map.resize(ScrollFont.TextLength, -1);

		// Pre-calc char positions in map
		for (std::int_fast32_t i{}; i < ScrollFont.TextLength; ++i)
		{
			for (std::int_fast32_t j{}, MapPos{}; j < static_cast<std::int_fast32_t>(ScrollFont.CharMap.length()); ++j)
			{
				if (ScrollFont.Text[i] == ScrollFont.CharMap[j])
				{
					ScrollFont.Map[i] = MapPos;
					break;
				}

				MapPos += ScrollFont.CharOverallWidth;
			}
		}
	}

	inline void Draw()
	{
		const std::int_fast32_t TransparentColor{ lwmf::RGBAtoINT(255, 0, 0, 255) };

		lwmf::ClearTexture(ScreenTexture, 0x00000000);

		// Bobs

		constexpr std::int_fast32_t MaxBobs{ 24 };
		static std::int_fast32_t Bob1{ 0 };
		static std::int_fast32_t Bob2{ 40 };

		for (std::int_fast32_t i{}; i <= MaxBobs; ++i)
		{
			lwmf::BlitTransTexture(Bob1BMP, ScreenTexture, static_cast<std::int_fast32_t>(Bob1Coord[Bob1 & 511].X) + (ScreenTexture.WidthMid - PatternWidth), static_cast<std::int_fast32_t>(Bob1Coord[Bob2 & 511].Y) + ((ScreenTexture.HeightMid - PatternHeight + 50) >> 1), TransparentColor);
			lwmf::BlitTransTexture(Bob2BMP, ScreenTexture, static_cast<std::int_fast32_t>(Bob2Coord[Bob1 + 512 & 511].X) + (ScreenTexture.WidthMid - PatternWidth), static_cast<std::int_fast32_t>(Bob2Coord[Bob2 + 512 & 511].Y) + ((ScreenTexture.HeightMid - PatternHeight + 50) >> 1), TransparentColor);

			Bob1 += 20;
			Bob2 += 20;
		}

		Bob1 += 8;
		Bob2 += 10;

		// SineScroller

		static std::int_fast32_t ScrollX{ ScreenTexture.Width };

		for (std::int_fast32_t i{}, XPos{ ScrollX }; i < ScrollFont.TextLength; ++i)
		{
			// check if actual character is "space" (-1)
			if (ScrollFont.Map[i] == -1)
			{
				XPos += ScrollFont.CharOverallWidth;
				continue;
			}

			for (std::int_fast32_t x1{}, x{ ScrollFont.Map[i] }; x < ScrollFont.Map[i] + ScrollFont.CharWidth; ++x1, ++x)
			{
				const std::int_fast32_t TempPosX{ XPos + x1 };

				if (TempPosX <= ScreenTexture.Width)
				{
					lwmf::BlitTransTexturePart(ScrollFont.CharMapBMP, x, 0, ScreenTexture, TempPosX, ScreenTexture.Height - 120 + static_cast<std::int_fast32_t>(std::sinf(0.01F * TempPosX) * 60.0F), 1, ScrollFont.CharHeight, TransparentColor);
				}
				else
				{
					break;
				}
			}

			XPos += ScrollFont.CharOverallWidth;
		}

		ScrollX -= ScrollFont.ScrollSpeed;

		if (ScrollX < -ScrollFont.Length)
		{
			ScrollX = ScreenTexture.Width;
		}

		DisplayInfoBox("SineScroller and Bobs");
	}


} // namespace Bobs