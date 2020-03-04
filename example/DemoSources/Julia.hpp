#pragma once

#include <complex>
#include <cstdint>

namespace Julia
{


	static constexpr std::int_fast32_t Iterations{ 256 };

	inline std::int_fast32_t Set(std::complex<float> z, const std::complex<float>& c)
	{
		for (std::int_fast32_t Result{}; Result < Iterations; ++Result)
		{
			z = (z * z) + c;

			if ((z.imag() * z.imag()) + (z.real() * z.real()) > 3)
			{
				return Result;
			}
		}

		return 0;
	}

	inline void Draw()
	{
		static constexpr std::int_fast32_t Size{ 600 };
		static constexpr float FCT{ 2.85F };
		static constexpr float hFCT{ FCT * 0.5F };
		static constexpr float Factor{ FCT / Size };
		static constexpr std::complex<float> f(Factor, Factor);
		static std::complex<float> Param{};
		static float Var1{};
		static float Var2{};

		Param.imag(Factor * 184.0F + Var1);
		Param.real(Factor * 307.0F + Var2);

		if (Var1 > -0.3F)
		{
			Var1 -= 0.0003F;
		}

		if (Var2 > -2.0F)
		{
			Var2 -= 0.003F;
		}

		static std::complex<float> c{};

		lwmf::ClearTexture(ScreenTexture, 0x00000000);

		for (std::int_fast32_t y{}; y < ScreenTexture.Height; ++y)
		{
			c.imag((f.imag() * y) - hFCT);

			for (std::int_fast32_t x{}; x < ScreenTexture.Width; ++x)
			{
				c.real(f.real() * x - hFCT);

				if (const std::int_fast32_t Result{ Set(c, Param) }; Result != 0)
				{
					const std::int_fast32_t NewResult{ Result & 255 };
					const std::int_fast32_t Color{ Result < (Iterations >> 1) ? lwmf::RGBAtoINT(NewResult << 2, NewResult << 3, NewResult << 4, 255) : lwmf::RGBAtoINT(NewResult << 4, NewResult << 2, NewResult << 5, 255) };
					lwmf::SetPixelSafe(ScreenTexture, (x + 100), y, Color);
				}
			}
		}
	}


} // namespace Julia