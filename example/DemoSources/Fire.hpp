#pragma once

#include <cstdint>
#include <cstdlib>
#include <random>

namespace Fire
{


	inline void Draw()
	{
		// Draw cube
		constexpr std::int_fast32_t Size{ 350 };
		constexpr std::int_fast32_t Half{ Size >> 1 };
		const std::int_fast32_t Red{ lwmf::RGBAtoINT(255, 0, 0, 255) };
		const std::int_fast32_t Yellow{ lwmf::RGBAtoINT(255, 255, 0, 255) };
		const lwmf::IntPointStruct Pos{ Canvas.WidthMid - Size + (Half >> 1), Canvas.HeightMid - (Half >> 1) + 80 };

		lwmf::Line(Canvas, Pos.X, Pos.Y, Pos.X + Half, Pos.Y - Half, Red);
		lwmf::Line(Canvas, Pos.X + 1, Pos.Y, Pos.X + Half, Pos.Y - Half + 1, Yellow);
		lwmf::Line(Canvas, Pos.X + 2, Pos.Y, Pos.X + Half, Pos.Y - Half + 2, Red);

		lwmf::Line(Canvas, Pos.X, Pos.Y + Size - 1, Pos.X + Half, Pos.Y + Half, Red);
		lwmf::Line(Canvas, Pos.X + 1, Pos.Y + Size - 1, Pos.X + Half + 1, Pos.Y + Half, Yellow);
		lwmf::Line(Canvas, Pos.X + 2, Pos.Y + Size - 1, Pos.X + Half + 2, Pos.Y + Half, Red);

		lwmf::Line(Canvas, Pos.X + Size, Pos.Y, Pos.X + Size + Half, Pos.Y - Half, Red);
		lwmf::Line(Canvas, Pos.X + Size - 1, Pos.Y, Pos.X + Size + Half - 1, Pos.Y - Half, Yellow);
		lwmf::Line(Canvas, Pos.X + Size - 2, Pos.Y, Pos.X + Size + Half - 2, Pos.Y - Half, Red);

		lwmf::Line(Canvas, Pos.X + Size - 1, Pos.Y + Size - 1, Pos.X + Size + Half - 1, Pos.Y + Half - 1, Red);
		lwmf::Line(Canvas, Pos.X + Size - 2, Pos.Y + Size - 1, Pos.X + Size + Half - 2, Pos.Y + Half - 1, Yellow);
		lwmf::Line(Canvas, Pos.X + Size - 3, Pos.Y + Size - 1, Pos.X + Size + Half - 3, Pos.Y + Half - 1, Red);

		lwmf::Rectangle(Canvas, Pos.X + Half, Pos.Y - Half, Size, Size, Red);
		lwmf::Rectangle(Canvas, Pos.X + Half + 1, Pos.Y - Half + 1, Size - 2, Size - 2, Yellow);
		lwmf::Rectangle(Canvas, Pos.X + Half + 2, Pos.Y - Half + 2, Size - 4, Size - 4, Red);

		lwmf::Rectangle(Canvas, Pos.X, Pos.Y, Size, Size, Red);
		lwmf::Rectangle(Canvas, Pos.X + 1, Pos.Y + 1, Size - 2, Size - 2, Yellow);
		lwmf::Rectangle(Canvas, Pos.X + 2, Pos.Y + 2, Size - 4, Size - 4, Red);

		// Apply fire
		std::uniform_int_distribution<std::int_fast32_t> Distrib1(-255, 255);

		#pragma omp parallel for
		for (std::int_fast32_t y{ 1 }; y < Canvas.Height; ++y)
		{
			for (std::int_fast32_t x{ 1 }; x < Canvas.Width - 1; ++x)
			{
				if ((Distrib1(RNG) & 31) == 0)
				{
					lwmf::SetPixel(Canvas, x, y, 0x00000000);
				}
				else if (lwmf::GetPixel(Canvas, x, y) != 0)
				{
					const lwmf::ColorStructRGBA Point1{ lwmf::INTtoRGBA(lwmf::GetPixel(Canvas,x, y)) };
					const lwmf::ColorStructRGBA Point2{ lwmf::INTtoRGBA(lwmf::GetPixel(Canvas,x, y - 1)) };
					const lwmf::ColorStructRGBA Point3{ lwmf::INTtoRGBA(lwmf::GetPixel(Canvas,x - 1, y)) };
					const lwmf::ColorStructRGBA Point4{ lwmf::INTtoRGBA(lwmf::GetPixel(Canvas,x + 1, y - 1)) };

					lwmf::SetPixel(Canvas, x, y - 1, lwmf::RGBAtoINT((Point1.Red + Point2.Red + Point3.Red + Point4.Red) >> 2,
						(Point1.Green + Point2.Green + Point3.Green + Point4.Green) >> 2,
						(Point1.Blue + Point2.Blue + Point3.Blue + Point4.Blue) >> 2, 255));
				}
			}
		}

		DisplayInfoBox("OpenMP accelerated fullscreen fire");
	}


} // namespace Fire