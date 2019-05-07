/*
********************************************************************
*                                                                  *
* lwmf_math - lightweight media framework - math library           *
*                                                                  *
* (C) 2019 - present by Stefan Kubsch                              *
*                                                                  *
********************************************************************
*/

#pragma once

#include <cmath>

namespace lwmf
{


	inline const float PI{ std::atan(1.0F) * 4.0F };
	inline const float DoublePI{ PI * 2.0F };
	inline const float OneQrtPI{ PI / 4.0F };
	inline const float RAD2DEG{ PI / 180.0F };

	template<typename T>T Max(const T a, const T b)
	{
		return a < b ? b : a;
	}

	template<typename T>T Min(const T a, const T b)
	{
		return b < a ? b : a;
	}

	template<typename T>T Abs(const T Value)
	{
		return Value < 0 ? -Value : Value;
	}

	template<typename T>T Lerp(const T t, const T a, const T b)
	{
		return a + t * (b - a);
	}



} // namespace lwmf
