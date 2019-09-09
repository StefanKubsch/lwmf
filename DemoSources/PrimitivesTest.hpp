#pragma once

#include <cstdint>
#include <vector>
#include <random>
#include <charconv>

namespace PrimitivesTest
{


	void Draw()
	{
		static const std::uniform_int_distribution<std::int_fast32_t> PointX(-255, ScreenTexture.Width + 255);
		static const std::uniform_int_distribution<std::int_fast32_t> PointY(-255, ScreenTexture.Height + 255);
		static const std::uniform_int_distribution<std::int_fast32_t> Width(0, 500);
		static const std::uniform_int_distribution<std::int_fast32_t> Color(0x00000000, 0xFFFFFFFF);
		static const std::uniform_int_distribution<std::int_fast32_t> Choice(0, 2);
		static std::int_fast64_t RectangleCounter{};
		static std::int_fast64_t CircleCounter{};
		static std::int_fast64_t PolygonCounter{};
		static std::vector<char> RectangleCounterString(20);
		static std::vector<char> CircleCounterString(20);
		static std::vector<char> PolygonCounterString(20);
		const std::int_fast32_t OutColor{ Color(Engine) };

		switch (Choice(Engine))
		{
			case 0:
			{
				lwmf::FilledRectangle(ScreenTexture, PointX(Engine), PointY(Engine), Width(Engine), Width(Engine), OutColor);
				std::to_chars(RectangleCounterString.data(), RectangleCounterString.data() + RectangleCounterString.size(), ++RectangleCounter);
				break;
			}
			case 1:
			{
				lwmf::FilledCircle(ScreenTexture, PointX(Engine), PointY(Engine), Width(Engine), OutColor, OutColor);
				std::to_chars(CircleCounterString.data(), CircleCounterString.data() + CircleCounterString.size(), ++CircleCounter);
				break;
			}
			case 2:
			{
				std::vector<lwmf::IntPointStruct> Polygon{ {PointX(Engine), PointY(Engine)}, {PointX(Engine), PointY(Engine) }, {PointX(Engine), PointY(Engine) } };
				lwmf::Polygon(ScreenTexture, Polygon, OutColor);
				std::to_chars(PolygonCounterString.data(), PolygonCounterString.data() + PolygonCounterString.size(), ++PolygonCounter);
				break;
			}
			default: {}
		}

		lwmf::FilledRectangle(ScreenTexture, 0, 0, ScreenTexture.Width - 1, 65, 0);
		lwmf::RenderText(ScreenTexture, "Primitives test", 10, 10, 0xFFFFFFFF);
		lwmf::RenderText(ScreenTexture, "Number of rectangles: " + std::string(RectangleCounterString.data()), 10, 30, 0xFFFFFFFF);
		lwmf::RenderText(ScreenTexture, "Number of circles   : " + std::string(CircleCounterString.data()), 10, 40, 0xFFFFFFFF);
		lwmf::RenderText(ScreenTexture, "Number of polygons  : " + std::string(PolygonCounterString.data()), 10, 50, 0xFFFFFFFF);
	}


} // namespace PrimitivesTest
