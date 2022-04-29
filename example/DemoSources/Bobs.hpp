#pragma once

#include <cstdint>
#include <array>
#include <cmath>

namespace Bobs
{


	inline lwmf::TextureStruct Bob1BMP{};
	inline lwmf::TextureStruct Bob2BMP{};

	inline std::array<lwmf::FloatPointStruct, 512> Bob1Coord{};
	inline std::array<lwmf::FloatPointStruct, 512> Bob2Coord{};

	constexpr std::int_fast32_t PatternWidth{ 300 };
	constexpr std::int_fast32_t PatternHeight{ 150 };

	struct Scrollfont
	{
		lwmf::TextureStruct CharMapBMP{};
		std::string Text{};
		std::string CharMap{};
		std::vector<std::int_fast32_t> Map{};
		std::int_fast32_t TextLength{};
		std::int_fast32_t Length{};
		std::int_fast32_t CharWidth{};
		std::int_fast32_t CharHeight{};
		std::int_fast32_t CharSpacing{};
		std::int_fast32_t CharOverallWidth{};
	} Font;

	inline void Init()
	{
		lwmf::LoadBMP(Bob1BMP, "./DemoGFX/Bob1.bmp");
		lwmf::LoadBMP(Bob2BMP, "./DemoGFX/Bob2.bmp");
		lwmf::LoadPNG(Font.CharMapBMP, "./DemoGFX/SinusScrollerChars.png");

		for (std::int_fast32_t i{}; i < 512; ++i)
		{
			const float Temp{ (i * 0.703125F) * 0.0174532F };
			const float SinTemp{ std::sinf(Temp) };
			const float CosTemp{ std::cosf(Temp) };

			Bob1Coord[i] = { SinTemp * PatternWidth + PatternWidth, CosTemp * PatternHeight + PatternHeight };
			Bob2Coord[i] = { CosTemp * PatternWidth + PatternWidth, SinTemp * PatternHeight + PatternHeight };
		}

		// Text & Font settings
		Font.Text = "...HELLO FOLKS, THIS IS JUST A LITTLE SINESCROLLER...AND SOME BOBS !!! ENJOY THE DEMO AND USE LWMF FOR YOUR OWN PROJECTS...";
		Font.CharMap = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?!().,";
		Font.CharWidth = 37;
		Font.CharHeight = 40;
		Font.CharSpacing = 1;
		Font.CharOverallWidth = Font.CharWidth + Font.CharSpacing;
		Font.TextLength = static_cast<std::int_fast32_t>(Font.Text.length());
		Font.Length = Font.TextLength * Font.CharOverallWidth;
		Font.Map.resize(Font.TextLength);

		// Pre-calc char positions in map
		for (std::int_fast32_t i{}; i < Font.TextLength; ++i)
		{
			// use -1 if char not found, "space"
			Font.Map[i] = -1;

			for (std::int_fast32_t j{}, MapPos{}; j < static_cast<std::int_fast32_t>(Font.CharMap.length()); ++j)
			{
				if (Font.Text[i] == Font.CharMap[j])
				{
					Font.Map[i] = MapPos;
					break;
				}

				MapPos += Font.CharOverallWidth;
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

		for (std::int_fast32_t i{}, XPos{ ScrollX }; i < Font.TextLength; ++i)
		{
			if (Font.Map[i] == -1)
			{
				XPos += Font.CharOverallWidth;
				continue;
			}

			for (std::int_fast32_t x1{}, x{ Font.Map[i] }; x < Font.Map[i] + Font.CharWidth; ++x1, ++x)
			{
				const std::int_fast32_t TempPosX{ XPos + x1 };

				if (TempPosX <= ScreenTexture.Width)
				{
					lwmf::BlitTransTexturePart(Font.CharMapBMP, x, 0, ScreenTexture, TempPosX, ScreenTexture.Height - 150 + static_cast<std::int_fast32_t>(std::sinf(0.01F * TempPosX) * 60.0F), 1, Font.CharHeight, TransparentColor);
				}
				else
				{
					break;
				}
			}

			XPos += Font.CharOverallWidth;
		}

		ScrollX -= 8;

		if (ScrollX < -Font.Length)
		{
			ScrollX = ScreenTexture.Width;
		}

		DisplayInfoBox("SineScroller and Bobs");
	}


} // namespace Bobs