#pragma once

#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <array>
#include <random>
#include <cmath>

namespace Swarm
{


	inline void Draw()
	{
		struct ParticleStruct final
		{
			lwmf::FloatPointStruct Pos{};
			float Speed{};
			float Direction{};

			ParticleStruct() { Init(); }

			void Init()
			{
				static const std::uniform_int_distribution<std::int_fast32_t> Distrib1(0, RAND_MAX);

				Pos = { 0.0F, 0.0F };

				Direction = (lwmf::DoublePI * Distrib1(RNG)) / RAND_MAX;
				Speed = (0.1F * Distrib1(RNG)) / RAND_MAX;
				Speed *= Speed;
			}

			void Update(const std::int_fast32_t Interval)
			{
				Pos.X += (Speed * std::cosf(Direction)) * Interval;
				Pos.Y += (Speed * std::sinf(Direction)) * Interval;
				Direction += static_cast<float>(Interval) * 0.0009F;

				if (Pos.X < -1.0F || Pos.X > 1.0F || Pos.Y < -1.0F || Pos.Y > 1.0F)
				{
					Init();
				}
			}
		};

		constexpr std::int_fast32_t NumberOfParticles{ 250000 };
		static std::array<ParticleStruct, NumberOfParticles> Particles{};
		static std::int_fast32_t LastTime{};
		const std::int_fast32_t Elapsed{ static_cast<std::int_fast32_t>(GetTickCount()) };
		const std::int_fast32_t Interval{ Elapsed - LastTime };

		lwmf::ClearTexture(ScreenTexture, 0x00000000);

		for (std::int_fast32_t i{}; i < NumberOfParticles; ++i)
		{
			Particles[i].Update(Interval);
			lwmf::SetPixelSafe(ScreenTexture, static_cast<std::int_fast32_t>((Particles[i].Pos.X + 1.0F) * ScreenTexture.WidthMid), static_cast<std::int_fast32_t>(Particles[i].Pos.Y * ScreenTexture.WidthMid + ScreenTexture.HeightMid), i >> 7);
		}

		LastTime = Elapsed;

		DisplayInfoBox("Realtime particle swarm - " + std::to_string(NumberOfParticles) + " particles");
	}


} // namespace Swarm