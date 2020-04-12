#pragma once

#include <cstdint>
#include <vector>
#include <algorithm>
#include <array>
#include <cmath>
#include <utility>

namespace VectorCube
{


	inline void Draw()
	{
		struct VertexStruct final
		{
			float x{};
			float y{};
			float z{};
		};

		constexpr std::int_fast32_t CubeNumPoints{ 8 };
		static std::array<VertexStruct, 8> CubeDef{ { { -200.0F, -200.0F, -200.0F }, { -200.0F, -200.0F, 200.0F }, { -200.0F, 200.0F, -200.0F }, { -200.0F, 200.0F, 200.0F }, { 200.0F, -200.0F, -200.0F }, { 200.0F, -200.0F, 200.0F }, { 200.0F, 200.0F, -200.0F }, { 200.0F, 200.0F, 200.0F } } };
		std::array<lwmf::IntPointStruct, CubeNumPoints> Cube{};
		const float CosA{ std::cosf(0.03F) };
		const float SinA{ std::sinf(0.03F) };

		lwmf::ClearTexture(ScreenTexture, 0x00000000);

		for (std::int_fast32_t i{}; i < CubeNumPoints; ++i)
		{
			// x - rotation
			const float y{ CubeDef[i].y };
			CubeDef[i].y = y * CosA - CubeDef[i].z * SinA;

			// y - rotation
			const float z{ CubeDef[i].z * CosA + y * SinA };
			CubeDef[i].z = z * CosA + CubeDef[i].x * SinA;

			// z - rotation
			const float x{ CubeDef[i].x * CosA - z * SinA };
			CubeDef[i].x = x * CosA - CubeDef[i].y * SinA;
			CubeDef[i].y = CubeDef[i].y * CosA + x * SinA;

			// 2D projection & translate
			Cube[i].X = ScreenTexture.WidthMid + static_cast<std::int_fast32_t>(CubeDef[i].x);
			Cube[i].Y = ScreenTexture.HeightMid + static_cast<std::int_fast32_t>(CubeDef[i].y);
		}

		struct CubeFaceStruct final
		{
			std::int_fast32_t p0{};
			std::int_fast32_t p1{};
			std::int_fast32_t p2{};
			std::int_fast32_t p3{};
		};

		constexpr std::int_fast32_t CubeNumFaces{ 6 };
		constexpr std::array<CubeFaceStruct, 6> CubeFaces{ { {0,1,3,2}, {4,0,2,6}, {5,4,6,7}, {1,5,7,3}, {0,1,5,4}, {2,3,7,6} } };
		std::vector<std::pair<std::int_fast32_t, float>> Order(6);

		// Sort faces
		for (std::int_fast32_t i{}; i < CubeNumFaces; ++i)
		{
			Order[i].second = (CubeDef[CubeFaces[i].p0].z + CubeDef[CubeFaces[i].p1].z + CubeDef[CubeFaces[i].p2].z + CubeDef[CubeFaces[i].p3].z) * 0.25F;
			Order[i].first = i;
		}

		std::sort(Order.begin(), Order.end(), [](auto& left, auto& right) {	return left.second < right.second; });

		std::vector<lwmf::IntPointStruct> Points(4);
		const std::array<std::int_fast32_t, 6> CubeFacesColors{ lwmf::RGBAtoINT(185, 242, 145, 255), lwmf::RGBAtoINT(80, 191, 148, 255), lwmf::RGBAtoINT(94, 89, 89, 255),	lwmf::RGBAtoINT(247, 35, 73, 255), lwmf::RGBAtoINT(255, 132, 94,255), lwmf::RGBAtoINT(246, 220, 133, 255) };

		for (std::int_fast32_t i{}; i < CubeNumFaces; ++i)
		{
			Points =
			{
				{ Cube[CubeFaces[Order[i].first].p0].X, Cube[CubeFaces[Order[i].first].p0].Y },
				{ Cube[CubeFaces[Order[i].first].p1].X, Cube[CubeFaces[Order[i].first].p1].Y },
				{ Cube[CubeFaces[Order[i].first].p2].X, Cube[CubeFaces[Order[i].first].p2].Y },
				{ Cube[CubeFaces[Order[i].first].p3].X, Cube[CubeFaces[Order[i].first].p3].Y }
			};

			lwmf::FilledPolygon(ScreenTexture, Points, CubeFacesColors[Order[i].first], CubeFacesColors[Order[i].first]);
		}
	}


} // namespace VectorCube