#pragma once

#include <cstdint>
#include <vector>
#include <cmath>

namespace Bobs
{


	inline lwmf::BitmapStruct Bob1BMP;
	inline lwmf::BitmapStruct Bob2BMP;
	inline lwmf::BitmapStruct CharMapBMP;

	inline std::vector<lwmf::FloatPointStruct> Bob1Coord(512);
	inline std::vector<lwmf::FloatPointStruct> Bob2Coord(512);

	inline void Init()
	{
		lwmf::LoadBMP(Bob1BMP, "./DemoGFX/Bob1.bmp");
		lwmf::LoadBMP(Bob2BMP, "./DemoGFX/Bob2.bmp");
		lwmf::LoadBMP(CharMapBMP, "./DemoGFX/SinusScrollerChars.bmp");

		for (std::int_fast32_t i{}; i < 512; ++i)
		{
			const float Temp{ (i * 0.703125F) * 0.0174532F };

			Bob1Coord[i] = { std::sinf(Temp) * (600 >> 1) + (600 >> 1), std::cosf(Temp) * (300 >> 1) + (300 >> 1) };
			Bob2Coord[i] = { std::cosf(Temp) * (600 >> 1) + (600 >> 1), std::sinf(Temp) * (300 >> 1) + (300 >> 1) };
		}
	}

	inline void Draw()
	{
		static const std::int_fast32_t TransparentColor{ lwmf::RGBAtoINT(255, 0, 0, 255) };

		lwmf::ClearPixelBuffer(0);

		// Bobs
		static constexpr std::int_fast32_t MaxBobs{ 24 };
		static std::int_fast32_t Bob1{ 0 };
		static std::int_fast32_t Bob2{ 40 };

		for (std::int_fast32_t i{}; i <= MaxBobs; ++i)
		{
			lwmf::BlitTransBMP(Bob1BMP, static_cast<std::int_fast32_t>(Bob1Coord[Bob1 & 511].X) + 80, static_cast<std::int_fast32_t>(Bob1Coord[Bob2 & 511].Y) + 50, TransparentColor);
			lwmf::BlitTransBMP(Bob2BMP, static_cast<std::int_fast32_t>(Bob2Coord[Bob1 + 512 & 511].X) + 80, static_cast<std::int_fast32_t>(Bob2Coord[Bob2 + 512 & 511].Y) + 50, TransparentColor);

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
		static std::int_fast32_t ScrollX{ lwmf::ViewportWidth };

		for (std::int_fast32_t XPos{ ScrollX }, i{}; i < TextLength; ++i)
		{
			for (std::int_fast32_t YSine{ lwmf::ViewportHeight - 100 + static_cast<std::int_fast32_t>(std::sinf(0.02F * XPos) * 30.0F) }, CharX{}, j{}; j < CharMapLength; ++j)
			{
				if (Text[i] == CharMap[j])
				{
					for (std::int_fast32_t y{}; y < CharHeight; ++y, ++YSine)
					{
						for (std::int_fast32_t x1{}, x{ CharX }; x < CharX + CharWidth; ++x, ++x1)
						{
							if (XPos + x1 >= 0 && XPos + x1 < lwmf::ViewportWidth)
							{
								const std::int_fast32_t Color{ CharMapBMP.BitmapData[y * CharMapBMP.Width + x] };

								if (Color != TransparentColor)
								{
									lwmf::PixelBuffer[YSine * lwmf::ViewportWidth + XPos + x1] = Color;
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
			ScrollX = lwmf::ViewportWidth;
		}

		lwmf::RenderText("SineScroller and Bobs", 10, 10, 0xFFFFFFFF);
	}


} // namespace Bobs