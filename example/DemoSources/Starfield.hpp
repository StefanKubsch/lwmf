#pragma once

#include <cstdint>
#include <vector>
#include <random>

namespace Starfield
{


	inline void Draw()
	{
		struct StarStruct final
		{
			lwmf::IntPointStruct Pos;
			float z{};
		};

		static constexpr std::int_fast32_t MaxStars{ 15000 };
		static constexpr float MaxDepth{ 48.0F };
		static const std::int_fast32_t StarBorderColor{ lwmf::RGBAtoINT(0, 0, 160, 255) };
		static std::vector<StarStruct> Stars(MaxStars);

		static const std::uniform_int_distribution<std::int_fast32_t> Distrib1(-128, 128);
		static const std::uniform_real<float> Distrib2(1.0F, MaxDepth);

		lwmf::ClearTexture(ScreenTexture, 0);

		#pragma omp parallel for
		for (std::int_fast32_t i{}; i < MaxStars - 1; ++i)
		{
			Stars[i].z -= 0.19F;

			if (Stars[i].z < 0.0F)
			{
				Stars[i].Pos = { Distrib1(Engine), Distrib1(Engine) };
				Stars[i].z = Distrib2(Engine);
			}

			const float Factor{ 128.0F / Stars[i].z };
			const std::int_fast32_t ColorPart{ static_cast<std::int_fast32_t>(255.0F - Stars[i].z * 3.0F) };

			lwmf::FilledCircle(ScreenTexture, static_cast<std::int_fast32_t>(Stars[i].Pos.X * Factor) + ScreenTexture.WidthMid,
				static_cast<std::int_fast32_t>(Stars[i].Pos.Y * Factor) + ScreenTexture.HeightMid,
				static_cast<std::int_fast32_t>((1.0F - (Stars[i].z) / MaxDepth) * 6.0F), StarBorderColor, lwmf::RGBAtoINT(ColorPart, ColorPart, ColorPart, ColorPart));
		}

		lwmf::RenderText(ScreenTexture, "OpenMP accelerated 3D starfield - 15.000 stars", 10, 10, 0xFFFFFFFF);
	}


} // namespace Starfield