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

        void Draw();
        void Update();

        std::int_fast32_t x{};
        std::int_fast32_t y{};
        std::int_fast32_t DirectionX{};
        std::int_fast32_t DirectionY{};
        std::int_fast32_t Size{};
    };

    inline void Particle::Draw()
    {
        const std::int_fast32_t Factor{ 130 + (Size * 40) };
        const std::int_fast32_t Color = lwmf::RGBAtoINT(Factor, Factor, Factor, Factor);

        lwmf::FilledCircle(ScreenTexture, x, y, Size, Color, Color);
    }

    inline void Particle::Update()
    {
        std::uniform_int_distribution<std::int_fast32_t> Distrib3(5, 8);

        if (static_cast<std::uint_fast32_t>(x) > static_cast<std::uint_fast32_t>(ScreenTexture.Width))
        {
            DirectionX = -DirectionX;
            Size = Distrib3(RNG);
        }

        if (y > ScreenTexture.Height || y < 115)
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

        std::uniform_int_distribution<std::int_fast32_t> Distrib1(0, ScreenTexture.Width);
        std::uniform_int_distribution<std::int_fast32_t> Distrib2(120, ScreenTexture.Height);
        std::uniform_int_distribution<std::int_fast32_t> Distrib3(5, 8);
        std::uniform_int_distribution<std::int_fast32_t> Distrib4(1, 3);

        for (std::int_fast32_t i{}; i < ScreenTexture.Size / 8000; ++i)
        {
            ParticlesArray.push_back({ Distrib1(RNG), Distrib2(RNG), Distrib4(RNG), Distrib4(RNG), Distrib3(RNG) });
        }
    }

    inline void Connect()
    {
        const float DistComp{ (static_cast<float>(ScreenTexture.Width) / 7.0F) * (static_cast<float>(ScreenTexture.Height) / 7.0F) };
        const std::size_t NumberOfParticles{ ParticlesArray.size() };

        for (std::int_fast32_t a{}; a < NumberOfParticles; ++a)
        {
            for (std::int_fast32_t b{ a }; b < NumberOfParticles; ++b)
            {
                const float Distance{ static_cast<float>(((ParticlesArray[a].x - ParticlesArray[b].x) * (ParticlesArray[a].x - ParticlesArray[b].x)) + ((ParticlesArray[a].y - ParticlesArray[b].y) * (ParticlesArray[a].y - ParticlesArray[b].y))) };

                if (Distance < DistComp)
                {
                    const std::int_fast32_t Opaque{ static_cast<std::int_fast32_t>(255 - ((Distance / 20000.0F) * 100.0F)) };
                    lwmf::LineAA(ScreenTexture, ParticlesArray[a].x, ParticlesArray[a].y, ParticlesArray[b].x, ParticlesArray[b].y, lwmf::RGBAtoINT(Opaque, 0, 0, Opaque));
                }
            }
        }
    }

	inline void Draw()
	{
        if (ScreenTexture.Width != Wallpaper.Width || ScreenTexture.Height != Wallpaper.Height)
        {
            lwmf::ResizeTexture(Wallpaper, ScreenTexture.Width, ScreenTexture.Height, lwmf::FilterModes::NEAREST);
        }

        lwmf::BlitTexture(Wallpaper, ScreenTexture, 0, 0);

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
