#pragma once

#include <array>
#include <charconv>

namespace ThroughputTest
{


	inline void Draw()
	{
		lwmf::ClearTexture(Canvas, lwmf::XorShift32());

		DisplayInfoBox("Throughput test 2 (lwmf::ClearTexture)");

		std::array<char, 30> ThroughputString{};

		// 1.073.741.824 = 1024x1024x1024
		std::to_chars(ThroughputString.data(), ThroughputString.data() + ThroughputString.size(), static_cast<double>(static_cast<std::uint_fast64_t>(lwmf::FPS) * (static_cast<std::uint_fast64_t>(Canvas.Size) * 4)) / 1073741824.0);
		lwmf::RenderText(Canvas, "Throughput in GigaByte/s: " + std::string(ThroughputString.data()), 10, 70, 0xFFFFFFFF);

		std::array<char, 10> PixelString{};
		std::to_chars(PixelString.data(), PixelString.data() + PixelString.size(), Canvas.Size);
		lwmf::RenderText(Canvas, "Cleared pixels per frame: " + std::string(PixelString.data()), 10, 80, 0xFFFFFFFF);
	}


} // namespace ThroughputTest
