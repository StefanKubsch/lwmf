#pragma once

#include <cstdint>
#include <vector>
#include <cmath>

namespace Lens
{


	inline static lwmf::TextureStruct Wallpaper{};
	inline static lwmf::IntPointStruct LensPos{};
	inline static lwmf::IntPointStruct Dir{};
	inline static std::int_fast32_t LensWidth{ 250 };
	inline static std::vector<std::vector<std::int_fast32_t>> Lens(LensWidth, std::vector<std::int_fast32_t>(static_cast<size_t>(LensWidth)));
	inline static std::int_fast32_t OldViewPortWidth{};
	inline static std::int_fast32_t OldViewPortHeight{};

	inline void Init()
	{
		OldViewPortWidth = ScreenTexture.Width;
		OldViewPortHeight = ScreenTexture.Height;

		LensPos = { 16, 16 };
		Dir = { 4, 4 };

		lwmf::LoadPNG(Wallpaper, "./DemoGFX/Head.png");

		if (ScreenTexture.Width != Wallpaper.Width || ScreenTexture.Height != Wallpaper.Height)
		{
			lwmf::ResizeTexture(Wallpaper, ScreenTexture.Width, ScreenTexture.Height, lwmf::FilterModes::BILINEAR);
			LensWidth = (ScreenTexture.Size) / 2000;

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
					TempPos = { static_cast<std::int_fast32_t>(x * Shift - x), static_cast<std::int_fast32_t>(y * Shift - y) };
				}

				std::int_fast32_t Offset{ (TempPos.Y * ScreenTexture.Width + TempPos.X) };
				Lens[LensRadius - y][LensRadius - x] = -Offset;
				Lens[LensRadius + y][LensRadius + x] = Offset;
				Offset = (-TempPos.Y * ScreenTexture.Width + TempPos.X);
				Lens[LensRadius + y][LensRadius - x] = -Offset;
				Lens[LensRadius - y][LensRadius + x] = Offset;
			}
		}
	}

	inline void Draw()
	{
		if (OldViewPortWidth != ScreenTexture.Width || OldViewPortHeight != ScreenTexture.Height)
		{
			Init();
		}

		lwmf::BlitTexture(Wallpaper, ScreenTexture, 0, 0);

		for (std::int_fast32_t y{}; y < LensWidth; ++y)
		{
			const std::int_fast32_t TempY{ (y + LensPos.Y) * ScreenTexture.Width + LensPos.X };

			for (std::int_fast32_t x{}; x < LensWidth; ++x)
			{
				ScreenTexture.Pixels[TempY + x] = Wallpaper.Pixels[TempY + x + Lens[y][x]];
			}
		}

		LensPos.X += Dir.X;
		LensPos.Y += Dir.Y;

		if (LensPos.X > (Wallpaper.Width - LensWidth - 5) || LensPos.X < 5)
		{
			Dir.X *= -1;
		}

		if (LensPos.Y > (Wallpaper.Height - LensWidth - 5) || LensPos.Y < 5)
		{
			Dir.Y *= -1;
		}
	}


} // namespace Lens