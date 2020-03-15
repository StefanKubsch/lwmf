#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <cmath>
#include <algorithm>

namespace Cubes
{


	inline constexpr std::int_fast32_t CubeSize{ 12 };

	inline void DrawCube(const std::int_fast32_t cx, const std::int_fast32_t cy, const float Angle, const std::int_fast32_t Color)
	{
		std::array<std::int_fast32_t, 4> PosX;
		std::array<std::int_fast32_t, 4> PosY;
		std::int_fast32_t MinIdx{ -1 };
		std::int_fast32_t Index{};
		float Min{ FLT_MAX };

		for (Index; Index < 4; ++Index)
		{
			const float TempAngle{ Angle + Index * lwmf::HalfPI };
			PosX[Index] = static_cast<std::int_fast32_t>(static_cast<float>(cx) + std::cosf(TempAngle) * CubeSize);
			PosY[Index] = static_cast<std::int_fast32_t>(static_cast<float>(cy) + std::sinf(TempAngle) * CubeSize * 0.5F);

			if (PosY[Index] < Min)
			{
				Min = static_cast<float>(PosY[Index]);
				MinIdx = Index;
			}
		}

		std::vector<lwmf::IntPointStruct> Points(4);
		std::int_fast32_t NextIndex{};

		for (std::int_fast32_t y{ 1 }; y >= 0; --y)
		{
			Points[0] = { PosX[0], PosY[0] + y * CubeSize };

			for (Index = 0; Index < 4; ++Index)
			{
				NextIndex = (Index + 1) & 3;
				Points[Index + 1] = { PosX[NextIndex], PosY[NextIndex] + y * CubeSize };
			}

			lwmf::FilledPolygon(ScreenTexture, Points, 0x44444444, Color);
		}

		Index = (MinIdx + 1) & 3;

		for (std::int_fast32_t j{}; j < 2; ++j)
		{
			NextIndex = (Index + 1) & 3;

			Points[0] = { PosX[Index], PosY[Index] };
			Points[1] = { PosX[Index], PosY[Index] + CubeSize };
			Points[2] = { PosX[NextIndex], PosY[NextIndex] + CubeSize };
			Points[3] = { PosX[NextIndex], PosY[NextIndex] };

			lwmf::FilledPolygon(ScreenTexture, Points, 0x44444444, Color >> 1);

			Index = NextIndex;
		}
	}

	inline void Draw()
	{
		static std::int_fast32_t Steps{};
		Steps = (Steps + 2) % 1570;

		for (std::int_fast32_t i{}; i < ScreenTexture.Width; ++i)
		{
			lwmf::Line(ScreenTexture, i, 0, i, ScreenTexture.Height - 1, lwmf::RGBAtoINT(255 - (i >> 3), 0, i / 6, 255));
		}

		std::int_fast32_t ShiftX{};
		const std::int_fast32_t ScreenWidthMid{ ScreenTexture.Width >> 1 };
		const std::int_fast32_t ScreenHeightMid{ ScreenTexture.Height >> 1 };
		const std::int_fast32_t CubeSizeTemp1{ static_cast<std::int_fast32_t>(static_cast<float>(CubeSize) * 1.8F) };
		const std::int_fast32_t CubeSizeTemp2{ static_cast<std::int_fast32_t>(static_cast<float>(CubeSize) * 2.3F) };
		const std::int_fast32_t MaxHeight{ ScreenTexture.Height - (CubeSize << 1) };
		const std::int_fast32_t MaxWidth{ ScreenTexture.Width - (CubeSize << 1) };

		for (std::int_fast32_t j{ 100 }; j < MaxHeight; j += CubeSizeTemp1)
		{
			++ShiftX;
			const std::int_fast32_t ShiftXTemp{ (ShiftX & 1) * CubeSize };

			for (std::int_fast32_t i{ CubeSize << 1 }; i < MaxWidth; i += CubeSizeTemp2)
			{
				const std::int_fast32_t x{ i + ShiftXTemp };
				const float dx{ static_cast<float>(std::abs(ScreenWidthMid - x)) };
				const float dy{ static_cast<float>(std::abs(ScreenHeightMid - j)) };
				const float Distance{ std::sqrtf(dx * dx + dy * dy) };
				const float StepTemp{ static_cast<float>(Steps) - Distance };
				float Angle{ lwmf::HalfPI };
				std::int_fast32_t ShiftY{};

				if (StepTemp > 0.0F && StepTemp < 314.0F)
				{
					Angle += StepTemp * 0.005F;
					ShiftY = CubeSizeTemp1 * static_cast<std::int_fast32_t>(StepTemp) / 314;
				}
				else if (StepTemp <= 0.0F)
				{
				}
				else
				{
					ShiftY = CubeSizeTemp1;
				}

				DrawCube(x, j - ShiftY, Angle, lwmf::RGBAtoINT(255 - (i >> 3), 0, i / 6, 255));
			}
		}
	}


} // namespace Cubes
