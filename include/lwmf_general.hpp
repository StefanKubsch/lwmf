/*
******************************************************
*                                                    *
* lwmf_general - lightweight media framework         *
*                                                    *
* (C) 2019 - present by Stefan Kubsch                *
*                                                    *
******************************************************
*/

#pragma once

#include <cstdint>

namespace lwmf
{


	struct IntPointStruct final
	{
		std::int_fast32_t x{};
		std::int_fast32_t y{};
	};

	struct FloatPointStruct final
	{
		float x{};
		float y{};
	};


} // namespace lwmf