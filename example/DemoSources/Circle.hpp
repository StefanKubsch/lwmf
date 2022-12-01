#pragma once

#include <cstdint>
#include <cmath>

namespace Circle
{


    inline void Draw()
    {
        static float t{};
        static lwmf::PerlinNoise Ring;
        float j{};

        lwmf::ClearTexture(Canvas, 0x00000000);

        for (std::int_fast32_t i{}; i < 100; ++i)
        {
            const float Time{ t + (j * 16.0F) };
            const float sinTime{ std::sinf((Time + 270.0F) * lwmf::RAD2DEG) };
            const float TimeFactor{ Time * lwmf::RAD2DEG };
            const float cosTimeFactor{ std::cosf(TimeFactor) };
            const float sinTimeFactor{ std::sinf(TimeFactor) };
            const std::int_fast32_t ColorMod{ static_cast<std::int_fast32_t>((j * j) * 100.0F) };
            float b{};

            for (std::int_fast32_t a{}; a < 1800; ++a)
            {
                const float aFactor{ b * lwmf::RAD2DEG };
                const float cosaFactor{ std::cosf(aFactor) };
                const float sinaFactor{ std::sinf(aFactor) };

                const float RoundNoiseFactor{ 70.0F + Ring.Noise(Canvas.Width + cosTimeFactor + cosaFactor, Canvas.Height + sinTimeFactor + sinaFactor, 0.0F) * 150.0F * (1.0F + sinTime) };

                lwmf::SetPixelSafe(Canvas,
                    static_cast<std::int_fast32_t>(Canvas.WidthMid + (cosaFactor * RoundNoiseFactor)),
                    static_cast<std::int_fast32_t>(Canvas.HeightMid + (sinaFactor * RoundNoiseFactor)),
                    lwmf::RGBAtoINT(static_cast<std::int_fast32_t>(b + Time) % 360 - (ColorMod << 1), static_cast<std::int_fast32_t>(b + Time) % 100 - ColorMod, static_cast<std::int_fast32_t>(b + Time) % 270, 255));

                b += 0.2F;
            }

            j += 0.01F;
        }

        t = static_cast<float>(static_cast<std::int_fast32_t>(t += 1.0F) % 360);

        DisplayInfoBox("Perlin Noise Circle/Band");
    }


} // namespace Circle
