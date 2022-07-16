#pragma once

#include <cstdint>
#include <array>
#include <charconv>

namespace TextureRotationTest
{


	inline lwmf::TextureStruct SourceTexture{};

	inline void Init()
	{
		lwmf::LoadPNG(SourceTexture, "./DemoGFX/Head.png");
		lwmf::ResizeTexture(SourceTexture, SourceTexture.Width * 3, SourceTexture.Height * 3, lwmf::FilterModes::BILINEAR);
	}

	inline void Draw()
	{
		static float Angle{};

		lwmf::TextureStruct Texture{ SourceTexture };
		lwmf::RotateTexture(Texture, Texture.WidthMid, Texture.HeightMid, Angle);
		lwmf::BlitTexture(Texture, ScreenTexture, ScreenTexture.WidthMid - Texture.WidthMid, ScreenTexture.HeightMid - Texture.HeightMid);

		DisplayInfoBox("Texture rotation test (lwmf::RotateTexture)");

		static std::uint_fast64_t RotateCounter{};
		std::array<char, 20> CounterString{};
		std::to_chars(CounterString.data(), CounterString.data() + CounterString.size(), ++RotateCounter);
		lwmf::RenderText(ScreenTexture, "Number of texture rotations: " + std::string(CounterString.data()), 10, 70, 0xFFFFFFFF);

		(Angle >= 359.0F) ? Angle = 0.0F : Angle += 0.005F;
	}


} // namespace TextureRotationTest
