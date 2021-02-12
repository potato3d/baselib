#pragma once
#include <bl/util/integer.h>
#include <cmath>
#include <limits>

namespace bl
{
	extern const float PI;
	extern const float TWO_PI;
	extern const float HALF_PI;
	extern const float INV_PI;

	using std::abs;
	using std::cos;
	using std::cosh;
	using std::sin;
	using std::sinh;
	using std::sqrt;
	using std::tan;

	template<typename t_number>
	t_number limit_min(t_number a)
	{
		return std::numeric_limits<t_number>::min();
	}

	template<typename t_number>
	t_number limit_max(t_number a)
	{
		return std::numeric_limits<t_number>::max();
	}

	inline float to_radians(const float degrees)
	{
		return degrees * 0.017453292519943295769236907684886f;
	}

	inline float to_degrees(const float radians)
	{
		return radians * 57.295779513082320876798154814105f;
	}

	inline bool is_infinite(float a)
	{
		return std::isinf(a);
	}

	inline bool equal(float a, float b)
	{
		const int32 max_ulps = 1;

		const bool a_inf = is_infinite(a);
		const bool b_inf = is_infinite(b);

		if(a_inf || b_inf)
		{
			if(a_inf && b_inf)
			{
				if((a > 0.0f && b > 0.0f) || (a < 0.0f && b < 0.0f))
				{
					return true;
				}
			}
			return false;
		}

		// Make sure maxUlps is non-negative and small enough that the
		// default NAN won't compare as equal to anything.
		//assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);
		union
		{
			float a_float;
			int32 a_int;
		};

		union
		{
			float b_float;
			int32 b_int;
		};

		a_float = a;
		b_float = b;

		// Make a_int lexicographically ordered as a twos-complement int
		if(a_int < 0)
		{
			a_int = static_cast<int32>(0x80000000) - a_int;
		}

		// Make b_int lexicographically ordered as a twos-complement int
		if(b_int < 0)
		{
			b_int = static_cast<int32>(0x80000000) - b_int;
		}

		int32 int_diff = abs(a_int - b_int);
		return int_diff <= max_ulps;
	}
} // namespace bl
