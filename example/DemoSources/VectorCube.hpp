#pragma once

#include <cstdint>
#include <vector>
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

		struct CubeFaceStruct final
		{
			std::int_fast32_t p0{};
			std::int_fast32_t p1{};
			std::int_fast32_t p2{};
			std::int_fast32_t p3{};
		};

		static constexpr std::int_fast32_t CubeNumFaces{ 6 };
		static constexpr std::int_fast32_t CubeNumPoints{ 8 };
		static std::vector<VertexStruct> CubeDef{ { -200.0F, -200.0F, -200.0F }, { -200.0F, -200.0F, 200.0F }, { -200.0F, 200.0F, -200.0F }, { -200.0F, 200.0F, 200.0F }, { 200.0F, -200.0F, -200.0F }, { 200.0F, -200.0F, 200.0F }, { 200.0F, 200.0F, -200.0F }, { 200.0F, 200.0F, 200.0F } };
		static const std::vector<CubeFaceStruct> CubeFaces{ {0,1,3,2}, {4,0,2,6}, {5,4,6,7}, {1,5,7,3}, {0,1,5,4}, {2,3,7,6} };
		static const std::vector<std::int_fast32_t> CubeFacesColors{ lwmf::RGBAtoINT(185, 242, 145, 255), lwmf::RGBAtoINT(80, 191, 148, 255), lwmf::RGBAtoINT(94, 89, 89, 255),	lwmf::RGBAtoINT(247, 35, 73, 255), lwmf::RGBAtoINT(255, 132, 94,255), lwmf::RGBAtoINT(246, 220, 133, 255) };
		static std::vector<lwmf::IntPointStruct> Cube(CubeNumPoints);
		static std::vector<lwmf::IntPointStruct> Points(4);
		static std::vector<std::int_fast32_t> Order(CubeNumFaces);
		static std::vector<float> AverageZ(CubeNumFaces);
		static const float CosA{ std::cosf(0.03F) };
		static const float SinA{ std::sinf(0.03F) };

		lwmf::ClearTexture(ScreenTexture, 0);

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

		// Sort faces
		for (std::int_fast32_t i{}; i < CubeNumFaces; ++i)
		{
			AverageZ[i] = (CubeDef[CubeFaces[i].p0].z + CubeDef[CubeFaces[i].p1].z + CubeDef[CubeFaces[i].p2].z + CubeDef[CubeFaces[i].p3].z) * 0.25F;
			Order[i] = i;
		}

		for (std::int_fast32_t i{}; i < CubeNumFaces - 1; ++i)
		{
			std::int_fast32_t Min{ i };

			for (std::int_fast32_t j{ i + 1 }; j < CubeNumFaces; ++j)
			{
				if (AverageZ[j] < AverageZ[Min])
				{
					Min = j;
				}
			}

			std::swap(AverageZ[Min], AverageZ[i]);
			std::swap(Order[Min], Order[i]);
		}

		for (std::int_fast32_t i{}; i < CubeNumFaces; ++i)
		{
			Points[0] = { Cube[CubeFaces[Order[i]].p0].X, Cube[CubeFaces[Order[i]].p0].Y };
			Points[1] = { Cube[CubeFaces[Order[i]].p1].X, Cube[CubeFaces[Order[i]].p1].Y };
			Points[2] = { Cube[CubeFaces[Order[i]].p2].X, Cube[CubeFaces[Order[i]].p2].Y };
			Points[3] = { Cube[CubeFaces[Order[i]].p3].X, Cube[CubeFaces[Order[i]].p3].Y };

			lwmf::FilledPolygon(ScreenTexture, Points, CubeFacesColors[Order[i]], CubeFacesColors[Order[i]]);
		}
	}


} // namespace VectorCube