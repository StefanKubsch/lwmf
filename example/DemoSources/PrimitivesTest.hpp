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
		static const std::uniform_int_distribution<std::int_fast32_t> Point(-255, ScreenTexture.Width + 255);
		static const std::uniform_int_distribution<std::int_fast32_t> Choice(0, 3);
		static std::array<char, 20> RectangleCounterString{};
		static std::array<char, 20> CircleCounterString{};
		static std::array<char, 20> PolygonCounterString{};
		static std::array<char, 20> EllipseCounterString{};

		switch (Choice(Engine))
		{
			case 0:
			{
				static const std::uniform_int_distribution<std::int_fast32_t> Width(1, 500);
				lwmf::FilledRectangle(ScreenTexture, Point(Engine), Point(Engine), Width(Engine), Width(Engine), lwmf::XorShift32(), lwmf::XorShift32());

				static std::uint_fast64_t RectangleCounter{};
				std::to_chars(RectangleCounterString.data(), RectangleCounterString.data() + RectangleCounterString.size(), ++RectangleCounter);
				break;
			}
			case 1:
			{
				static const std::uniform_int_distribution<std::int_fast32_t> Width(1, 500);
				lwmf::FilledCircle(ScreenTexture, Point(Engine), Point(Engine), Width(Engine), lwmf::XorShift32(), lwmf::XorShift32());

				static std::uint_fast64_t CircleCounter{};
				std::to_chars(CircleCounterString.data(), CircleCounterString.data() + CircleCounterString.size(), ++CircleCounter);
				break;
			}
			case 2:
			{
				const std::vector<lwmf::IntPointStruct> Polygon{ {Point(Engine), Point(Engine)}, {Point(Engine), Point(Engine) }, {Point(Engine), Point(Engine) } };
				lwmf::Polygon(ScreenTexture, Polygon, lwmf::XorShift32());

				static std::uint_fast64_t PolygonCounter{};
				std::to_chars(PolygonCounterString.data(), PolygonCounterString.data() + PolygonCounterString.size(), ++PolygonCounter);
				break;
			}
			case 3:
			{
				static const std::uniform_int_distribution<std::int_fast32_t> Radius(1, 200);
				lwmf::Ellipse(ScreenTexture, Point(Engine), Point(Engine), Radius(Engine), Radius(Engine), lwmf::XorShift32());

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
