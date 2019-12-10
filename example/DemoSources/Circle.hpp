#pragma once

#include <cstdint>
#include <cmath>

namespace Circle
{


    inline float CalcNoise(const float a, const float Time)
    {
        static lwmf::PerlinNoise Ring;
        const float TimeFactor{ Time * lwmf::RAD2DEG };
        const float aFactor{ a * lwmf::RAD2DEG };
        const float x{ ScreenTexture.Width + std::cosf(TimeFactor) + std::cosf(aFactor) };
        const float y{ ScreenTexture.Height + std::sinf(TimeFactor) + std::sinf(aFactor) };
        return Ring.Noise(x, y, 0.0F) * 150.0F * (1.0F + std::sinf((Time + 270.0F) * lwmf::RAD2DEG));
    }

    inline void DrawBand(const float Time, const std::int_fast32_t ColorMod)
    {
        for (float a{}; a < 360.0F; a += 0.1F) //-V1034
        {
            const float RoundNoiseFactor{ (70.0F + CalcNoise(a, Time)) };
            const float aFactor{ a * lwmf::RAD2DEG };

            lwmf::SetPixel(ScreenTexture,
                static_cast<std::int_fast32_t>(ScreenTexture.WidthMid + (std::cosf(aFactor) * RoundNoiseFactor)),
                static_cast<std::int_fast32_t>(ScreenTexture.HeightMid + (std::sinf(aFactor) * RoundNoiseFactor)),
                lwmf::RGBAtoINT(static_cast<std::int_fast32_t>(a + Time) % 360 - (ColorMod << 1), static_cast<std::int_fast32_t>(a + Time) % 100 - ColorMod, static_cast<std::int_fast32_t>(a + Time) % 270, 255));
        }
    }

    inline void Draw()
    {
        static float t{};

        lwmf::ClearTexture(ScreenTexture, 0);

        for (float ColorMod{}; ColorMod < 1.0F; ColorMod += 0.01F) //-V1034
        {
            DrawBand(t + (ColorMod * 16.0F), static_cast<std::int_fast32_t>((ColorMod * ColorMod) * 100.0F));
        }

        t += 1.0F;
        t = static_cast<float>(static_cast<std::int_fast32_t>(t) % 360);
    }


} // namespace Circle
