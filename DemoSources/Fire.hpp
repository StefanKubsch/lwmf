#pragma once

#include <cstdint>
#include <cstdlib>
#include <random>

namespace Fire
{


	inline void Draw()
	{
		static constexpr lwmf::IntPointStruct CubePos{ 200, 250 };
		static constexpr std::int_fast32_t CubeSize{ 250 };
		static constexpr std::int_fast32_t CubeHalf{ CubeSize >> 1 };
		static const std::int_fast32_t CubeRed{ lwmf::RGBAtoINT(255, 0, 0, 255) };
		static const std::int_fast32_t CubeYellow{ lwmf::RGBAtoINT(255, 255, 0, 255) };

		static std::mt19937_64 Engine;
		static const std::uniform_int_distribution<std::int_fast32_t> Distrib1(-128, 128);

		// Draw cube
		lwmf::Line(CubePos.x, CubePos.y, CubePos.x + CubeHalf, CubePos.y - CubeHalf, CubeRed);
		lwmf::Line(CubePos.x + 1, CubePos.y, CubePos.x + CubeHalf, CubePos.y - CubeHalf + 1, CubeYellow);
		lwmf::Line(CubePos.x + 2, CubePos.y, CubePos.x + CubeHalf, CubePos.y - CubeHalf + 2, CubeRed);

		lwmf::Line(CubePos.x, CubePos.y + CubeSize, CubePos.x + CubeHalf, CubePos.y + CubeHalf, CubeRed);
		lwmf::Line(CubePos.x + 1, CubePos.y + CubeSize, CubePos.x + CubeHalf + 1, CubePos.y + CubeHalf, CubeYellow);
		lwmf::Line(CubePos.x + 2, CubePos.y + CubeSize, CubePos.x + CubeHalf + 2, CubePos.y + CubeHalf, CubeRed);

		lwmf::Line(CubePos.x + CubeSize, CubePos.y, CubePos.x + CubeSize + CubeHalf, CubePos.y - CubeHalf, CubeRed);
		lwmf::Line(CubePos.x + CubeSize - 1, CubePos.y, CubePos.x + CubeSize + CubeHalf - 1, CubePos.y - CubeHalf, CubeYellow);
		lwmf::Line(CubePos.x + CubeSize - 2, CubePos.y, CubePos.x + CubeSize + CubeHalf - 2, CubePos.y - CubeHalf, CubeRed);

		lwmf::Line(CubePos.x + CubeSize, CubePos.y + CubeSize, CubePos.x + CubeSize + CubeHalf, CubePos.y + CubeHalf, CubeRed);
		lwmf::Line(CubePos.x + CubeSize - 1, CubePos.y + CubeSize, CubePos.x + CubeSize + CubeHalf - 1, CubePos.y + CubeHalf, CubeYellow);
		lwmf::Line(CubePos.x + CubeSize - 2, CubePos.y + CubeSize, CubePos.x + CubeSize + CubeHalf - 2, CubePos.y + CubeHalf, CubeRed);

		lwmf::Rectangle(CubePos.x + CubeHalf, CubePos.y - CubeHalf, CubeSize, CubeSize, CubeRed);
		lwmf::Rectangle(CubePos.x + CubeHalf + 1, CubePos.y - CubeHalf + 1, CubeSize - 2, CubeSize - 2, CubeYellow);
		lwmf::Rectangle(CubePos.x + CubeHalf + 2, CubePos.y - CubeHalf + 2, CubeSize - 4, CubeSize - 4, CubeRed);

		lwmf::Rectangle(CubePos.x, CubePos.y, CubeSize, CubeSize, CubeRed);
		lwmf::Rectangle(CubePos.x + 1, CubePos.y + 1, CubeSize - 2, CubeSize - 2, CubeYellow);
		lwmf::Rectangle(CubePos.x + 2, CubePos.y + 2, CubeSize - 4, CubeSize - 4, CubeRed);

		// Apply fire
		for (std::int_fast32_t y{ 1 }; y < lwmf::ViewportHeight; ++y)
		{
			for (std::int_fast32_t x{}; x < lwmf::ViewportWidth; ++x)
			{
				if ((Distrib1(Engine) & 31) == 0)
				{
					lwmf::SetPixel(x, y, 0);
				}
				else
				{
					const lwmf::ColorStruct Point1{ lwmf::INTtoRGBA(lwmf::GetPixel(x, y)) };
					const lwmf::ColorStruct Point2{ lwmf::INTtoRGBA(lwmf::GetPixel(x, y - 1)) };
					const lwmf::ColorStruct Point3{ lwmf::INTtoRGBA(lwmf::GetPixel(x - 1, y)) };
					const lwmf::ColorStruct Point4{ lwmf::INTtoRGBA(lwmf::GetPixel(x + 1, y - 1)) };

					lwmf::SetPixel(x, y - 1, lwmf::RGBAtoINT((Point1.Red + Point2.Red + Point3.Red + Point4.Red) >> 2,
						(Point1.Green + Point2.Green + Point3.Green + Point4.Green) >> 2,
						(Point1.Blue + Point2.Blue + Point3.Blue + Point4.Blue) >> 2, 255));
				}
			}
		}

		lwmf::FilledRectangle(0, 0, 210, 40, 0);
		lwmf::RenderText("Realtime fullscreen fire", 10, 10, 0xFFFFFFFF);
	}


} // namespace Fire