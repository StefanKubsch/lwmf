#pragma once

#include <cstdint>
#include <array>
#include <charconv>

namespace PixelTest
{


	inline void Draw()
	{
		for (std::int_fast32_t y{}; y < ScreenTexture.Height; ++y)
		{
			for (std::int_fast32_t x{}; x < ScreenTexture.Width; ++x)
			{
				lwmf::SetPixel(ScreenTexture, x, y, lwmf::XorShift32());
			}
		}

		lwmf::FilledRectangle(ScreenTexture, 0, 65, ScreenTexture.Width, 20, 0, 0);

		std::array<char, 15> MPixelsPerSecond{};
		std::to_chars(MPixelsPerSecond.data(), MPixelsPerSecond.data() + MPixelsPerSecond.size(), static_cast<float>(static_cast<std::uint_fast64_t>(ScreenTexture.Size) * static_cast<std::uint_fast64_t>(lwmf::FPS)) / 1000000.0F );
		lwmf::RenderText(ScreenTexture, "Throughput in MegaPixels/s: " + std::string(MPixelsPerSecond.data()), 10, 70, 0xFFFFFFFF);
	}


} // namespace PixelTest
