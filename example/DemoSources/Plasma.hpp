#pragma once

#include <cstdint>
#include <cmath>

namespace Plasma
{


    inline void Draw()
    {
        static float TimeElapsed{};
        TimeElapsed += 0.0166F;
        const float cosTimeElapsed{ std::cosf(TimeElapsed * 0.5F) };
        const float sinTimeElapsed{ 10.0F * (std::sinf(TimeElapsed * 0.33F)) };

        #pragma omp parallel for
        for (std::int_fast32_t y{}; y < Canvas.Height; y += 2)
        {
            const float TempY{ (0.5F + y / static_cast<float>(Canvas.Height) - 1.0F) * 20.0F - 10.0F };
            const float cy{ TempY + 10.0F * cosTimeElapsed };

            for (std::int_fast32_t x{}; x < Canvas.Width; x += 2)
            {
                const float TempX{ (0.5F + x / static_cast<float>(Canvas.Width) - 1.0F) * 20.0F - 10.0F };
                const float cx{ TempX + sinTimeElapsed };
                const float Value{ (std::sinf(TempY + TimeElapsed) + std::sinf((TempX + TimeElapsed) * 0.5F) + std::sinf((TempX + TempY + TimeElapsed) * 0.5F) + ((std::sinf(std::sqrtf(cx * cx + cy * cy + 1.0F) + TimeElapsed)) * 0.5F)) * lwmf::PI };

                lwmf::SetPixel(Canvas, x, y,
                    lwmf::RGBAtoINT(static_cast<std::int_fast32_t>((std::sinf(Value) * 0.5F + 0.5F) * 255.0F),
                                    static_cast<std::int_fast32_t>((std::sinf(Value + lwmf::DoublePI * 0.33F) * 0.5F + 0.5F) * 255.0F),
                                    static_cast<std::int_fast32_t>((std::sinf(Value + 4.0F * lwmf::PI * 0.33F) * 0.5F + 0.5F) * 255.0F), 255));
            }
        }

        DisplayInfoBox("OpenMP accelerated RGB plasma");
    }


} // namespace Plasma
