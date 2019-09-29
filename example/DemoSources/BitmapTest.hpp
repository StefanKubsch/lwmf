#pragma once

#include <cstdint>
#include <random>
#include <vector>
#include <charconv>

namespace BitmapTest
{


	inline lwmf::TextureStruct SourceTexture;

	inline void Init()
	{
		lwmf::LoadPNG(SourceTexture, "./DemoGFX/Head.png");
	}

	void Draw()
	{
		static const std::uniform_int_distribution<std::int_fast32_t> Point(-255, ScreenTexture.Width + 255);
		static const std::uniform_int_distribution<std::int_fast32_t> Size(1, 500);
		static std::int_fast64_t BlitCounter{};
		static std::vector<char> CounterString(20);

		lwmf::TextureStruct Texture{ SourceTexture };
		lwmf::ResizeTexture(Texture, Size(Engine), Size(Engine), lwmf::FilterModes::NEAREST);
		lwmf::BlitTexture(Texture, ScreenTexture, Point(Engine), Point(Engine));

		std::to_chars(CounterString.data(), CounterString.data() + CounterString.size(), ++BlitCounter);

		lwmf::FilledRectangle(ScreenTexture, 0, 0, ScreenTexture.Width - 1, 45, 0, 0);
		lwmf::RenderText(ScreenTexture, "Bitmap resize & blitting test", 10, 10, 0xFFFFFFFF);
		lwmf::RenderText(ScreenTexture, "Number of bitmap blits (resize by nearest neighbour): " + std::string(CounterString.data()), 10, 30, 0xFFFFFFFF);
	}


} // namespace BitmapTest
