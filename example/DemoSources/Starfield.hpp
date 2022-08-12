#pragma once

#include <cstdint>
#include <array>
#include <random>

namespace Starfield
{


	inline void Draw()
	{
		struct StarStruct final
		{
			lwmf::IntPointStruct Pos{};
			float z{};
		};

		constexpr std::int_fast32_t MaxStars{ 15000 };
		constexpr float MaxDepth{ 48.0F };
		const std::int_fast32_t StarBorderColor{ lwmf::RGBAtoINT(0, 0, 160, 255) };
		static std::array<StarStruct, MaxStars> Stars{};

		std::uniform_int_distribution<std::int_fast32_t> Distrib1(-128, 128);
		std::uniform_real<float> Distrib2(1.0F, MaxDepth);

		lwmf::ClearTexture(ScreenTexture, 0x00000000);

		for (auto& Star : Stars)
		{
			Star.z -= 0.19F;

			if (Star.z < 0.0F)
			{
				Star.Pos = { Distrib1(RNG), Distrib1(RNG) };
				Star.z = Distrib2(RNG);
			}

			const float Factor{ 128.0F / Star.z };
			const std::int_fast32_t ColorPart{ static_cast<std::int_fast32_t>(255.0F - Star.z * 3.0F) };

			lwmf::FilledCircle(ScreenTexture, static_cast<std::int_fast32_t>(Star.Pos.X * Factor) + ScreenTexture.WidthMid,
				static_cast<std::int_fast32_t>(Star.Pos.Y * Factor) + ScreenTexture.HeightMid,
				static_cast<std::int_fast32_t>((1.0F - (Star.z) / MaxDepth) * 6.0F), StarBorderColor, lwmf::RGBAtoINT(ColorPart, ColorPart, ColorPart, ColorPart));
		}

		DisplayInfoBox("3D starfield - " + std::to_string(MaxStars) + " stars");
	}


} // namespace Starfield