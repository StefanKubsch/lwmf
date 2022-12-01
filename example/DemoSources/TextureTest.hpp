#pragma once

#include <cstdint>
#include <random>
#include <array>
#include <charconv>

namespace TextureTest
{


	inline lwmf::TextureStruct SourceTexture{};

	inline void Init()
	{
		lwmf::LoadPNG(SourceTexture, "./DemoGFX/Head.png");
	}

	inline void Draw()
	{
		lwmf::TextureStruct Texture{ SourceTexture };

		std::uniform_int_distribution<std::int_fast32_t> Size(0, 500);
		lwmf::ResizeTexture(Texture, Size(RNG), Size(RNG), lwmf::FilterModes::BILINEAR);

		std::uniform_int_distribution<std::int_fast32_t> Point(-255, Canvas.Width + 255);
		lwmf::BlitTexture(Texture, Canvas, Point(RNG), Point(RNG));

		DisplayInfoBox("Texture resize & blitting test (lwmf::ResizeTexture and lwmf::BlitTexture)");

		static std::uint_fast32_t BlitCounter{};
		std::array<char, 10> CounterString{};
		std::to_chars(CounterString.data(), CounterString.data() + CounterString.size(), ++BlitCounter);
		lwmf::RenderText(Canvas, "Number of texture blits (resize with bilinear filtering): " + std::string(CounterString.data()), 10, 70, 0xFFFFFFFF);
	}


} // namespace TextureTest
