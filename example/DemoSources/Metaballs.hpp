#pragma once

#include <vector>
#include <cstdint>
#include <cmath>

namespace Metaballs
{


	inline void Draw()
	{
		struct MetaballStruct final
		{
			lwmf::IntPointStruct Pos;
			lwmf::IntPointStruct Velocity;
		};

		static std::vector<MetaballStruct> Metaballs
		{
			{ 800, 600, -8, -8 },
			{ 490, 40, 4, 2 },
			{ 10, 10, 5, 6 },
			{ 30, 500, -5, -6 }
		};

		static const std::int_fast32_t MetaballColor{ lwmf::RGBAtoINT(255, 0, 0, 255) };

		for (auto& Ball : Metaballs)
		{
			Ball.Pos.X += Ball.Velocity.X;

			if (Ball.Pos.X > ScreenTexture.Width || Ball.Pos.X < 0)
			{
				Ball.Velocity.X *= -1;
			}

			Ball.Pos.Y += Ball.Velocity.Y;

			if (Ball.Pos.Y > ScreenTexture.Height || Ball.Pos.Y < 0)
			{
				Ball.Velocity.Y *= -1;
			}
		}

		#pragma omp parallel for
		for (std::int_fast32_t y{}; y < ScreenTexture.Height; ++y)
		{
			for (std::int_fast32_t x{}; x < ScreenTexture.Width; ++x)
			{
				float BallSum{};

				for (auto& Ball : Metaballs)
				{
					const lwmf::FloatPointStruct BallTemp{ static_cast<float>(x - Ball.Pos.X), static_cast<float>(y - Ball.Pos.Y) };
					BallSum += 3.0F / std::sqrtf(BallTemp.X * BallTemp.X + BallTemp.Y * BallTemp.Y);
				}

				lwmf::SetPixel(ScreenTexture, x, y, BallSum > 0.035F ? MetaballColor : lwmf::RGBAtoINT(static_cast<std::int_fast32_t>((10000.0F * BallSum) - 100.0F), 0, 0, 255));
			}
		}

		lwmf::RenderText(ScreenTexture, "OpenMP accelerated realtime metaballs", 10, 10, 0xFFFFFFFF);
	}


} // namespace Metaballs
