#pragma once

#include <cstdint>

namespace PerlinGFX
{


	inline void Draw()
	{
		static lwmf::PerlinNoise PGFX;
		static float NoiseFactor{ 0.4F };

		for (std::int_fast32_t Offset{}, i{}; i < lwmf::ViewportHeight; ++i)
		{
			const float y{ static_cast<float>(i) / lwmf::ViewportHeight };

			for (std::int_fast32_t j{}; j < lwmf::ViewportWidth; ++j)
			{
				const float x{ static_cast<float>(j) / lwmf::ViewportWidth };
				const float n{ (15.0F * PGFX.Noise(x, y, NoiseFactor)) - PGFX.Noise(15.0F * x, 15.0F * y, NoiseFactor) };

				lwmf::PixelBuffer[Offset++] = lwmf::RGBAtoINT(static_cast<std::int_fast32_t>(128.0F * n), static_cast<std::int_fast32_t>(n), static_cast<std::int_fast32_t>(255.0F * n), static_cast<std::int_fast32_t>(255.0F * n));
			}
		}

		NoiseFactor += 0.005F;

		lwmf::RenderText("Perlin noise generated gfx", 10, 10, 0xFFFFFFFF);
	}


} // namespace PerlinGFX