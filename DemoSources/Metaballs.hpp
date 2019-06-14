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
			std::int_fast32_t x{};
			std::int_fast32_t y{};
			std::int_fast32_t VelocityX{};
			std::int_fast32_t VelocityY{};
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
			Ball.x += Ball.VelocityX;

			if (Ball.x > ScreenTexture.Width || Ball.x < 0)
			{
				Ball.VelocityX *= -1;
			}

			Ball.y += Ball.VelocityY;

			if (Ball.y > ScreenTexture.Height || Ball.y < 0)
			{
				Ball.VelocityY *= -1;
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
					const float BallXTemp{ static_cast<float>(x - Ball.x) };
					const float BallYTemp{ static_cast<float>(y - Ball.y) };

					BallSum += 3.0F / std::sqrtf(BallXTemp * BallXTemp + BallYTemp * BallYTemp);
				}

				lwmf::SetPixel(ScreenTexture, x, y, BallSum > 0.035F ? MetaballColor : lwmf::RGBAtoINT(static_cast<std::int_fast32_t>((10000.0F * BallSum) - 100.0F), 0, 0, 255));
			}
		}

		lwmf::RenderText(ScreenTexture, "OpenMP accelerated realtime metaballs", 10, 10, 0xFFFFFFFF);
	}


} // namespace Metaballs

