#pragma once

#include <cstdint>

namespace PerlinGFX
{


	enum class Renderpart : std::int_fast32_t
	{
		TopLeft,
		TopRight,
		DownLeft,
		DownRight
	};

	inline float NoiseFactor{ 0.4F };

	inline void Draw(const Renderpart Part)
	{
		static lwmf::PerlinNoise PGFX;

		std::int_fast32_t StartX{};
		std::int_fast32_t EndX{};
		std::int_fast32_t StartY{};
		std::int_fast32_t EndY{};

		switch (Part)
		{
			case Renderpart::TopLeft:
			{
				EndX = lwmf::ViewportWidthMid;
				EndY = lwmf::ViewportHeightMid;
				break;
			}
			case Renderpart::TopRight:
			{
				StartX = lwmf::ViewportWidthMid;
				EndX = lwmf::ViewportWidth;
				EndY = lwmf::ViewportHeightMid;
				break;
			}
			case Renderpart::DownLeft:
			{
				EndX = lwmf::ViewportWidthMid;
				StartY = lwmf::ViewportHeightMid;
				EndY = lwmf::ViewportHeight;
				break;
			}
			case Renderpart::DownRight:
			{
				StartX = lwmf::ViewportWidthMid;
				EndX = lwmf::ViewportWidth;
				StartY = lwmf::ViewportHeightMid;
				EndY = lwmf::ViewportHeight;
				break;
			}

			default: {}
		}

		for (std::int_fast32_t y{ StartY }; y < EndY; ++y)
		{
			const float PosY{ static_cast<float>(y) / lwmf::ViewportHeight };

			for (std::int_fast32_t x{ StartX }; x < EndX; ++x)
			{
				const float PosX{ static_cast<float>(x) / lwmf::ViewportWidth };
				const float n{ (15.0F * PGFX.Noise(PosX, PosY, NoiseFactor)) - PGFX.Noise(15.0F * PosX, 15.0F * PosY, NoiseFactor) };

				lwmf::PixelBuffer[y * lwmf::ViewportWidth + x] = lwmf::RGBAtoINT(static_cast<std::int_fast32_t>(128.0F * n), static_cast<std::int_fast32_t>(n), static_cast<std::int_fast32_t>(255.0F * n), static_cast<std::int_fast32_t>(255.0F * n));
			}
		}
	}

	inline void DrawParts()
	{
		static lwmf::Multithreading Threadpool;

		Threadpool.AddThread(&Draw, Renderpart::TopLeft);
		Threadpool.AddThread(&Draw, Renderpart::TopRight);
		Threadpool.AddThread(&Draw, Renderpart::DownLeft);
		Threadpool.AddThread(&Draw, Renderpart::DownRight);
		Threadpool.WaitForThreads();

		NoiseFactor += 0.002F;

		lwmf::RenderText("Multithreaded, Perlin noise generated gfx", 10, 10, 0xFFFFFFFF);
	}


} // namespace PerlinGFX