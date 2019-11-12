#pragma once

#include <cstdint>
#include <cstdlib>
#include <random>

namespace Fire
{


	inline void Draw()
	{
		constexpr std::int_fast32_t CubeSize{ 250 };
		constexpr std::int_fast32_t CubeHalf{ CubeSize >> 1 };
		const std::int_fast32_t CubeRed{ lwmf::RGBAtoINT(255, 0, 0, 255) };
		const std::int_fast32_t CubeYellow{ lwmf::RGBAtoINT(255, 255, 0, 255) };
		const lwmf::IntPointStruct CubePos{ ScreenTexture.WidthMid - CubeSize + (CubeHalf >> 1), ScreenTexture.HeightMid - (CubeHalf >> 1) };
		const std::uniform_int_distribution<std::int_fast32_t> Distrib1(-128, 128);

		// Draw cube
		lwmf::Line(ScreenTexture, CubePos.X, CubePos.Y, CubePos.X + CubeHalf, CubePos.Y - CubeHalf, CubeRed);
		lwmf::Line(ScreenTexture, CubePos.X + 1, CubePos.Y, CubePos.X + CubeHalf, CubePos.Y - CubeHalf + 1, CubeYellow);
		lwmf::Line(ScreenTexture, CubePos.X + 2, CubePos.Y, CubePos.X + CubeHalf, CubePos.Y - CubeHalf + 2, CubeRed);

		lwmf::Line(ScreenTexture, CubePos.X, CubePos.Y + CubeSize, CubePos.X + CubeHalf, CubePos.Y + CubeHalf, CubeRed);
		lwmf::Line(ScreenTexture, CubePos.X + 1, CubePos.Y + CubeSize, CubePos.X + CubeHalf + 1, CubePos.Y + CubeHalf, CubeYellow);
		lwmf::Line(ScreenTexture, CubePos.X + 2, CubePos.Y + CubeSize, CubePos.X + CubeHalf + 2, CubePos.Y + CubeHalf, CubeRed);

		lwmf::Line(ScreenTexture, CubePos.X + CubeSize, CubePos.Y, CubePos.X + CubeSize + CubeHalf, CubePos.Y - CubeHalf, CubeRed);
		lwmf::Line(ScreenTexture, CubePos.X + CubeSize - 1, CubePos.Y, CubePos.X + CubeSize + CubeHalf - 1, CubePos.Y - CubeHalf, CubeYellow);
		lwmf::Line(ScreenTexture, CubePos.X + CubeSize - 2, CubePos.Y, CubePos.X + CubeSize + CubeHalf - 2, CubePos.Y - CubeHalf, CubeRed);

		lwmf::Line(ScreenTexture, CubePos.X + CubeSize, CubePos.Y + CubeSize, CubePos.X + CubeSize + CubeHalf, CubePos.Y + CubeHalf, CubeRed);
		lwmf::Line(ScreenTexture, CubePos.X + CubeSize - 1, CubePos.Y + CubeSize, CubePos.X + CubeSize + CubeHalf - 1, CubePos.Y + CubeHalf, CubeYellow);
		lwmf::Line(ScreenTexture, CubePos.X + CubeSize - 2, CubePos.Y + CubeSize, CubePos.X + CubeSize + CubeHalf - 2, CubePos.Y + CubeHalf, CubeRed);

		lwmf::Rectangle(ScreenTexture, CubePos.X + CubeHalf, CubePos.Y - CubeHalf, CubeSize, CubeSize, CubeRed);
		lwmf::Rectangle(ScreenTexture, CubePos.X + CubeHalf + 1, CubePos.Y - CubeHalf + 1, CubeSize - 2, CubeSize - 2, CubeYellow);
		lwmf::Rectangle(ScreenTexture, CubePos.X + CubeHalf + 2, CubePos.Y - CubeHalf + 2, CubeSize - 4, CubeSize - 4, CubeRed);

		lwmf::Rectangle(ScreenTexture, CubePos.X, CubePos.Y, CubeSize, CubeSize, CubeRed);
		lwmf::Rectangle(ScreenTexture, CubePos.X + 1, CubePos.Y + 1, CubeSize - 2, CubeSize - 2, CubeYellow);
		lwmf::Rectangle(ScreenTexture, CubePos.X + 2, CubePos.Y + 2, CubeSize - 4, CubeSize - 4, CubeRed);

		// Apply fire
		#pragma omp parallel for
		for (std::int_fast32_t y{ 1 }; y < ScreenTexture.Height; ++y)
		{
			const std::int_fast32_t TempY{ (y - 1) * ScreenTexture.Width };
			const std::int_fast32_t TempY1{ y * ScreenTexture.Width };

			for (std::int_fast32_t x{}; x < ScreenTexture.Width; ++x)
			{
				if ((Distrib1(Engine) & 31) == 0)
				{
					ScreenTexture.Pixels[TempY1 + x] = 0;
				}
				else if (lwmf::GetPixel(ScreenTexture, x, y) != 0)
				{
					const lwmf::ColorStruct Point1{ lwmf::INTtoRGBA(lwmf::GetPixel(ScreenTexture,x, y)) };
					const lwmf::ColorStruct Point2{ lwmf::INTtoRGBA(lwmf::GetPixel(ScreenTexture,x, y - 1)) };
					const lwmf::ColorStruct Point3{ lwmf::INTtoRGBA(lwmf::GetPixel(ScreenTexture,x - 1, y)) };
					const lwmf::ColorStruct Point4{ lwmf::INTtoRGBA(lwmf::GetPixel(ScreenTexture,x + 1, y - 1)) };

					ScreenTexture.Pixels[TempY + x] = lwmf::RGBAtoINT((Point1.Red + Point2.Red + Point3.Red + Point4.Red) >> 2,
						(Point1.Green + Point2.Green + Point3.Green + Point4.Green) >> 2,
						(Point1.Blue + Point2.Blue + Point3.Blue + Point4.Blue) >> 2, 255);
				}
			}
		}
	}


} // namespace Fire