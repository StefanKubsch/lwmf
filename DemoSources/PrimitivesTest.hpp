#pragma once

#include <cstdint>
#include <vector>
#include <random>

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

		const std::int_fast32_t OutColor{ Color(Engine) };

		switch (Choice(Engine))
		{
			case 0:
			{
				lwmf::FilledRectangle(ScreenTexture, PointX(Engine), PointY(Engine), Width(Engine), Width(Engine), OutColor);
				++RectangleCounter;
				break;
			}
			case 1:
			{
				lwmf::FilledCircle(ScreenTexture, PointX(Engine), PointY(Engine), Width(Engine), OutColor, OutColor);
				++CircleCounter;
				break;
			}
			case 2:
			{
				std::vector<lwmf::IntPointStruct> Polygon{ {PointX(Engine), PointY(Engine)}, {PointX(Engine), PointY(Engine) }, {PointX(Engine), PointY(Engine) } };
				lwmf::Polygon(ScreenTexture, Polygon, OutColor);
				++PolygonCounter;
				break;
			}
			default: {}
		}

		lwmf::FilledRectangle(ScreenTexture, 0, 0, ScreenTexture.Width, 60, 0);
		lwmf::RenderText(ScreenTexture, "Primitives test", 10, 10, 0xFFFFFFFF);
		lwmf::RenderText(ScreenTexture, "Number of rectangles: " + std::to_string(RectangleCounter), 10, 30, 0xFFFFFFFF);
		lwmf::RenderText(ScreenTexture, "Number of circles   : " + std::to_string(CircleCounter), 10, 40, 0xFFFFFFFF);
		lwmf::RenderText(ScreenTexture, "Number of polygons  : " + std::to_string(PolygonCounter), 10, 50, 0xFFFFFFFF);
	}


} // namespace PrimitivesTest
