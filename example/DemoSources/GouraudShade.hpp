#pragma once

#include <cstdint>
#include <array>
#include <cmath>

namespace GouraudShade
{


	struct Point3D final
	{
		std::int_fast32_t x{};
		std::int_fast32_t y{};
		std::int_fast32_t z{};
	};

	constexpr std::int_fast32_t ScaleFactor1{ 60 };
	constexpr std::int_fast32_t ScaleFactor2{ 400 };
	constexpr std::int_fast32_t Radius1{ 50 };
	constexpr std::int_fast32_t Radius2{ 100 };
	constexpr std::int_fast32_t NumberOfVertices{ ScaleFactor1 * ScaleFactor2 };
	inline std::array<Point3D, NumberOfVertices> Shape{};
	inline std::array<Point3D, NumberOfVertices> RotatedShape{};
	inline std::array<lwmf::IntPointStruct, NumberOfVertices> Shape2D{};
	inline std::array<std::int_fast32_t, NumberOfVertices> DrawOrder{};
	inline std::array<std::array<std::int_fast32_t, 4>, NumberOfVertices> PolygonCoords{};
	inline std::int_fast32_t Vertices{};
	inline std::int_fast32_t VertexCount{};

	inline void Init()
	{
		float Alpha{};

		for (std::int_fast32_t Count2{}; Count2 < ScaleFactor2; ++Count2, Alpha += lwmf::DoublePI / ScaleFactor2)
		{
			const float TwoAlpha{ 2.0F * Alpha };
			const float ThreeAlpha{ 3.0F * Alpha };
			const float X{ Radius2 * std::cosf(TwoAlpha) + Radius1 * std::sinf(Alpha) };
			const float Y{ Radius2 * std::sinf(TwoAlpha) + Radius1 * std::cosf(Alpha) };
			const float Z{ Radius2 * std::cosf(ThreeAlpha) };
			const float dx{ -2.0F * Radius2 * std::sinf(TwoAlpha) + Radius1 * std::cosf(Alpha) };
			const float dy{ 2.0F * Radius2 * std::cosf(TwoAlpha) - Radius1 * std::sinf(Alpha) };
			const float dz{ -3.0F * Radius2 * std::sinf(ThreeAlpha) };
			const float Value{ std::sqrtf(dx * dx + dz * dz) };
			const float Modulus{ std::sqrtf(dx * dx + dy * dy + dz * dz) };
			float Beta{};

			for (std::int_fast32_t Count1{}; Count1 < ScaleFactor1; ++Count1, Beta += lwmf::DoublePI / ScaleFactor1)
			{
				Shape[Vertices].x = static_cast<std::int_fast32_t>(X - static_cast<float>(Radius1) * (std::cosf(Beta) * dz - std::sinf(Beta) *	dx * dy / Modulus) / Value);
				Shape[Vertices].y = static_cast<std::int_fast32_t>(Y - static_cast<float>(Radius1) * std::sinf(Beta) * Value / Modulus);
				Shape[Vertices].z = static_cast<std::int_fast32_t>(Z + static_cast<float>(Radius1) * (std::cosf(Beta) * dx + std::sinf(Beta) * dy * dz / Modulus) / Value);

				++Vertices;
			}
		}

		for (std::int_fast32_t Count1{}; Count1 < ScaleFactor2; ++Count1)
		{
			const std::int_fast32_t Index1{ Count1 * ScaleFactor1 };
			std::int_fast32_t Index2{ Index1 + ScaleFactor1 };
			std::int_fast32_t Rotation{};
			Index2 %= Vertices;
			float MinDistance{ static_cast<float>((Shape[Index1].x - Shape[Index2].x) * (Shape[Index1].x - Shape[Index2].x) + (Shape[Index1].y - Shape[Index2].y) * (Shape[Index1].y - Shape[Index2].y) + (Shape[Index1].z - Shape[Index2].z) * (Shape[Index1].z - Shape[Index2].z)) };

			for (std::int_fast32_t Count2{ 1 }; Count2 < ScaleFactor1; ++Count2)
			{
				Index2 = (Count1 == ScaleFactor2 - 1) ? Count2 : Count2 + Index1 + ScaleFactor1;
				const float Distance{ static_cast<float>((Shape[Index1].x - Shape[Index2].x) * (Shape[Index1].x - Shape[Index2].x) + (Shape[Index1].y - Shape[Index2].y) * (Shape[Index1].y - Shape[Index2].y) + (Shape[Index1].z - Shape[Index2].z) * (Shape[Index1].z - Shape[Index2].z)) };

				if (Distance < MinDistance)
				{
					MinDistance = Distance;
					Rotation = Count2;
				}
			}

			for (std::int_fast32_t Count2{}; Count2 < ScaleFactor1; ++Count2)
			{
				PolygonCoords[Index1 + Count2][0] = Index1 + Count2;
				Index2 = Count2 + 1;
				Index2 %= ScaleFactor1;

				PolygonCoords[Index1 + Count2][1] = Index1 + Index2;
				Index2 = Count2 + Rotation + 1;
				Index2 %= ScaleFactor1;

				PolygonCoords[Index1 + Count2][2] = (Index1 + Index2 + ScaleFactor1) % Vertices;
				Index2 = Count2 + Rotation;
				Index2 %= ScaleFactor1;

				PolygonCoords[Index1 + Count2][3] = (Index1 + Index2 + ScaleFactor1) % Vertices;
			}
		}

		for (std::int_fast32_t Count1{}; Count1 < Vertices; Count1++)
		{
			DrawOrder[Count1] = Count1;
		}
	}

