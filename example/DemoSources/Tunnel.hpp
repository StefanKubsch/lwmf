#pragma once

#include <cstdint>
#include <vector>
#include <cmath>

namespace Tunnel
{


	inline lwmf::TextureStruct Texture{};
	inline std::int_fast32_t TunnelScreenWidth{};
	inline std::int_fast32_t TunnelScreenHeight{};
	inline std::int_fast32_t TunnelWidth{};
	inline std::int_fast32_t TunnelHeight{};
	inline std::vector<std::vector<std::int_fast32_t>> Distance{};
	inline std::vector<std::vector<std::int_fast32_t>> Angle{};

	inline void Init()
	{
		lwmf::LoadPNG(Texture, "./DemoGFX/TunnelTexture.png");

		TunnelScreenWidth = ScreenTexture.Width;
		TunnelScreenHeight = ScreenTexture.Height;
		TunnelWidth = TunnelScreenWidth >> 1;
		TunnelHeight = TunnelScreenHeight >> 1;
		Distance.clear();
		Distance.shrink_to_fit();
		Distance.resize(static_cast<std::size_t>(TunnelScreenWidth) << 1, std::vector<std::int_fast32_t>(static_cast<std::size_t>(TunnelScreenHeight) << 1));
		Angle.clear();
		Angle.shrink_to_fit();
		Angle.resize(static_cast<std::size_t>(TunnelScreenWidth) << 1, std::vector<std::int_fast32_t>(static_cast<std::size_t>(TunnelScreenHeight) << 1));

		for (std::int_fast32_t x{}; x < TunnelScreenWidth << 1; ++x)
		{
			const float Temp1{ 0.5F * static_cast<float>(Texture.Width) };

			for (std::int_fast32_t y{}; y < TunnelScreenHeight << 1; ++y)
			{
				Distance[x][y] = static_cast<std::int_fast32_t>((32.0F * Texture.Width / std::sqrtf(static_cast<float>((x - TunnelScreenWidth) * (x - TunnelScreenWidth) + (y - TunnelScreenHeight) * (y - TunnelScreenHeight))))) & (Texture.Width - 1);
				const float AngleTemp{ Temp1 * lwmf::FastAtan2Approx(static_cast<float>(y - TunnelScreenHeight), static_cast<float>(x - TunnelScreenWidth)) / lwmf::PI };
				Angle[x][y] = static_cast<std::int_fast32_t>((256.0F - AngleTemp)) & 255;
			}
		}
	}

	inline void Draw()
	{
		static std::int_fast32_t OldViewPortWidth{};
		static std::int_fast32_t OldViewPortHeight{};

		if (OldViewPortWidth != ScreenTexture.Width || OldViewPortHeight != ScreenTexture.Height)
		{
			OldViewPortWidth = ScreenTexture.Width;
			OldViewPortHeight = ScreenTexture.Height;
			Init();
			lwmf::ClearTexture(ScreenTexture, 0x00000000);
		}

		static float Anim{};
		Anim += 0.004F;

		const std::int_fast32_t SpeedFactor{ static_cast<std::int_fast32_t>(Texture.Width * 2.15F * Anim) };
		const lwmf::IntPointStruct Source{ static_cast<std::int_fast32_t>((TunnelWidth * std::cosf(Anim * 4.0F)) + TunnelWidth), static_cast<std::int_fast32_t>((TunnelHeight * std::sinf(Anim * 6.0F)) + TunnelHeight) };

		#pragma omp parallel for
		for (std::int_fast32_t y{}; y < TunnelScreenHeight; ++y)
		{
			for (std::int_fast32_t x{}; x < TunnelScreenWidth; ++x)
			{
				lwmf::SetPixel(ScreenTexture, x, y, Texture.Pixels[((Angle[x + Source.X][y + Source.Y] + SpeedFactor) & 255) * Texture.Width + ((Distance[x + Source.X][y + Source.Y] + SpeedFactor) & 255)]);
			}
		}

		DisplayInfoBox("OpenMP accelerated textured tunnel");
	}


} // namespace Tunnel
