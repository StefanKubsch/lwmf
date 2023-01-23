#pragma once

#include <cstdint>
#include <random>
#include <vector>
#include <algorithm>

namespace Particles
{


    class Particle
    {
    public:

        void Draw() const;
        void Update();

        std::int_fast32_t x{};
        std::int_fast32_t y{};
        std::int_fast32_t DirectionX{};
        std::int_fast32_t DirectionY{};
        std::int_fast32_t Size{};
    };

    inline void Particle::Draw() const
    {
        const std::int_fast32_t Factor{ 130 + (Size * 40) };
        const std::int_fast32_t Color = lwmf::RGBAtoINT(Factor, Factor, Factor, Factor);

        lwmf::FilledCircle(Canvas, x, y, Size, Color, Color);
    }

    inline void Particle::Update()
    {
        std::uniform_int_distribution<std::int_fast32_t> Distrib3(5, 8);

        if (static_cast<std::uint_fast32_t>(x) > static_cast<std::uint_fast32_t>(Canvas.Width))
        {
            DirectionX = -DirectionX;
            Size = Distrib3(RNG);
        }

        if (y > Canvas.Height || y < 115)
        {
            DirectionY = -DirectionY;
            Size = Distrib3(RNG);
        }

        x += DirectionX;
        y += DirectionY;
    }

    inline std::vector<Particle> ParticlesArray{};
    inline lwmf::TextureStruct Wallpaper{};

    inline void Init()
    {
        lwmf::LoadPNG(Wallpaper, "./DemoGFX/Colors.png");

        std::uniform_int_distribution<std::int_fast32_t> Distrib1(0, Canvas.Width);
        std::uniform_int_distribution<std::int_fast32_t> Distrib2(120, Canvas.Height);
        std::uniform_int_distribution<std::int_fast32_t> Distrib3(5, 8);
        std::uniform_int_distribution<std::int_fast32_t> Distrib4(1, 3);

        for (std::int_fast32_t i{}; i < Canvas.Size / 8000; ++i)
        {
            ParticlesArray.push_back({ Distrib1(RNG), Distrib2(RNG), Distrib4(RNG), Distrib4(RNG), Distrib3(RNG) });
        }
    }

    inline void Connect()
    {
        const float DistComp{ (static_cast<float>(Canvas.Width) / 7.0F) * (static_cast<float>(Canvas.Height) / 7.0F) };
        const std::size_t NumberOfParticles{ ParticlesArray.size() };

        for (std::int_fast32_t a{}; a < NumberOfParticles; ++a)
        {
            for (std::int_fast32_t b{ a }; b < NumberOfParticles; ++b)
            {
                const float Distance{ static_cast<float>(((ParticlesArray[a].x - ParticlesArray[b].x) * (ParticlesArray[a].x - ParticlesArray[b].x)) + ((ParticlesArray[a].y - ParticlesArray[b].y) * (ParticlesArray[a].y - ParticlesArray[b].y))) };

                if (Distance < DistComp)
                {
                    const std::int_fast32_t Opaque{ static_cast<std::int_fast32_t>(255 - ((Distance / 20000.0F) * 100.0F)) };
                    lwmf::LineAA(Canvas, ParticlesArray[a].x, ParticlesArray[a].y, ParticlesArray[b].x, ParticlesArray[b].y, lwmf::RGBAtoINT(Opaque, 0, 0, Opaque));
                }
            }
        }
    }

	inline void Draw()
	{
        if (Canvas.Width != Wallpaper.Width || Canvas.Height != Wallpaper.Height)
        {
            lwmf::ResizeTexture(Wallpaper, Canvas.Width, Canvas.Height, lwmf::FilterModes::NEAREST);
        }

        lwmf::BlitTexture(Wallpaper, Canvas, 0, 0);

        for (auto& Particle : ParticlesArray)
        {
            Particle.Update();
        }

        std::sort(ParticlesArray.begin(), ParticlesArray.end(), [](const Particle& lhs, const Particle& rhs)
        {
            return lhs.Size < rhs.Size;
        });

        Connect();

        for (auto& Particle : ParticlesArray)
        {
            Particle.Draw();
        }

		DisplayInfoBox("Particles - connected with anti-aliased lines");
	}


} // namespace Particles
