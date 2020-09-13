#pragma once

#include <array>
#include <cstdint>
#include <cmath>

namespace Metaballs
{


	inline void Draw()
	{
		struct MetaballStruct final
		{
			lwmf::IntPointStruct Pos{};
			lwmf::IntPointStruct Velocity{};
		};

		static std::array<MetaballStruct, 4> Metaballs
		{ {
			{ {800, 600}, {-8, -8} },
			{ {490, 40}, {4, 2} },
			{ {10, 10}, {5, 6} },
			{ {30, 500}, {-5, -6} }
		} };

		for (auto& Ball : Metaballs)
		{
			Ball.Pos.X += Ball.Velocity.X;

			if (static_cast<std::uint_fast32_t>(Ball.Pos.X) > static_cast<std::uint_fast32_t>(ScreenTexture.Width))
			{
				Ball.Velocity.X *= -1;
			}

			Ball.Pos.Y += Ball.Velocity.Y;

			if (static_cast<std::uint_fast32_t>(Ball.Pos.Y) > static_cast<std::uint_fast32_t>(ScreenTexture.Height))
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

				lwmf::SetPixel(ScreenTexture, x, y, BallSum > 0.035F ? 0xFF0000FF : lwmf::RGBAtoINT(static_cast<std::int_fast32_t>((10000.0F * BallSum) - 100.0F), 0, 0, 255));
			}
		}

		DisplayInfoBox("OpenMP accelerated realtime metaballs");
	}


} // namespace Metaballs

