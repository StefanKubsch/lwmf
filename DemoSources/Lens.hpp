#pragma once

#include <cstdint>
#include <vector>
#include <cmath>

namespace Lens
{


	inline lwmf::BitmapStruct Wallpaper;
	inline lwmf::IntPointStruct LensPos{};
	inline std::int_fast32_t LensWidth{ 250 };
	inline std::vector<std::vector<std::int_fast32_t>> Lens(LensWidth, std::vector<std::int_fast32_t>(LensWidth));
	inline std::int_fast32_t XDir{};
	inline std::int_fast32_t YDir{};
	inline std::int_fast32_t OldViewPortWidth{};
	inline std::int_fast32_t OldViewPortHeight{};

	inline void Init()
	{
		OldViewPortWidth = lwmf::ViewportWidth;
		OldViewPortHeight = lwmf::ViewportHeight;

		LensPos.X = 16;
		LensPos.Y = 16;
		XDir = 4;
		YDir = 4;

		lwmf::LoadBMP(Wallpaper, "./DemoGFX/Head.bmp");

		if (lwmf::ViewportWidth != Wallpaper.Width || lwmf::ViewportHeight != Wallpaper.Height)
		{
			lwmf::ResizeBMP(Wallpaper, lwmf::ViewportWidth, lwmf::ViewportHeight, lwmf::BILINEAR);
			LensWidth = (lwmf::ViewportWidth * lwmf::ViewportHeight) / 2000;

			if (LensWidth > 250)
			{
				LensWidth = 250;
			}
			else if (LensWidth < 2)
			{
				LensWidth = 2;
			}
		}

		const std::int_fast32_t LensRadius{ LensWidth >> 1 };
		const std::int_fast32_t LensRadiusPOW{ LensRadius * LensRadius };
		constexpr std::int_fast32_t LensZoom{ 40 };
		constexpr std::int_fast32_t LensZoomPOW{ LensZoom * LensZoom };

		for (std::int_fast32_t y{}; y < LensRadius; ++y)
		{
			for (std::int_fast32_t x{}; x < LensRadius; ++x)
			{
				lwmf::IntPointStruct TempPos;
				const std::int_fast32_t Temp{ x * x + y * y };

				if (Temp < LensRadiusPOW)
				{
					const float Shift{ LensZoom / std::sqrtf(static_cast<float>(LensZoomPOW - (Temp - LensRadiusPOW))) };
					TempPos.X = static_cast<std::int_fast32_t>(x * Shift - x);
					TempPos.Y = static_cast<std::int_fast32_t>(y * Shift - y);
				}

				std::int_fast32_t Offset{ (TempPos.Y * lwmf::ViewportWidth + TempPos.X) };
				Lens[LensRadius - y][LensRadius - x] = -Offset;
				Lens[LensRadius + y][LensRadius + x] = Offset;
				Offset = (-TempPos.Y * lwmf::ViewportWidth + TempPos.X);
				Lens[LensRadius + y][LensRadius - x] = -Offset;
				Lens[LensRadius - y][LensRadius + x] = Offset;
			}
		}
	}

	inline void Draw()
	{
		if (OldViewPortWidth != lwmf::ViewportWidth || OldViewPortHeight != lwmf::ViewportHeight)
		{
			Init();
		}

		lwmf::BlitBMP(Wallpaper, 0, 0);

		for (std::int_fast32_t y{}; y < LensWidth; ++y)
		{
			const std::int_fast32_t Temp{ (y + LensPos.Y) * lwmf::ViewportWidth + LensPos.X };

			for (std::int_fast32_t x{}; x < LensWidth; ++x)
			{
				lwmf::PixelBuffer[Temp + x] = Wallpaper.BitmapData[Temp + x + Lens[y][x]];
			}
		}

		LensPos.X += XDir;
		LensPos.Y += YDir;

		if (LensPos.X > (Wallpaper.Width - LensWidth - 5) || LensPos.X < 5)
		{
			XDir *= -1;
		}

		if (LensPos.Y > (Wallpaper.Height - LensWidth - 5) || LensPos.Y < 5)
		{
			YDir *= -1;
		}

		lwmf::RenderText("Realtime lens", 10, 10, 0xFFFFFFFF);
	}


} // namespace Lens