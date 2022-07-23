#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <random>
#include <charconv>

namespace PrimitivesTest
{


	inline void Draw()
	{
		const std::uniform_int_distribution<std::int_fast32_t> Point(-255, ScreenTexture.Width + 255);
		const std::uniform_int_distribution<std::int_fast32_t> Choice(0, 3);

		static std::array<char, 10> RectangleCounterString{};
		static std::array<char, 10> CircleCounterString{};
		static std::array<char, 10> PolygonCounterString{};
		static std::array<char, 10> EllipseCounterString{};

		switch (Choice(RNG))
		{
			case 0:
			{
				const std::uniform_int_distribution<std::int_fast32_t> Width(0, 500);
				lwmf::FilledRectangle(ScreenTexture, Point(RNG), Point(RNG), Width(RNG), Width(RNG), lwmf::XorShift32(), lwmf::XorShift32());

				static std::uint_fast32_t RectangleCounter{};
				std::to_chars(RectangleCounterString.data(), RectangleCounterString.data() + RectangleCounterString.size(), ++RectangleCounter);
				break;
			}
			case 1:
			{
				const std::uniform_int_distribution<std::int_fast32_t> Width(0, 500);
				lwmf::FilledCircle(ScreenTexture, Point(RNG), Point(RNG), Width(RNG), lwmf::XorShift32(), lwmf::XorShift32());

				static std::uint_fast32_t CircleCounter{};
				std::to_chars(CircleCounterString.data(), CircleCounterString.data() + CircleCounterString.size(), ++CircleCounter);
				break;
			}
			case 2:
			{
				const std::uniform_int_distribution<std::int_fast32_t> PolygonPointX(0, ScreenTexture.Width);
				const std::uniform_int_distribution<std::int_fast32_t> PolygonPointY(0, ScreenTexture.Height);

				const std::vector<lwmf::IntPointStruct> Polygon{ {PolygonPointX(RNG), PolygonPointY(RNG)}, {PolygonPointX(RNG), PolygonPointY(RNG) }, {PolygonPointX(RNG), PolygonPointY(RNG) } };
				lwmf::Polygon(ScreenTexture, Polygon, lwmf::XorShift32());

				static std::uint_fast32_t PolygonCounter{};
				std::to_chars(PolygonCounterString.data(), PolygonCounterString.data() + PolygonCounterString.size(), ++PolygonCounter);
				break;
			}
			case 3:
			{
				const std::uniform_int_distribution<std::int_fast32_t> Radius(0, 200);
				lwmf::Ellipse(ScreenTexture, Point(RNG), Point(RNG), Radius(RNG), Radius(RNG), lwmf::XorShift32());

				static std::uint_fast64_t EllipseCounter{};
				std::to_chars(EllipseCounterString.data(), EllipseCounterString.data() + EllipseCounterString.size(), ++EllipseCounter);
				break;
			}
			default: {}
		}

		DisplayInfoBox("Primitives test");

		lwmf::RenderText(ScreenTexture, "Number of rectangles: " + std::string(RectangleCounterString.data()), 10, 70, 0xFFFFFFFF);
		lwmf::RenderText(ScreenTexture, "Number of circles   : " + std::string(CircleCounterString.data()), 10, 80, 0xFFFFFFFF);
		lwmf::RenderText(ScreenTexture, "Number of polygons  : " + std::string(PolygonCounterString.data()), 10, 90, 0xFFFFFFFF);
		lwmf::RenderText(ScreenTexture, "Number of ellipses  : " + std::string(EllipseCounterString.data()), 10, 100, 0xFFFFFFFF);
	}


} // namespace PrimitivesTest
