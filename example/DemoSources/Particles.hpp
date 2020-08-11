#pragma once

#include <cstdint>
#include <random>
#include <vector>

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
#
    inline void Particle::Draw()
    {
        lwmf::FilledCircle(ScreenTexture, x, y, Size, 0xFF0000FF, 0xFFFFFFFF);
    }

    inline void Particle::Update()
    {
        if (x > ScreenTexture.Width || x < 0)
        {
            DirectionX = -DirectionX;
        }

        if (y > ScreenTexture.Height || y < 0)
        {
            DirectionY = -DirectionY;
        }

        x += DirectionX;
        y += DirectionY;
    }


    inline std::vector<Particle> ParticlesArray{};
    inline lwmf::TextureStruct Wallpaper{};

    inline void Init()
    {
        lwmf::LoadPNG(Wallpaper, "./DemoGFX/Colors.png");

        static const std::uniform_int_distribution<std::int_fast32_t> Distrib1(0, ScreenTexture.Width);
        static const std::uniform_int_distribution<std::int_fast32_t> Distrib2(0, ScreenTexture.Height);
        static const std::uniform_int_distribution<std::int_fast32_t> Distrib3(5, 8);
        static const std::uniform_int_distribution<std::int_fast32_t> Distrib4(1, 3);

        for (std::int_fast32_t i{}; i < ScreenTexture.Size / 8000; ++i)
        {
            ParticlesArray.emplace_back();
            ParticlesArray[i].x = Distrib1(Engine);
            ParticlesArray[i].y = Distrib2(Engine);
            ParticlesArray[i].DirectionX = Distrib4(Engine);
            ParticlesArray[i].DirectionY = Distrib4(Engine);
            ParticlesArray[i].Size = Distrib3(Engine);
        }
    }

    inline void Connect()
    {
        for (std::int_fast32_t a{}; a < ParticlesArray.size(); ++a)
        {
            for (std::int_fast32_t b{ a }; b < ParticlesArray.size(); ++b)
            {
                const float Distance{ static_cast<float>(((ParticlesArray[a].x - ParticlesArray[b].x) * (ParticlesArray[a].x - ParticlesArray[b].x)) + ((ParticlesArray[a].y - ParticlesArray[b].y) * (ParticlesArray[a].y - ParticlesArray[b].y))) };

                if (Distance < static_cast<float>((ScreenTexture.Width / 7) * (ScreenTexture.Height / 7)))
                {
                    const std::int_fast32_t Opaque{ static_cast<std::int_fast32_t>(255 - ((Distance / 20000.0F) * 255.0F)) };

                    lwmf::Line(ScreenTexture, ParticlesArray[a].x, ParticlesArray[a].y, ParticlesArray[b].x, ParticlesArray[b].y, lwmf::RGBAtoINT(Opaque, 0, 0, Opaque));
                }
            }
        }
    }

	inline void Draw()
	{

        if (ScreenTexture.Width != Wallpaper.Width || ScreenTexture.Height != Wallpaper.Height)
        {
            lwmf::ResizeTexture(Wallpaper, ScreenTexture.Width, ScreenTexture.Height, lwmf::FilterModes::BILINEAR);
        }

        lwmf::BlitTexture(Wallpaper, ScreenTexture, 0, 0);

        for (std::int_fast32_t i{}; i < ParticlesArray.size(); ++i)
        {
            ParticlesArray[i].Update();
        }

        Connect();

        for (std::int_fast32_t i{}; i < ParticlesArray.size(); ++i)
        {
            ParticlesArray[i].Draw();
        }

		DisplayInfoBox("Particles");
	}


} // namespace Particles
