#pragma once

#include <cstdint>
#include <vector>
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

	constexpr std::int_fast32_t NumberOfPoints{ 10000 };
	inline std::vector<DotStruct> Sphere(NumberOfPoints);
	inline std::vector<DotStruct> Torus(NumberOfPoints);
	inline std::vector<DotStruct> Morph(NumberOfPoints);

	inline void Init()
	{
		static std::mt19937_64 Engine;
		static const std::uniform_real<float> Distrib(0.0F, RAND_MAX);

		for (std::int_fast32_t i{}; i < NumberOfPoints; ++i)
		{
			float Theta{ (Distrib(Engine) / static_cast<float>(RAND_MAX)) * lwmf::DoublePI };
			float Phi{ (Distrib(Engine) / static_cast<float>(RAND_MAX)) * lwmf::DoublePI };

			Sphere[i].x = std::cosf(Phi) * std::sinf(Theta);
			Sphere[i].y = std::sinf(Phi) * std::sinf(Theta);
			Sphere[i].z = std::cosf(Theta);

			Theta = (Distrib(Engine) / static_cast<float>(RAND_MAX)) * lwmf::DoublePI;
			Phi = (Distrib(Engine) / static_cast<float>(RAND_MAX)) * lwmf::DoublePI;

			Torus[i].x = (0.8F + 0.4F * std::cosf(Theta)) * std::cosf(Phi);
			Torus[i].y = (0.8F + 0.4F * std::cosf(Theta)) * std::sinf(Phi);
			Torus[i].z = 0.4F * std::sinf(Theta);
		}
	}

	inline void ShowDots(const std::int_fast32_t CenterX, const std::int_fast32_t CenterY, float const Scale, float& DotAngle)
	{
		static constexpr float Distance{ 128.0F };

		lwmf::ClearTexture(ScreenTexture, 0);

		for (std::int_fast32_t i{}; i < NumberOfPoints; ++i)
		{
			float Ry{ Morph[i].y * std::cosf(0.0F) - Morph[i].z * std::sinf(0.0F) };
			float Rz{ Morph[i].y * std::sinf(0.0F) + Morph[i].z * std::cosf(0.0F) };
			float Rx{ Morph[i].x * std::cosf(DotAngle) + Rz * std::sinf(DotAngle) };
			Rz = -Morph[i].x * std::sinf(DotAngle) + Rz * std::cosf(DotAngle);
			const float Temp{ Rx * std::cosf(DotAngle) - Ry * std::sinf(DotAngle) };
			Ry = Rx * std::sinf(DotAngle) + Ry * std::cosf(DotAngle);
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
		static constexpr float ScaleFactor{ 95.0F };
		static float Angle{};
		static std::int_fast32_t Count1{};
		static std::int_fast32_t Count2{};

		if (Count1 < 256)
		{
			for (std::int_fast32_t i{}; i < NumberOfPoints; ++i)
			{
				Morph[i].x = (Count1 * Sphere[i].x + (256 - Count1) * Torus[i].x) / 256;
				Morph[i].y = (Count1 * Sphere[i].y + (256 - Count1) * Torus[i].y) / 256;
				Morph[i].z = (Count1 * Sphere[i].z + (256 - Count1) * Torus[i].z) / 256;
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
				Morph[i].x = (Count2 * Torus[i].x + (256 - Count2) * Sphere[i].x) / 256;
				Morph[i].y = (Count2 * Torus[i].y + (256 - Count2) * Sphere[i].y) / 256;
				Morph[i].z = (Count2 * Torus[i].z + (256 - Count2) * Sphere[i].z) / 256;
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

		lwmf::RenderText(ScreenTexture, "Realtime morph - 10.000 dots", 10, 10, 0xFFFFFFFF);
	}


} // namespace Morph
