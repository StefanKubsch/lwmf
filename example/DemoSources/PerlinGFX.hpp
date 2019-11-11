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

		lwmf::IntPointStruct Start{};
		lwmf::IntPointStruct End{};

		switch (Part)
		{
			case Renderpart::TopLeft:
			{
				End = { ScreenTexture.WidthMid, ScreenTexture.HeightMid };
				break;
			}
			case Renderpart::TopRight:
			{
				Start.X = ScreenTexture.WidthMid;
				End = { ScreenTexture.Width, ScreenTexture.HeightMid };
				break;
			}
			case Renderpart::DownLeft:
			{
				Start.Y = ScreenTexture.HeightMid;
				End = { ScreenTexture.WidthMid, ScreenTexture.Height };
				break;
			}
			case Renderpart::DownRight:
			{
				Start = { ScreenTexture.WidthMid, ScreenTexture.HeightMid };
				End = { ScreenTexture.Width, ScreenTexture.Height };
				break;
			}

			default: {}
		}

		for (std::int_fast32_t y{ Start.Y }; y < End.Y; ++y)
		{
			const float PosY{ static_cast<float>(y) / ScreenTexture.Height };

			for (std::int_fast32_t x{ Start.X }; x < End.X; ++x)
			{
				const float PosX{ static_cast<float>(x) / ScreenTexture.Width };
				const float n{ (15.0F * PGFX.Noise(PosX, PosY, NoiseFactor)) - PGFX.Noise(15.0F * PosX, 15.0F * PosY, NoiseFactor) };

				lwmf::SetPixel(ScreenTexture, x, y, lwmf::RGBAtoINT(static_cast<std::int_fast32_t>(128.0F * n), static_cast<std::int_fast32_t>(n), static_cast<std::int_fast32_t>(255.0F * n), static_cast<std::int_fast32_t>(255.0F * n)));
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
	}


} // namespace PerlinGFX