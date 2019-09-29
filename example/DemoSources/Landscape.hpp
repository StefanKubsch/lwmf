#pragma once

#include <cstdint>
#include <cmath>

namespace Landscape
{


	inline lwmf::TextureStruct LandscapeTextureMap;
	inline lwmf::TextureStruct LandscapeTerrainMap;

	inline void Init()
	{
		lwmf::LoadPNG(LandscapeTextureMap, "./DemoGFX/LandscapeTexture.png");
		lwmf::LoadPNG(LandscapeTerrainMap, "./DemoGFX/Landscape.png");
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

		lwmf::ClearTexture(ScreenTexture, 0);

		for (std::int_fast32_t x{}; x < LandscapeTextureMap.Width; ++x)
		{
			for (std::int_fast32_t z{}; z < LandscapeTextureMap.Height; ++z)
			{
				const lwmf::ColorStruct LandScapeTerrainColorRGBA{ lwmf::INTtoRGBA(LandscapeTerrainMap.Pixels[z * LandscapeTerrainMap.Width + x]) };
				const std::int_fast32_t TempX{ ((x - XPos) * CosA - (z - ZPos) * SinA) };
				const std::int_fast32_t TempY{ (LandScapeTerrainColorRGBA.Red >> 1) - YPos };
				const std::int_fast32_t TempZ{ ((x - XPos) * SinA + (z - ZPos) * CosA) >> 7 };

				lwmf::SetPixel(ScreenTexture, (ScreenTexture.WidthMid + (TempX << 2) / TempZ), (ScreenTexture.HeightMid - (TempY << 7) / TempZ), LandscapeTextureMap.Pixels[z * LandscapeTextureMap.Width + x]);
			}
		}

		lwmf::RenderText(ScreenTexture, "Textured dotted landscape", 10, 10, 0xFFFFFFFF);
	}


} // namespace Landscape