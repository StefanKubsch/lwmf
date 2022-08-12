#pragma once

#include <cstdint>
#include <array>
#include <cmath>
#include <random>

namespace Morph
{


	struct DotStruct final
	{
		float x{};
		float y{};
		float z{};
	};

	inline constexpr std::int_fast32_t NumberOfPoints{ 10000 };
	inline std::array<DotStruct, NumberOfPoints> Sphere{};
	inline std::array<DotStruct, NumberOfPoints> Torus{};
	inline std::array<DotStruct, NumberOfPoints> Morph{};

	inline void Init()
	{
		std::uniform_real<float> Distrib(0.0F, RAND_MAX);

		for (std::int_fast32_t i{}; i < NumberOfPoints; ++i)
		{
			float Theta{ (Distrib(RNG) / static_cast<float>(RAND_MAX)) * lwmf::DoublePI };
			float Phi{ (Distrib(RNG) / static_cast<float>(RAND_MAX)) * lwmf::DoublePI };

			Sphere[i].x = std::cosf(Phi) * std::sinf(Theta);
			Sphere[i].y = std::sinf(Phi) * std::sinf(Theta);
			Sphere[i].z = std::cosf(Theta);

			Theta = (Distrib(RNG) / static_cast<float>(RAND_MAX)) * lwmf::DoublePI;
			Phi = (Distrib(RNG) / static_cast<float>(RAND_MAX)) * lwmf::DoublePI;

			Torus[i].x = (0.8F + 0.4F * std::cosf(Theta)) * std::cosf(Phi);
			Torus[i].y = (0.8F + 0.4F * std::cosf(Theta)) * std::sinf(Phi);
			Torus[i].z = 0.4F * std::sinf(Theta);
		}
	}

	inline void ShowDots(const std::int_fast32_t CenterX, const std::int_fast32_t CenterY, float const Scale, float& DotAngle)
	{
		constexpr float Distance{ 128.0F };
		const float cosDotAngle{ std::cosf(DotAngle) };
		const float sinDotAngle{ std::sinf(DotAngle) };

		for (std::int_fast32_t i{}; i < NumberOfPoints; ++i)
		{
			float Ry{ Morph[i].y * std::cosf(0.0F) - Morph[i].z * std::sinf(0.0F) };
			float Rz{ Morph[i].y * std::sinf(0.0F) + Morph[i].z * std::cosf(0.0F) };
			float Rx{ Morph[i].x * cosDotAngle + Rz * sinDotAngle };
			Rz = -Morph[i].x * sinDotAngle + Rz * cosDotAngle;
			const float Temp{ Rx * cosDotAngle - Ry * sinDotAngle };
			Ry = Rx * sinDotAngle + Ry * cosDotAngle;
			Rx = Temp;

			const float ScaleProd{ Scale * Rz + Distance };
			const lwmf::IntPointStruct Pos{ CenterX + static_cast<std::int_fast32_t>(std::ceilf(Scale * Rx * Distance / ScaleProd * 1.2F)), CenterY + static_cast<std::int_fast32_t>(std::ceilf(Scale * Ry * Distance / ScaleProd)) };

			lwmf::SetPixelSafe(ScreenTexture, Pos.X, Pos.Y, lwmf::RGBAtoINT(static_cast<std::int_fast32_t>(Scale * (Rz / -20.0F) + 128), 0, 255, 255));
		}

		if (DotAngle >= lwmf::DoublePI)
		{
			DotAngle = 0.0F;
		}

		DotAngle += lwmf::RAD2DEG * 2;
	}

	inline void Draw()
	{
		constexpr float ScaleFactor{ 95.0F };
		static float Angle{};
		static std::int_fast32_t Count1{};
		static std::int_fast32_t Count2{};

		lwmf::ClearTexture(ScreenTexture, 0x00000000);

		if (Count1 < 256)
		{
			for (std::int_fast32_t i{}; i < NumberOfPoints; ++i)
			{
				Morph[i].x = (static_cast<std::int_fast32_t>(Count1) * Sphere[i].x + (256.0F - static_cast<std::int_fast32_t>(Count1)) * Torus[i].x) / 256.0F;
				Morph[i].y = (static_cast<std::int_fast32_t>(Count1) * Sphere[i].y + (256.0F - static_cast<std::int_fast32_t>(Count1)) * Torus[i].y) / 256.0F;
				Morph[i].z = (static_cast<std::int_fast32_t>(Count1) * Sphere[i].z + (256.0F - static_cast<std::int_fast32_t>(Count1)) * Torus[i].z) / 256.0F;
			}

			ShowDots(ScreenTexture.WidthMid, ScreenTexture.HeightMid, ScaleFactor, Angle);
		}
		else if (Count1 >= 256 && Count1 <= 512) //-V560
		{
			ShowDots(ScreenTexture.WidthMid, ScreenTexture.HeightMid, ScaleFactor, Angle);
		}
		else if (Count1 > 512 && Count1 <= 768) //-V560
		{
			for (std::int_fast32_t i{}; i < NumberOfPoints; ++i)
			{
				Morph[i].x = (static_cast<std::int_fast32_t>(Count2) * Torus[i].x + (256.0F - static_cast<std::int_fast32_t>(Count2)) * Sphere[i].x) / 256.0F;
				Morph[i].y = (static_cast<std::int_fast32_t>(Count2) * Torus[i].y + (256.0F - static_cast<std::int_fast32_t>(Count2)) * Sphere[i].y) / 256.0F;
				Morph[i].z = (static_cast<std::int_fast32_t>(Count2) * Torus[i].z + (256.0F - static_cast<std::int_fast32_t>(Count2)) * Sphere[i].z) / 256.0F;
			}

			ShowDots(ScreenTexture.WidthMid, ScreenTexture.HeightMid, ScaleFactor, Angle);

			Count2 += 2;
		}
		else if (Count1 > 768 && Count1 <= 1024) //-V560
		{
			ShowDots(ScreenTexture.WidthMid, ScreenTexture.HeightMid, ScaleFactor, Angle);
		}

		Count1 += 2;

		if (Count1 > 1024)
		{
			Count1 = 0;
			Count2 = 0;
		}

		DisplayInfoBox("Morph - " + std::to_string(NumberOfPoints) + " points");
	}


} // namespace Morph
