#pragma once

#include <cstdint>
#include <cmath>

namespace DotTunnel
{


	inline void Draw()
	{
		static constexpr std::int_fast32_t DotTunnelRings{ 120 };
		static constexpr std::int_fast32_t DotTunnelSpace{ 3 };
		static constexpr float DotTunnelRadius{ 2500.0F };
		static constexpr float DotTunnelAdd{ 0.06F };
		static const float RadY1{ 300.0F * std::sinf(DotTunnelAdd * lwmf::RAD2DEG) + DotTunnelRadius };
		static const float RadY2{ 300.0F * std::cosf(DotTunnelAdd * lwmf::RAD2DEG) + DotTunnelRadius };
		static float DotTunnelMove{};
		static float Warp{};
		float Depth{ 35.0F };
		const float DepthAdd{ Depth / DotTunnelRings };

		Depth += DotTunnelMove;
		DotTunnelMove -= 0.3F;
		Warp -= 0.3F;
		float Factor{ Warp };

		if (DotTunnelMove <= 0.0F)
		{
			DotTunnelMove += DepthAdd * 2.0F;
			Warp -= DepthAdd * 60.0F;
		}

		lwmf::ClearTexture(ScreenTexture, 0);

		for (std::int_fast32_t j{}; j < DotTunnelRings; ++j)
		{
			const float SinA{ 260.0F * std::sinf(Factor * lwmf::RAD2DEG) };

			for (std::int_fast32_t i{ 360 }; i >= 0; i -= DotTunnelSpace)
			{
				const float Calc{ lwmf::RAD2DEG * (static_cast<float>(i) + DotTunnelAdd) };

				lwmf::SetPixelSafe(ScreenTexture, static_cast<std::int_fast32_t>(((RadY1 * std::sinf(Calc) + SinA) / Depth)) + ScreenTexture.WidthMid,
					static_cast<std::int_fast32_t>(((RadY2 * std::cosf(Calc) + SinA) / Depth)) + ScreenTexture.HeightMid, lwmf::RGBAtoINT(255 - j, 255 - j, 255, 255 - j));
			}

			Factor += 6.0F;
			Depth -= DepthAdd;
		}
	}


} // namespace DotTunnel