	inline void Shade(const std::array<lwmf::IntPointStruct, 3>& VertexPoints, const std::array<std::int_fast32_t, 3>& Color)
	{
		std::int_fast32_t MinY{};
		std::int_fast32_t MaxY{};

		for (std::int_fast32_t Count{ 1 }; Count < 3; ++Count)
		{
			if (VertexPoints[Count].Y < VertexPoints[MinY].Y)
			{
				MinY = Count;
			}
			else if (VertexPoints[Count].Y > VertexPoints[MaxY].Y)
			{
				MaxY = Count;
			}
		}

		std::int_fast32_t EndVertex1{ MinY + 2 };

		if (EndVertex1 >= 3)
		{
			EndVertex1 -= 3;
		}

		std::int_fast32_t EndVertex2{ MinY + 1 };

		if (EndVertex2 >= 3)
		{
			EndVertex2 -= 3;
		}

		std::int_fast32_t StartVertex2{ MinY };
		std::int_fast32_t StartVertex1{ StartVertex2 };
		std::int_fast32_t XDiff1{ VertexPoints[EndVertex1].X - VertexPoints[StartVertex1].X };
		std::int_fast32_t YDiff1{ VertexPoints[EndVertex1].Y - VertexPoints[StartVertex1].Y };
		std::int_fast32_t XDiff2{ VertexPoints[EndVertex2].X - VertexPoints[StartVertex1].X };
		std::int_fast32_t YDiff2{ VertexPoints[EndVertex2].Y - VertexPoints[StartVertex1].Y };
		std::int_fast32_t ColourDiff1{ Color[EndVertex1] - Color[StartVertex1] };
		std::int_fast32_t ColourDiff2{ Color[EndVertex2] - Color[StartVertex2] };

		if (YDiff1 == 0)
		{
			YDiff1 = 1;
		}

		if (YDiff2 == 0)
		{
			YDiff2 = 1;
		}

		std::int_fast32_t XCalc1{};
		std::int_fast32_t XCalc2{};
		std::int_fast32_t ColourCalc1{};
		std::int_fast32_t ColourCalc2{};

		for (std::int_fast32_t Y{ VertexPoints[MinY].Y }; Y <= VertexPoints[MaxY].Y; ++Y)
		{
			const std::int_fast32_t X2{ VertexPoints[StartVertex1].X + XCalc1 / YDiff1 };
			XCalc1 += XDiff1;
			std::int_fast32_t X1{ VertexPoints[StartVertex2].X + XCalc2 / YDiff2 };
			XCalc2 += XDiff2;
			const std::int_fast32_t EndColour{ Color[StartVertex1] + ColourCalc1 / YDiff1 };
			ColourCalc1 += ColourDiff1;
			std::int_fast32_t StartColour{ Color[StartVertex2] + ColourCalc2 / YDiff2 };
			ColourCalc2 += ColourDiff2;

			const std::int_fast32_t ColourIncSign{ EndColour > StartColour ? 1 : -1 };
			const std::int_fast32_t ColourDiff{ std::abs(StartColour - EndColour) };
			const std::int_fast32_t LineWidth{ X2 - X1 };
			std::int_fast32_t ColourIncCount{ ColourDiff - (LineWidth >> 1) };

			for (std::int_fast32_t Count{ X1 }; Count < X2; ++Count)
			{
				lwmf::SetPixelSafe(ScreenTexture, Count, Y, StartColour);

				while (ColourIncCount >= 0)
				{
					StartColour += ColourIncSign;
					ColourIncCount -= LineWidth;
				}

				ColourIncCount += ColourDiff;
			}

			if (Y == VertexPoints[EndVertex1].Y)
			{
				StartVertex1 = EndVertex1;
				EndVertex1 = EndVertex2; //-V537
				XDiff1 = VertexPoints[EndVertex1].X - VertexPoints[StartVertex1].X;
				YDiff1 = VertexPoints[EndVertex1].Y - VertexPoints[StartVertex1].Y;
				ColourDiff1 = Color[EndVertex1] - Color[StartVertex1];

				if (YDiff1 == 0)
				{
					YDiff1 = 1;
				}

				XCalc1 = XDiff1;
				ColourCalc1 = ColourDiff1;
			}

			if (Y == VertexPoints[EndVertex2].Y)
			{
				StartVertex2 = EndVertex2;
				EndVertex2 = EndVertex1;
				XDiff2 = VertexPoints[EndVertex2].X - VertexPoints[StartVertex2].X;
				YDiff2 = VertexPoints[EndVertex2].Y - VertexPoints[StartVertex2].Y;
				ColourDiff2 = Color[EndVertex2] - Color[StartVertex2];

				if (YDiff2 == 0)
				{
					YDiff2 = 1;
				}

				XCalc2 = XDiff2;
				ColourCalc2 = ColourDiff2;
			}
		}
	}

