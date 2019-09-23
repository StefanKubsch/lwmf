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
				static const std::uniform_int_distribution<std::int_fast32_t> Distrib1(0, RAND_MAX);

				Pos.X = 0.0F;
				Pos.Y = 0.0F;

				Direction = (lwmf::DoublePI * Distrib1(Engine)) / RAND_MAX;
				Speed = (0.1F * Distrib1(Engine)) / RAND_MAX;
				Speed *= Speed;
			}

			void Update(const std::int_fast32_t Interval)
			{
				Pos.X += (Speed * std::cosf(Direction)) * Interval;
				Pos.Y += (Speed * std::sinf(Direction)) * Interval;
				Direction += Interval * 0.0009F;

				if (Pos.X < -1 || Pos.X > 1 || Pos.Y < -1 || Pos.Y > 1)
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

		lwmf::ClearTexture(ScreenTexture, 0);

		#pragma omp parallel for
		for (std::int_fast32_t i{}; i < NumberOfParticles; ++i)
		{
			Particles[i].Update(Interval);
			lwmf::SetPixelSafe(ScreenTexture, static_cast<std::int_fast32_t>((Particles[i].Pos.X + 1.0F) * ScreenTexture.WidthMid), static_cast<std::int_fast32_t>(Particles[i].Pos.Y * ScreenTexture.WidthMid + ScreenTexture.HeightMid), ParticleColor);
		}

		LastTime = Elapsed;
		lwmf::RenderText(ScreenTexture, "Realtime particle swarm - 30.000 particles", 10, 10, 0xFFFFFFFF);
	}


} // namespace Swarm