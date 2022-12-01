#pragma once

#include <cstdint>
#include <cmath>

namespace RotoZoom
{


	inline lwmf::TextureStruct Texture{};

	inline void Init()
	{
		lwmf::LoadPNG(Texture, "./DemoGFX/Head.png");
	}

	inline void Draw()
	{
		static std::int_fast32_t Angle{};
		const float AngleProd{ Angle * lwmf::RAD2DEG };
		const float Val1{ std::cosf(AngleProd) };
		const float Val2{ std::sinf(AngleProd) };

		++Angle;
		Angle %= 360;

		for (std::int_fast32_t y{}; y < Canvas.Height; ++y)
		{
			for (std::int_fast32_t x{}; x < Canvas.Width; ++x)
			{
				lwmf::SetPixel(Canvas, x, y, Texture.Pixels[(static_cast<std::int_fast32_t>((x * Val2 + y * Val1) * (Val2 + 1)) & Texture.Height - 1) * Texture.Width + (static_cast<std::int_fast32_t>((x * Val1 - y * Val2) * (Val2 + 1)) & Texture.Width - 1)]);
			}
		}

		DisplayInfoBox("RotoZoomer");
	}


} // namespace RotoZoom