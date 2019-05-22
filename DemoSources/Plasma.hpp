#pragma once

#include <cstdint>
#include <cmath>

namespace Plasma
{


	inline void Draw()
	{
		static std::int_fast32_t PlasmaCount1{ 5 };
		static std::int_fast32_t PlasmaCount2{ 125 };
		static std::int_fast32_t PlasmaCount3{ 250 };
		static std::int_fast32_t PlasmaSpeed1{ 2 };
		static std::int_fast32_t PlasmaSpeed2{ 1 };
		static std::int_fast32_t PlasmaSpeed3{ 3 };

		if (PlasmaCount1 > 255 || PlasmaCount1 <= 1)
		{
			PlasmaSpeed1 = -PlasmaSpeed1;
		}

		if (PlasmaCount2 > 255 || PlasmaCount2 <= 1)
		{
			PlasmaSpeed2 = -PlasmaSpeed2;
		}

		if (PlasmaCount3 > 255 || PlasmaCount3 <= 1)
		{
			PlasmaSpeed3 = -PlasmaSpeed3;
		}

		PlasmaCount1 += PlasmaSpeed1;
		PlasmaCount2 -= PlasmaSpeed2;
		PlasmaCount3 += PlasmaSpeed3;

		for (std::int_fast32_t Offset{}, y{}; y < ScreenTexture.Height; ++y)
		{
			for (std::int_fast32_t x{}; x < ScreenTexture.Width; ++x)
			{
				const std::int_fast32_t Color{ static_cast<std::int_fast32_t>((128.0F * std::sinf(x / 64.0F)) + (64.0F * std::cosf(y / 64.0F))) };
				ScreenTexture.Pixels[Offset++] = lwmf::RGBAtoINT((Color + PlasmaCount3) & 255, (Color + PlasmaCount1) & 255, (Color + PlasmaCount2) & 255, 255);
			}
		}

		lwmf::RenderText(ScreenTexture, "Realtime plasma", 10, 10, 0xFFFFFFFF);
	}


} // namespace Plasma
