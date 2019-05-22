#pragma once

#include <cstdint>
#include <vector>
#include <cmath>

namespace Copperbars
{


	inline std::vector<lwmf::ColorStruct> CopperList(46);
	inline std::vector<std::int_fast32_t> SinTable(360);

	inline void Init()
	{
		static const std::vector<std::int_fast32_t> Colors{ 34, 68, 102, 136, 170, 204, 238, 255, 238, 204, 170, 136, 102, 68, 34 };

		for (std::int_fast32_t i{}; i < 360; ++i)
		{
			SinTable[i] = static_cast<std::int_fast32_t>(std::sinf(static_cast<float>(i) * 0.0174532F) * 100.0F);
		}

		for (std::int_fast32_t i{}; i < 15; ++i)
		{
			CopperList[i + 1].Red = Colors[i];
			CopperList[i + 16].Green = Colors[i];
			CopperList[i + 31].Blue = Colors[i];
		}
	}

	inline void DrawCopperbar(std::int_fast32_t y, const std::int_fast32_t ColorAdd)
	{
		for (std::int_fast32_t i{}; i < 15; ++i, ++y)
		{
			lwmf::Line(ScreenTexture, 0, y, ScreenTexture.Width, y, lwmf::RGBAtoINT(CopperList[i + ColorAdd].Red, CopperList[i + ColorAdd].Green, CopperList[i + ColorAdd].Blue, 255));
		}
	}

	inline void Draw()
	{
		static constexpr std::int_fast32_t Speed{ 3 };
		static std::vector<std::int_fast32_t> RedBars{ 96, 88, 80, 72 };
		static std::vector<std::int_fast32_t> GreenBars{ 64, 56, 48, 40 };
		static std::vector<std::int_fast32_t> BlueBars{ 32, 24, 16, 8 };

		lwmf::ClearTexture(ScreenTexture, 0);

		for (std::int_fast32_t i{ 3 }; i >= 0; --i)
		{
			DrawCopperbar(ScreenTexture.HeightMid + SinTable[BlueBars[i]], 31);
			BlueBars[i] = (BlueBars[i] + Speed) % 360;
		}

		for (std::int_fast32_t i{ 3 }; i >= 0; --i)
		{
			DrawCopperbar(ScreenTexture.HeightMid + SinTable[GreenBars[i]], 16);
			GreenBars[i] = (GreenBars[i] + Speed) % 360;
		}

		for (std::int_fast32_t i{ 3 }; i >= 0; --i)
		{
			DrawCopperbar(ScreenTexture.HeightMid + SinTable[RedBars[i]], 1);
			RedBars[i] = (RedBars[i] + Speed) % 360;
		}

		lwmf::RenderText(ScreenTexture, "Copperbars", 10, 10, 0xFFFFFFFF);
	}


} // namespace Copperbars