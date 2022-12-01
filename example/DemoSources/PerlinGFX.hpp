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

	inline void DrawPart(const Renderpart Part)
	{
		static lwmf::PerlinNoise PGFX;

		lwmf::IntPointStruct Start{};
		lwmf::IntPointStruct End{};

		switch (Part)
		{
			case Renderpart::TopLeft:
			{
				End = { Canvas.WidthMid, Canvas.HeightMid };
				break;
			}
			case Renderpart::TopRight:
			{
				Start.X = Canvas.WidthMid;
				End = { Canvas.Width, Canvas.HeightMid };
				break;
			}
			case Renderpart::DownLeft:
			{
				Start.Y = Canvas.HeightMid;
				End = { Canvas.WidthMid, Canvas.Height };
				break;
			}
			case Renderpart::DownRight:
			{
				Start = { Canvas.WidthMid, Canvas.HeightMid };
				End = { Canvas.Width, Canvas.Height };
				break;
			}

			default: {}
		}

		for (std::int_fast32_t y{ Start.Y }; y < End.Y; ++y)
		{
			const float PosY{ static_cast<float>(y) / Canvas.Height };

			for (std::int_fast32_t x{ Start.X }; x < End.X; ++x)
			{
				const float PosX{ static_cast<float>(x) / Canvas.Width };
				const float n{ (15.0F * PGFX.Noise(PosX, PosY, NoiseFactor)) - PGFX.Noise(15.0F * PosX, 15.0F * PosY, NoiseFactor) };

				lwmf::SetPixel(Canvas, x, y, lwmf::RGBAtoINT(static_cast<std::int_fast32_t>(128.0F * n), static_cast<std::int_fast32_t>(n), static_cast<std::int_fast32_t>(255.0F * n), static_cast<std::int_fast32_t>(255.0F * n)));
			}
		}
	}

	inline void Draw()
	{
		static lwmf::Multithreading Threadpool;

		Threadpool.AddThread(&DrawPart, Renderpart::TopLeft);
		Threadpool.AddThread(&DrawPart, Renderpart::TopRight);
		Threadpool.AddThread(&DrawPart, Renderpart::DownLeft);
		Threadpool.AddThread(&DrawPart, Renderpart::DownRight);
		Threadpool.WaitForThreads();

		NoiseFactor += 0.002F;

		DisplayInfoBox("Multithreaded Perlin noise generated gfx");
	}


} // namespace PerlinGFX