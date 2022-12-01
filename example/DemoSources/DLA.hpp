#pragma once

#include <cstdint>
#include <vector>
#include <cmath>
#include <random>
#include <execution>

namespace DLA
{


    //
    // https://en.wikipedia.org/wiki/Diffusion-limited_aggregation
    //

    inline constexpr std::int_fast32_t NumberOfParticles{ 280000 };
    inline constexpr float AggregationThresh{ 0.8F };
    inline constexpr float AggregationDecay{ 0.89F };
    inline constexpr float AggregationPrevent{ 0.001F };

    class Particle
    {
    public:
        lwmf::IntPointStruct Point{};

        Particle()
        {
            RandomPos();
        }

        void Update()
        {
            std::uniform_int_distribution<std::int_fast32_t> Choice(0, 7);

            switch (Choice(RNG))
            {
                case 0:
                {
                    --Point.X;
                    --Point.Y;
                    break;
                }
                case 1:
                {
                    --Point.Y;
                    break;
                }
                case 2:
                {
                    ++Point.X;
                    --Point.Y;
                    break;
                }
                case 3:
                {
                    ++Point.X;
                    break;
                }
                case 4:
                {
                    ++Point.X;
                    ++Point.Y;
                    break;
                }
                case 5:
                {
                    ++Point.Y;
                    break;
                }
                case 6:
                {
                    --Point.X;
                    ++Point.Y;
                    break;
                }
                case 7:
                {
                    --Point.X;
                    break;
                }
                default: {}
            }

            if (static_cast<std::uint_fast32_t>(Point.X) >= static_cast<std::uint_fast32_t>(Canvas.Width) || static_cast<std::uint_fast32_t>(Point.Y) >= static_cast<std::uint_fast32_t>(Canvas.Height))
            {
                RandomPos();
            }
        }

        void RandomPos()
        {
            std::uniform_int_distribution<std::int_fast32_t> DistribX(0, Canvas.Width);
            std::uniform_int_distribution<std::int_fast32_t> DistribY(0, Canvas.Height);

            Point = { DistribX(RNG), DistribY(RNG) };
        }
    };

    inline std::vector<Particle> Particles{};
    inline std::vector<float> Aggregation{};

    inline void WriteAggregation(const float v, const std::int_fast32_t x, const std::int_fast32_t y)
    {
        if (static_cast<std::uint_fast32_t>(x) >= static_cast<std::uint_fast32_t>(Canvas.Width) || static_cast<std::uint_fast32_t>(y) >= static_cast<std::uint_fast32_t>(Canvas.Height))
        {
            return;
        }

        Aggregation[y * Canvas.Width + x] = v;
    }

    inline float GetAggregation(const std::int_fast32_t x, const std::int_fast32_t y)
    {
        if (static_cast<std::uint_fast32_t>(x) >= static_cast<std::uint_fast32_t>(Canvas.Width) || static_cast<std::uint_fast32_t>(y) >= static_cast<std::uint_fast32_t>(Canvas.Height))
        {
            return 0;
        }

        return Aggregation[y * Canvas.Width + x];
    }

    inline void Init()
    {
        Aggregation.resize(static_cast<std::size_t>(Canvas.Size));

        for (std::int_fast32_t y{}; y < Canvas.Height; ++y)
        {
            const float Value{ y > Canvas.HeightMid - 1 && y < Canvas.HeightMid + 1 ? 1.0F : 0.0F };

            for (std::int_fast32_t x{}; x < Canvas.Width; ++x)
            {
                WriteAggregation(Value, x, y);
            }
        }

        Particles.reserve(NumberOfParticles);

        for (std::int_fast32_t i{}; i < NumberOfParticles; ++i)
        {
            Particles.emplace_back(Particle());
        }
    }

    inline float SumUpNeighbours(const std::int_fast32_t x, const std::int_fast32_t y)
    {
        float Sum{ GetAggregation(x - 1, y - 1) };
        Sum += GetAggregation(x, y - 1);
        Sum += GetAggregation(x + 1, y - 1);
        Sum += GetAggregation(x + 1, y);
        Sum += GetAggregation(x + 1, y + 1);
        Sum += GetAggregation(x, y + 1);
        Sum += GetAggregation(x - 1, y + 1);
        Sum += GetAggregation(x - 1, y);

        return Sum;
    }

    inline void Draw()
    {
        for (auto& Particle : Particles)
        {
            Particle.Update();

            const float Dist{ std::sqrtf(static_cast<float>((Particle.Point.X - Canvas.WidthMid) * (Particle.Point.X - Canvas.WidthMid) + (Particle.Point.Y - Canvas.HeightMid) * (Particle.Point.Y - Canvas.HeightMid))) };

            if (SumUpNeighbours(Particle.Point.X, Particle.Point.Y) > AggregationThresh && GetAggregation(Particle.Point.X, Particle.Point.Y) < AggregationPrevent && Dist < 720.0F)
            {
                WriteAggregation(1.0F, Particle.Point.X, Particle.Point.Y);
                Particle.RandomPos();
            }
        }

        std::for_each(std::execution::par_unseq, Aggregation.begin(), Aggregation.end(),[](auto&& Item)
        {
            Item *= AggregationDecay;
        });

        for (std::int_fast32_t y{}; y < Canvas.Height; ++y)
        {
            for (std::int_fast32_t x{}; x < Canvas.Width; ++x)
            {
                const std::int_fast32_t Color{ static_cast<std::int_fast32_t>(std::min(0.5F, GetAggregation(x, y)) * 384.0F) };
                lwmf::SetPixel(Canvas, x, y, lwmf::RGBAtoINT(Color, 0, Color, 255));
            }
        }

        DisplayInfoBox("Diffusion-limited aggregation - " + std::to_string(NumberOfParticles) + " particles");
    }


} // namespace DLA