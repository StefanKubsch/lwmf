#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <random>
#include <charconv>

namespace PrimitivesTest
{


	void Draw()
	{
		const std::uniform_int_distribution<std::int_fast32_t> PointX(-255, ScreenTexture.Width + 255);
		const std::uniform_int_distribution<std::int_fast32_t> PointY(-255, ScreenTexture.Height + 255);
		const std::uniform_int_distribution<std::int_fast32_t> Width(0, 500);
		const std::uniform_int_distribution<std::int_fast32_t> RadiusX(0, 200);
		const std::uniform_int_distribution<std::int_fast32_t> RadiusY(0, 200);
		const std::uniform_int_distribution<std::int_fast32_t> Color(0x00000000, 0xFFFFFFFF);
		const std::uniform_int_distribution<std::int_fast32_t> Choice(0, 3);
		static std::int_fast64_t RectangleCounter{};
		static std::int_fast64_t CircleCounter{};
		static std::int_fast64_t PolygonCounter{};
		static std::int_fast64_t EllipseCounter{};
		static std::array<char, 20> RectangleCounterString{};
		static std::array<char, 20> CircleCounterString{};
		static std::array<char, 20> PolygonCounterString{};
		static std::array<char, 20> EllipseCounterString{};
		const std::int_fast32_t BorderColor{ Color(Engine) };
		const std::int_fast32_t FillColor{ Color(Engine) };

		switch (Choice(Engine))
		{
			case 0:
			{
				lwmf::FilledRectangle(ScreenTexture, PointX(Engine), PointY(Engine), Width(Engine), Width(Engine), BorderColor, FillColor);
				std::to_chars(RectangleCounterString.data(), RectangleCounterString.data() + RectangleCounterString.size(), ++RectangleCounter);
				break;
			}
			case 1:
			{
				lwmf::FilledCircle(ScreenTexture, PointX(Engine), PointY(Engine), Width(Engine), BorderColor, FillColor);
				std::to_chars(CircleCounterString.data(), CircleCounterString.data() + CircleCounterString.size(), ++CircleCounter);
				break;
			}
			case 2:
			{
				const std::vector<lwmf::IntPointStruct> Polygon{ {PointX(Engine), PointY(Engine)}, {PointX(Engine), PointY(Engine) }, {PointX(Engine), PointY(Engine) } };
				lwmf::Polygon(ScreenTexture, Polygon, BorderColor);
				std::to_chars(PolygonCounterString.data(), PolygonCounterString.data() + PolygonCounterString.size(), ++PolygonCounter);
				break;
			}
			case 3:
			{
				lwmf::Ellipse(ScreenTexture, PointX(Engine), PointY(Engine), RadiusX(Engine), RadiusY(Engine), FillColor);
				std::to_chars(EllipseCounterString.data(), EllipseCounterString.data() + EllipseCounterString.size(), ++EllipseCounter);
				break;
			}
			default: {}
		}

		lwmf::FilledRectangle(ScreenTexture, 0, 65, ScreenTexture.Width, 50, 0, 0);
		lwmf::RenderText(ScreenTexture, "Number of rectangles: " + std::string(RectangleCounterString.data()), 10, 70, 0xFFFFFFFF);
		lwmf::RenderText(ScreenTexture, "Number of circles   : " + std::string(CircleCounterString.data()), 10, 80, 0xFFFFFFFF);
		lwmf::RenderText(ScreenTexture, "Number of polygons  : " + std::string(PolygonCounterString.data()), 10, 90, 0xFFFFFFFF);
		lwmf::RenderText(ScreenTexture, "Number of ellipses  : " + std::string(EllipseCounterString.data()), 10, 100, 0xFFFFFFFF);
	}


} // namespace PrimitivesTest
