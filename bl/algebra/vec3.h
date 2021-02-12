#pragma once
#include <bl/algebra/math.h>
#include <ostream>

namespace bl
{
	class vec3
	{
	public:
		static const vec3 UNIT_X;
		static const vec3 UNIT_Y;
		static const vec3 UNIT_Z;

		inline vec3();
		inline vec3(const float vx, const float vy, const float vz);

		inline vec3& operator+=(const vec3& other);
		inline vec3& operator-=(const vec3& other);

		inline vec3 operator-() const;
		inline vec3 operator-(const vec3& other) const;
		inline vec3 operator*(const float s) const;

		inline float length() const;
		inline float length_squared() const;

		inline vec3 normalized() const;

		inline vec3 cross(const vec3& other) const;

		float x;
		float y;
		float z;
	};

	std::ostream& operator<<(std::ostream& o, const vec3& v);

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	vec3::vec3()
	{
	}

	vec3::vec3(const float vx, const float vy, const float vz)
		: x(vx), y(vy), z(vz)
	{
	}

	vec3& vec3::operator+=(const vec3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	vec3& vec3::operator-=(const vec3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	vec3 vec3::operator-() const
	{
		return vec3(-x, -y, -z);
	}

	vec3 vec3::operator-(const vec3& other) const
	{
		return vec3(x-other.x, y-other.y, z-other.z);
	}

	vec3 vec3::operator*(const float s) const
	{
		return vec3(x*s, y*s, z*s);
	}

	float vec3::length() const
	{
		return sqrt(length_squared());
	}

	float vec3::length_squared() const
	{
		return x*x + y*y + z*z;
	}

	vec3 vec3::normalized() const
	{
		const float len_sqr = length_squared();
		if(!equal(len_sqr, 0.0f))
		{
			const float inv_len_sqr = 1.0f / sqrt(len_sqr);
			return vec3(x*inv_len_sqr,
						y*inv_len_sqr,
						z*inv_len_sqr);
		}
		else
		{
			return vec3(0.0f, 0.0f, 0.0f);
		}
	}

	vec3 vec3::cross(const vec3& other) const
	{
		return vec3(y*other.z - z*other.y, z*other.x - x*other.z, x*other.y - y*other.x);
	}
} // namespace bl
