#pragma once

#include <cstdint>
#include <cmath>

namespace Landscape
{


	inline lwmf::BitmapStruct LandscapeTextureMap;
	inline lwmf::BitmapStruct LandscapeTerrainMap;

	inline void Init()
	{
		lwmf::LoadBMP(LandscapeTextureMap, "./DemoGFX/LandscapeTexture.bmp");
		lwmf::LoadBMP(LandscapeTerrainMap, "./DemoGFX/Landscape.bmp");
	}

	inline void Draw()
	{
		static constexpr std::int_fast32_t YPos{ 40 };
		static constexpr std::int_fast32_t XOrigin{ 64 };
		static constexpr std::int_fast32_t ZOrigin{ 64 };
		static float LandscapeAngle{};

		LandscapeAngle += 10.0F;

		if (LandscapeAngle > 627.0F)
		{
			LandscapeAngle = 0.0F;
		}

		const std::int_fast32_t XPos{ 63 + (((static_cast<std::int_fast32_t>(std::cosf(LandscapeAngle * 0.01F) * 128.0F)) << 7) >> 7) };
		const std::int_fast32_t ZPos{ 63 + (((static_cast<std::int_fast32_t>(std::sinf(LandscapeAngle * 0.01F) * 128.0F)) << 7) >> 7) };
		const float Factor{ ZOrigin - ZPos > 0 ? 100.0F * (std::atanf(static_cast<float>((XOrigin - XPos)) / (ZOrigin - ZPos))) : 100.0F * (std::acosf(-1.0F) + std::atanf(static_cast<float>((XOrigin - XPos)) / (ZOrigin - ZPos))) };
		const std::int_fast32_t CosA{ static_cast<std::int_fast32_t>(std::cosf(Factor * 0.01F) * 128.0F) };
		const std::int_fast32_t SinA{ static_cast<std::int_fast32_t>(std::sinf(Factor * 0.01F) * 128.0F) };

		lwmf::ClearPixelBuffer(0);

		for (std::int_fast32_t x{}; x < LandscapeTextureMap.Width; ++x)
		{
			for (std::int_fast32_t z{}; z < LandscapeTextureMap.Width; ++z)
			{
				const std::int_fast32_t TempX{ ((x - XPos) * CosA - (z - ZPos) * SinA) };
				const lwmf::ColorStruct LandScapeTerrainColorRGBA{ lwmf::INTtoRGBA(LandscapeTerrainMap.BitmapData[z * LandscapeTerrainMap.Width + x]) };
				const std::int_fast32_t TempY{ (LandScapeTerrainColorRGBA.Red >> 1) - YPos };
				const std::int_fast32_t TempZ{ ((x - XPos) * SinA + (z - ZPos) * CosA) >> 7 };

				lwmf::SetPixel((lwmf::ViewportWidthMid + (TempX << 2) / TempZ), (lwmf::ViewportHeightMid - (TempY << 7) / TempZ), LandscapeTextureMap.BitmapData[z * LandscapeTextureMap.Width + x]);
			}
		}

		lwmf::RenderText("Textured dotted landscape", 10, 10, 0xFFFFFFFF);
	}


} // namespace Landscape