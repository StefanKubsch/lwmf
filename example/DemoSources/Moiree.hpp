#pragma once

#include <Windows.h>
#include <cstdint>
#include <cmath>

namespace Moiree
{


	inline void Draw()
	{
		const float Time{ GetTickCount() / 1000.0F };
		const lwmf::FloatPointStruct C1 { std::sinf(Time / 2.0F) * ScreenTexture.Width / 3.0F + ScreenTexture.WidthMid, std::sinf(Time / 4.0F) * ScreenTexture.Height / 3.0F + ScreenTexture.HeightMid };
		const lwmf::FloatPointStruct C2 { std::cosf(Time / 3.0F) * ScreenTexture.Width / 3.0F + ScreenTexture.WidthMid, std::cosf(Time) * ScreenTexture.Height / 3.0F + ScreenTexture.HeightMid };

		for (std::int_fast32_t y{}; y < ScreenTexture.Height; ++y)
		{
			const float DY1{ (y - C1.Y) * (y - C1.Y) };
			const float DY2{ (y - C2.Y) * (y - C2.Y) };

			for (std::int_fast32_t x{}; x < ScreenTexture.Width; ++x)
			{
				const std::int_fast32_t Color{ ((static_cast<std::int_fast32_t>(static_cast<std::int_fast32_t>(std::sqrtf(((x - C1.X) * (x - C1.X)) + DY1)) ^ static_cast<std::int_fast32_t>(std::sqrtf(((x - C2.X) * (x - C2.X)) + DY2))) >> 4) & 1) * 255 };
				lwmf::SetPixel(ScreenTexture, x, y, lwmf::RGBAtoINT(0, Color >> 1, Color, 255));
			}
		}

		DisplayInfoBox("Moiree");
	}


} // namespace Moiree