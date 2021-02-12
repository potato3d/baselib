#pragma once
#include <bl/algebra/math.h>

namespace bl
{
	class vec2
	{
	public:
		inline vec2();
		inline vec2(const float vx, const float vy);

		inline bool operator==(const vec2& other) const;

		inline vec2 operator-(const vec2& other) const;

		float x;
		float y;
	};

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	vec2::vec2()
	{
	}

	vec2::vec2(const float vx, const float vy)
		: x(vx), y(vy)
	{
	}

	bool vec2::operator==(const vec2& other) const
	{
		return equal(x, other.x) && equal(y, other.y);
	}

	vec2 vec2::operator-(const vec2& other) const
	{
		return vec2(x-other.x, y-other.y);
	}
} // namespace bl
