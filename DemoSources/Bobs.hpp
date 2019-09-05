#pragma once

#include <cstdint>
#include <vector>
#include <cmath>

namespace Bobs
{


	inline lwmf::TextureStruct Bob1BMP;
	inline lwmf::TextureStruct Bob2BMP;
	inline lwmf::TextureStruct CharMapBMP;

	inline std::vector<lwmf::FloatPointStruct> Bob1Coord(512);
	inline std::vector<lwmf::FloatPointStruct> Bob2Coord(512);

	constexpr std::int_fast32_t PatternWidth{ 600 >> 1};
	constexpr std::int_fast32_t PatternHeight{ 300 >> 1 };

	inline void Init()
	{
		lwmf::LoadBMP(Bob1BMP, "./DemoGFX/Bob1.bmp");
		lwmf::LoadBMP(Bob2BMP, "./DemoGFX/Bob2.bmp");
		lwmf::LoadPNG(CharMapBMP, "./DemoGFX/SinusScrollerChars.png");

		for (std::int_fast32_t i{}; i < 512; ++i)
		{
			const float Temp{ (i * 0.703125F) * 0.0174532F };

			Bob1Coord[i] = { std::sinf(Temp) * PatternWidth + PatternWidth, std::cosf(Temp) * PatternHeight + PatternHeight };
			Bob2Coord[i] = { std::cosf(Temp) * PatternWidth + PatternWidth, std::sinf(Temp) * PatternHeight + PatternHeight };
		}
	}

	inline void Draw()
	{
		static const std::int_fast32_t TransparentColor{ lwmf::RGBAtoINT(255, 0, 0, 255) };

		lwmf::ClearTexture(ScreenTexture, 0);

		// Bobs
		static constexpr std::int_fast32_t MaxBobs{ 24 };
		static std::int_fast32_t Bob1{ 0 };
		static std::int_fast32_t Bob2{ 40 };

		for (std::int_fast32_t i{}; i <= MaxBobs; ++i)
		{
			lwmf::BlitTransTexture(Bob1BMP, ScreenTexture, static_cast<std::int_fast32_t>(Bob1Coord[Bob1 & 511].X) + (ScreenTexture.WidthMid - PatternWidth), static_cast<std::int_fast32_t>(Bob1Coord[Bob2 & 511].Y) + ((ScreenTexture.HeightMid - PatternHeight) >> 1), TransparentColor);
			lwmf::BlitTransTexture(Bob2BMP, ScreenTexture, static_cast<std::int_fast32_t>(Bob2Coord[Bob1 + 512 & 511].X) + (ScreenTexture.WidthMid - PatternWidth), static_cast<std::int_fast32_t>(Bob2Coord[Bob2 + 512 & 511].Y) + ((ScreenTexture.HeightMid - PatternHeight) >> 1), TransparentColor);

			Bob1 += 20;
			Bob2 += 20;
		}

		Bob1 += 8;
		Bob2 += 10;

		// SineScroller
		static constexpr std::int_fast32_t CharWidth{ 37 };
		static constexpr std::int_fast32_t CharHeight{ 39 };
		static constexpr std::int_fast32_t Speed{ 8 };
		static const std::string CharMap{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?!().," };
		static const std::string Text{ "...HELLO FOLKS, THIS IS JUST A LITTLE SCROLLER...AND SOME BOBS !!!" };
		static const std::int_fast32_t TextLength{ static_cast<std::int_fast32_t>(Text.length()) };
		static const std::int_fast32_t CharMapLength{ static_cast<std::int_fast32_t>(CharMap.length()) };
		static const std::int_fast32_t ScrollLength{ static_cast<std::int_fast32_t>(TextLength) * (CharWidth + 5) };
		static std::int_fast32_t ScrollX{ ScreenTexture.Width };

		for (std::int_fast32_t XPos{ ScrollX }, i{}; i < TextLength; ++i)
		{
			for (std::int_fast32_t YSine{ ScreenTexture.Height - 100 + static_cast<std::int_fast32_t>(std::sinf(0.02F * XPos) * 30.0F) }, CharX{}, j{}; j < CharMapLength; ++j)
			{
				if (Text[i] == CharMap[j])
				{
					for (std::int_fast32_t y{}; y < CharHeight; ++y, ++YSine)
					{
						for (std::int_fast32_t x1{}, x{ CharX }; x < CharX + CharWidth; ++x, ++x1)
						{
							if (XPos + x1 >= 0 && XPos + x1 < ScreenTexture.Width)
							{
								if (const std::int_fast32_t Color{ lwmf::GetPixel(CharMapBMP, x, y) }; Color != TransparentColor)
								{
									lwmf::SetPixel(ScreenTexture, XPos + x1, YSine, Color);
								}
							}
						}
					}

					break;
				}

				CharX += CharWidth + 1;
			}

			XPos += CharWidth + 5;
		}

		ScrollX -= Speed;

		if (ScrollX < -ScrollLength)
		{
			ScrollX = ScreenTexture.Width;
		}

		lwmf::RenderText(ScreenTexture, "SineScroller and Bobs", 10, 10, 0xFFFFFFFF);
	}


} // namespace Bobs