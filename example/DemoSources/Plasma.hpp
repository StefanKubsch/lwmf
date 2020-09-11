#pragma once

#include <cstdint>
#include <cmath>

namespace Plasma
{


    inline void Draw()
    {
        static float TimeElapsed{};
        TimeElapsed += 0.0166F;

        #pragma omp parallel for
        for (std::int_fast32_t y{}; y < ScreenTexture.Height; y += 2)
        {
            const float TempY{ (0.5F + y / static_cast<float>(ScreenTexture.Height) - 1.0F) * 20.0F - 10.0F };

            for (std::int_fast32_t x{}; x < ScreenTexture.Width; x += 2)
            {
                const float TempX{ (0.5F + x / static_cast<float>(ScreenTexture.Width) - 1.0F) * 20.0F - 10.0F };
                const float cx{ TempX + 10.0F * (std::sinf(TimeElapsed * 0.33F)) };
                const float cy{ TempY + 10.0F * (std::cosf(TimeElapsed * 0.5F)) };
                const float Value{ (std::sinf(TempY + TimeElapsed) + std::sinf((TempX + TimeElapsed) * 0.5F) + std::sinf((TempX + TempY + TimeElapsed) * 0.5F) + ((std::sinf(std::sqrtf(cx * cx + cy * cy + 1.0F) + TimeElapsed)) * 0.5F)) * lwmf::PI };

                const std::int_fast32_t Red{ static_cast<std::int_fast32_t>((std::sinf(Value) * 0.5F + 0.5F) * 255.0F) };
                const std::int_fast32_t Green{ static_cast<std::int_fast32_t>((std::sinf(Value + lwmf::DoublePI * 0.33F) * 0.5F + 0.5F) * 255.0F) };
                const std::int_fast32_t Blue{ static_cast<std::int_fast32_t>((std::sinf(Value + 4.0F * lwmf::PI * 0.33F) * 0.5F + 0.5F) * 255.0F) };

                lwmf::SetPixel(ScreenTexture, x, y, lwmf::RGBAtoINT(Red, Green, Blue, 255));
            }
        }

        DisplayInfoBox("OpenMP accelerated RGB plasma");
    }


} // namespace Plasma