	inline void RotatePoint(const std::int_fast32_t X, const std::int_fast32_t Y, const float SinAngle, const float CosAngle, std::int_fast32_t& EndX, std::int_fast32_t& EndY)
	{
		EndX = static_cast<std::int_fast32_t>(static_cast<float>(X) * CosAngle - static_cast<float>(Y) * SinAngle);
		EndY = static_cast<std::int_fast32_t>(static_cast<float>(Y) * CosAngle + static_cast<float>(X) * SinAngle);
	}

	inline void RotateShape()
	{
		static float Phi{};
		static float Theta{};
		const float SinePhi{ std::sinf(Phi) };
		const float CosinePhi{ std::cosf(Phi) };
		const float SineTheta{ std::sinf(Theta) };
		const float CosineTheta{ std::cosf(Theta) };

		for (VertexCount = 0; VertexCount < Vertices; ++VertexCount)
		{
			RotatePoint(Shape[VertexCount].y, Shape[VertexCount].z, SinePhi, CosinePhi, RotatedShape[VertexCount].y, RotatedShape[VertexCount].z);
			RotatePoint(Shape[VertexCount].x, RotatedShape[VertexCount].z, SineTheta, CosineTheta, RotatedShape[VertexCount].x, RotatedShape[VertexCount].z);
		}

		Phi += 0.04F;
		Theta += 0.03F;
	}

	inline void DrawFace(const std::int_fast32_t Index1, const std::int_fast32_t Index2, const std::int_fast32_t Index3)
	{
		std::array<lwmf::IntPointStruct, 3> Face2D{};
		std::array<std::int_fast32_t, 3> Colors{};

		Face2D[0].X = Shape2D[Index1].X;
		Face2D[0].Y = Shape2D[Index1].Y;
		Face2D[1].X = Shape2D[Index2].X;
		Face2D[1].Y = Shape2D[Index2].Y;
		Face2D[2].X = Shape2D[Index3].X;
		Face2D[2].Y = Shape2D[Index3].Y;

		constexpr std::int_fast32_t MaxZ{ Radius1 + Radius2 + Radius1 };

		Colors[0] = 30 + 127 * (RotatedShape[Index1].z + MaxZ) / MaxZ; //-V525
		Colors[1] = 50 + 127 * (RotatedShape[Index2].z + MaxZ) / MaxZ;
		Colors[2] = 30 + 127 * (RotatedShape[Index3].z + MaxZ) / MaxZ;

		Shade(Face2D, Colors);
	}

	inline std::int_fast32_t Compare(const void* Number1, const void *Number2)
	{
		return RotatedShape[*static_cast<const std::int_fast32_t*>(Number2)].z - RotatedShape[*static_cast<const std::int_fast32_t*>(Number1)].z; //-V206
	}

	inline void Draw()
	{
		RotateShape();
		std::qsort(DrawOrder.data(), NumberOfVertices, sizeof(DrawOrder[0]), Compare);

		lwmf::ClearTexture(ScreenTexture, 0x00000000);

		for (VertexCount = 0; VertexCount < Vertices; ++VertexCount)
		{
			const std::int_fast32_t ZValue{ (450 + RotatedShape[VertexCount].z) >> 2 };

			Shape2D[VertexCount].X = ScreenTexture.WidthMid + ((RotatedShape[VertexCount].x) << 7) / ZValue;
			Shape2D[VertexCount].Y = ScreenTexture.HeightMid + ((RotatedShape[VertexCount].y) << 7) / ZValue;
		}

		for (VertexCount = 0; VertexCount < Vertices; ++VertexCount)
		{
			DrawFace(PolygonCoords[DrawOrder[VertexCount]][0], PolygonCoords[DrawOrder[VertexCount]][1], PolygonCoords[DrawOrder[VertexCount]][2]);
			DrawFace(PolygonCoords[DrawOrder[VertexCount]][0], PolygonCoords[DrawOrder[VertexCount]][2], PolygonCoords[DrawOrder[VertexCount]][3]);
		}

		DisplayInfoBox("Realtime torus knot with gouraud shading - " + std::to_string(ScaleFactor1 * ScaleFactor2) + " vertices");
	}


} // namespace GouraudShade