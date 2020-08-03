#pragma once

#include <cstdint>
#include <random>
#include <array>
#include <charconv>

namespace BitmapTest
{


	inline lwmf::TextureStruct SourceTexture{};

	inline void Init()
	{
		lwmf::LoadPNG(SourceTexture, "./DemoGFX/Head.png");
	}

	inline void Draw()
	{
		static const std::uniform_int_distribution<std::int_fast32_t> Point(-255, ScreenTexture.Width + 255);
		static const std::uniform_int_distribution<std::int_fast32_t> Size(1, 500);
		static std::int_fast64_t BlitCounter{};
		static std::array<char, 25> CounterString{};

		lwmf::TextureStruct Texture{ SourceTexture };
		lwmf::ResizeTexture(Texture, Size(Engine), Size(Engine), lwmf::FilterModes::BILINEAR);
		lwmf::BlitTexture(Texture, ScreenTexture, Point(Engine), Point(Engine));

		std::to_chars(CounterString.data(), CounterString.data() + CounterString.size(), ++BlitCounter);

		lwmf::FilledRectangle(ScreenTexture, 0, 65, ScreenTexture.Width, 20, 0, 0);
		lwmf::RenderText(ScreenTexture, "Number of bitmap blits (resize with bilinear filtering): " + std::string(CounterString.data()), 10, 70, 0xFFFFFFFF);
	}


} // namespace BitmapTest
