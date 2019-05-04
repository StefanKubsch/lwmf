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
			std::int_fast32_t xvel{};
			std::int_fast32_t yvel{};
		};

		static std::vector<MetaballStruct> Metaballs
		{
			{ 600, 600, -8, -8 },
			{ 490, 40, 4, 2 },
			{ 10, 10, 5, 6 }
		};

		static const std::int_fast32_t MetaballColor{ lwmf::RGBAtoINT(255, 0, 0, 255) };

		for (auto& Ball : Metaballs)
		{
			Ball.x += Ball.xvel;

			if (Ball.x > lwmf::ViewportWidth || Ball.x < 0)
			{
				Ball.xvel *= -1;
			}

			Ball.y += Ball.yvel;

			if (Ball.y > lwmf::ViewportHeight || Ball.y < 0)
			{
				Ball.yvel *= -1;
			}
		}

		for (std::int_fast32_t Offset{}, y{}; y < lwmf::ViewportHeight; ++y)
		{
			for (std::int_fast32_t x{}; x < lwmf::ViewportWidth; ++x)
			{
				float BallSum{};

				for (auto& Ball : Metaballs)
				{
					const float BallXTemp{ static_cast<float>(x - Ball.x) };
					const float BallYTemp{ static_cast<float>(y - Ball.y) };

					BallSum += 3.0F / std::sqrtf(BallXTemp * BallXTemp + BallYTemp * BallYTemp);
				}

				lwmf::PixelBuffer[Offset++] = BallSum > 0.035F ? MetaballColor : lwmf::RGBAtoINT(static_cast<std::int_fast32_t>((10000.0F * BallSum) - 100.0F), 0, 0, 255);
			}
		}

		lwmf::RenderText("Realtime metaballs", 10, 10, 0xFFFFFFFF);
	}


} // namespace Metaballs

