#pragma once

#include <cstdint>
#include <vector>
#include <random>
#include <cmath>

namespace Swarm
{


	inline void Draw()
	{
		struct ParticleStruct final
		{
			lwmf::FloatPointStruct Pos;
			float Speed{};
			float Direction{};

			ParticleStruct() { Init(); }

			void Init()
			{
				static std::mt19937_64 Engine;
				static const std::uniform_int_distribution<std::int_fast32_t> Distrib1(0, RAND_MAX);

				Pos.x = 0.0F;
				Pos.y = 0.0F;

				Direction = (lwmf::DoublePI * Distrib1(Engine)) / RAND_MAX;
				Speed = (0.1F * Distrib1(Engine)) / RAND_MAX;
				Speed *= Speed;
			}

			void Update(const std::int_fast32_t Interval)
			{
				Pos.x += (Speed * std::cosf(Direction)) * Interval;
				Pos.y += (Speed * std::sinf(Direction)) * Interval;
				Direction += Interval * 0.0009F;

				if (Pos.x < -1 || Pos.x > 1 || Pos.y < -1 || Pos.y > 1)
				{
					Init();
				}
			}
		};

		static constexpr std::int_fast32_t NumberOfParticles{ 30000 };
		static std::vector<ParticleStruct> Particles(NumberOfParticles);
		static const std::int_fast32_t ParticleColor{ lwmf::RGBAtoINT(255, 0, 255, 255) };
		static std::int_fast32_t LastTime{};
		const std::int_fast32_t Elapsed{ static_cast<std::int_fast32_t>(GetTickCount()) };
		const std::int_fast32_t Interval{ Elapsed - LastTime };

		lwmf::ClearPixelBuffer(0);

		for (std::int_fast32_t i{}; i < NumberOfParticles; ++i)
		{
			Particles[i].Update(Interval);
			lwmf::SetPixelSafe(static_cast<std::int_fast32_t>((Particles[i].Pos.x + 1.0F) * lwmf::ViewportWidthMid), static_cast<std::int_fast32_t>(Particles[i].Pos.y * lwmf::ViewportWidthMid + lwmf::ViewportHeightMid), ParticleColor);
		}

		LastTime = Elapsed;
		lwmf::RenderText("Realtime particle swarm - 30.000 particles", 10, 10, 0xFFFFFFFF);
	}


} // namespace Swarm