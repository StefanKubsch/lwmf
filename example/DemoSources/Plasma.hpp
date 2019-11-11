#pragma once

#include <cstdint>
#include <cmath>
#include <array>
#include <utility>

namespace Plasma
{


	inline void Draw()
	{
		static std::array<std::pair<std::int_fast32_t, std::int_fast32_t>, 3> Plasmas{ { {5, 2}, {125, 1}, {250,3} } };

		for (auto& Plasma : Plasmas)
		{
			if (Plasma.first > 255 || Plasma.first <= 1)
			{
				Plasma.second = -Plasma.second;
			}

			Plasma.first += Plasma.second;
		}

		#pragma omp parallel for
		for (std::int_fast32_t y{}; y < ScreenTexture.Height; ++y)
		{
			const float TempY{ 64.0F * std::cosf(y / 64.0F) };

			for (std::int_fast32_t x{}; x < ScreenTexture.Width; ++x)
			{
				const std::int_fast32_t Color{ static_cast<std::int_fast32_t>((128.0F * std::sinf(x / 64.0F)) + TempY) };
				lwmf::SetPixel(ScreenTexture, x, y, lwmf::RGBAtoINT((Color + Plasmas[2].first) & 255, (Color + Plasmas[0].first) & 255, (Color + Plasmas[1].first) & 255, 255));
			}
		}
	}


} // namespace Plasma
