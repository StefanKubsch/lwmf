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
				EndX = ScreenTexture.WidthMid;
				EndY = ScreenTexture.HeightMid;
				break;
			}
			case Renderpart::TopRight:
			{
				StartX = ScreenTexture.WidthMid;
				EndX = ScreenTexture.Width;
				EndY = ScreenTexture.HeightMid;
				break;
			}
			case Renderpart::DownLeft:
			{
				EndX = ScreenTexture.WidthMid;
				StartY = ScreenTexture.HeightMid;
				EndY = ScreenTexture.Height;
				break;
			}
			case Renderpart::DownRight:
			{
				StartX = ScreenTexture.WidthMid;
				EndX = ScreenTexture.Width;
				StartY = ScreenTexture.HeightMid;
				EndY = ScreenTexture.Height;
				break;
			}

			default: {}
		}

		for (std::int_fast32_t y{ StartY }; y < EndY; ++y)
		{
			const float PosY{ static_cast<float>(y) / ScreenTexture.Height };

			for (std::int_fast32_t x{ StartX }; x < EndX; ++x)
			{
				const float PosX{ static_cast<float>(x) / ScreenTexture.Width };
				const float n{ (15.0F * PGFX.Noise(PosX, PosY, NoiseFactor)) - PGFX.Noise(15.0F * PosX, 15.0F * PosY, NoiseFactor) };

				ScreenTexture.Pixels[y * ScreenTexture.Width + x] = lwmf::RGBAtoINT(static_cast<std::int_fast32_t>(128.0F * n), static_cast<std::int_fast32_t>(n), static_cast<std::int_fast32_t>(255.0F * n), static_cast<std::int_fast32_t>(255.0F * n));
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

		lwmf::RenderText(ScreenTexture, "Multithreaded, Perlin noise generated gfx", 10, 10, 0xFFFFFFFF);
	}


} // namespace PerlinGFX