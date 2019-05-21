#pragma once

#include <cstdint>
#include <cmath>

namespace Moiree
{


	inline void Draw()
	{
		const float Time{ GetTickCount() / 1000.0F };
		const lwmf::FloatPointStruct C1 { std::sinf(Time / 2.0F) * lwmf::ViewportWidth / 3.0F + lwmf::ViewportWidthMid, std::sinf(Time / 4.0F) * lwmf::ViewportHeight / 3.0F + lwmf::ViewportHeightMid };
		const lwmf::FloatPointStruct C2 { std::cosf(Time / 3.0F) * lwmf::ViewportWidth / 3.0F + lwmf::ViewportWidthMid, std::cosf(Time) * lwmf::ViewportHeight / 3.0F + lwmf::ViewportHeightMid };

		for (std::int_fast32_t Offset{}, y{}; y < lwmf::ViewportHeight; ++y)
		{
			const float DY1{ (y - C1.Y) * (y - C1.Y) };
			const float DY2{ (y - C2.Y) * (y - C2.Y) };

			for (std::int_fast32_t x{}; x < lwmf::ViewportWidth; ++x)
			{
				const std::int_fast32_t Color{ ((static_cast<std::int_fast32_t>(static_cast<std::int_fast32_t>(std::sqrtf(((x - C1.X) * (x - C1.X)) + DY1)) ^ static_cast<std::int_fast32_t>(std::sqrtf(((x - C2.X) * (x - C2.X)) + DY2))) >> 4) & 1) * 255 };
				lwmf::PixelBuffer[Offset++] = lwmf::RGBAtoINT(0, Color >> 1, Color, 255);
			}
		}

		lwmf::RenderText("Moiree", 10, 10, 0xFFFFFFFF);
	}


} // namespace Moiree