#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <charconv>

namespace PixelTest
{


	inline void Draw()
	{
		static std::array<char, 25> PixelCounterString{};
		static std::int_fast64_t PixelCounter{};

		for (std::int_fast32_t y{}; y < ScreenTexture.Height; ++y)
		{
			for (std::int_fast32_t x{}; x < ScreenTexture.Width; ++x)
			{
				lwmf::SetPixel(ScreenTexture, x, y, lwmf::XorShift32());
				++PixelCounter;
			}
		}

		std::to_chars(PixelCounterString.data(), PixelCounterString.data() + PixelCounterString.size(), PixelCounter);
		lwmf::FilledRectangle(ScreenTexture, 0, 65, ScreenTexture.Width, 20, 0, 0);
		lwmf::RenderText(ScreenTexture, "Number of pixels in total: " + std::string(PixelCounterString.data()), 10, 70, 0xFFFFFFFF);
	}


} // namespace PixelTest
