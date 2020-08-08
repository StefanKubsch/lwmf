#pragma once

#include <array>
#include <charconv>

namespace ThroughputTest
{


	inline void Draw()
	{
		lwmf::ClearTexture(ScreenTexture, lwmf::XorShift32());

		lwmf::FilledRectangle(ScreenTexture, 0, 65, ScreenTexture.Width, 30, 0, 0);

		std::array<char, 15> ThroughputString{};
		std::to_chars(ThroughputString.data(), ThroughputString.data() + ThroughputString.size(), static_cast<float>(lwmf::FPS * (ScreenTexture.Size << 2)) / (1024.0F * 1024.0F * 1024.0F));
		lwmf::RenderText(ScreenTexture, "Troughput in GigaByte/s: " + std::string(ThroughputString.data()), 10, 70, 0xFFFFFFFF);

		std::array<char, 15> PixelString{};
		std::to_chars(PixelString.data(), PixelString.data() + PixelString.size(), ScreenTexture.Size);
		lwmf::RenderText(ScreenTexture, "Clearing pixels per frame: " + std::string(PixelString.data()), 10, 80, 0xFFFFFFFF);
	}


} // namespace ThroughputTest
