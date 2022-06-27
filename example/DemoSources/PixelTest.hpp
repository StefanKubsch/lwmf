#pragma once

#include <cstdint>
#include <array>
#include <charconv>

namespace PixelTest
{


	inline void Draw()
	{
		const std::int_fast32_t Color{ static_cast<std::int_fast32_t>(lwmf::XorShift32()) };

		#pragma omp parallel for
		for (std::int_fast32_t y{}; y < ScreenTexture.Height; ++y)
		{
			for (std::int_fast32_t x{}; x < ScreenTexture.Width; ++x)
			{
				lwmf::SetPixel(ScreenTexture, x, y, Color);
			}
		}

		DisplayInfoBox("Throughput test 1 (lwmf::SetPixel, OpenMP accelerated)");

		std::array<char, 15> MPixelsPerSecond{};
		std::to_chars(MPixelsPerSecond.data(), MPixelsPerSecond.data() + MPixelsPerSecond.size(), static_cast<float>(static_cast<std::uint_fast64_t>(ScreenTexture.Size) * static_cast<std::uint_fast64_t>(lwmf::FPS)) / 1000000.0F );
		lwmf::RenderText(ScreenTexture, "Throughput in MegaPixel/s: " + std::string(MPixelsPerSecond.data()), 10, 70, 0xFFFFFFFF);

		// 1.073.741.824 = 1024x1024x1024
		std::array<char, 30> ThroughputString{};
		std::to_chars(ThroughputString.data(), ThroughputString.data() + ThroughputString.size(), static_cast<double>(static_cast<std::uint_fast64_t>(lwmf::FPS) * (static_cast<std::uint_fast64_t>(ScreenTexture.Size) * 4)) / 1073741824.0);
		lwmf::RenderText(ScreenTexture, "Throughput in GigaByte/s: " + std::string(ThroughputString.data()), 10, 80, 0xFFFFFFFF);

		std::array<char, 10> PixelString{};
		std::to_chars(PixelString.data(), PixelString.data() + PixelString.size(), ScreenTexture.Size);
		lwmf::RenderText(ScreenTexture, "Drawn pixels per frame: " + std::string(PixelString.data()), 10, 90, 0xFFFFFFFF);
	}


} // namespace PixelTest
