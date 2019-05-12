/*
********************************************************************
*                                                                  *
* lwmf_fpscounter - lightweight media framework - FPS counter      *
*                                                                  *
* (C) 2019 - present by Stefan Kubsch                              *
*                                                                  *
********************************************************************
*/

#pragma once

#include <cstdint>
#include <string>

namespace lwmf
{


	inline std::uint_fast32_t FPSUpdate{};
	inline std::int_fast32_t FPSFrames{};

	inline void DisplayFPSCounter(const std::int_fast32_t PosX, const std::int_fast32_t PosY, const std::int_fast32_t Color)
	{
		static std::int_fast32_t FPS{};

		if (const std::uint_fast32_t SystemTime{ GetTickCount() }; SystemTime - FPSUpdate >= 1000)
		{
			FPS = FPSFrames;
			FPSUpdate = SystemTime;
			FPSFrames = 0;
		}

		++FPSFrames;

		RenderText("fps:" + std::to_string(FPS), PosX, PosY, Color);
	}


} // namespace lwmf