#pragma once

#include <array>
#include <charconv>

namespace ThroughputTest
{


	inline void Draw()
	{
		lwmf::ClearTexture(ScreenTexture, lwmf::XorShift32());

		DisplayInfoBox("Throughput test 2 (std::fill)");

		std::array<char, 15> ThroughputString{};
		std::to_chars(ThroughputString.data(), ThroughputString.data() + ThroughputString.size(), static_cast<float>(static_cast<std::uint_fast32_t>(lwmf::FPS) * (static_cast<std::uint_fast32_t>(ScreenTexture.Size) * 4)) / (1024.0F * 1024.0F * 1024.0F));
		lwmf::RenderText(ScreenTexture, "Troughput in GigaByte/s: " + std::string(ThroughputString.data()), 10, 70, 0xFFFFFFFF);

		std::array<char, 10> PixelString{};
		std::to_chars(PixelString.data(), PixelString.data() + PixelString.size(), ScreenTexture.Size);
		lwmf::RenderText(ScreenTexture, "Cleared pixels per frame: " + std::string(PixelString.data()), 10, 80, 0xFFFFFFFF);
	}


} // namespace ThroughputTest
