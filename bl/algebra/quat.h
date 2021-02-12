#pragma once
#include <bl/algebra/vec3.h>

namespace bl
{
	class quat
	{
	public:
		inline quat();
		inline quat(float qx, float qy, float qz, float qw);
		inline quat(const float angle, const vec3& axis);
		inline quat(const float radians_u, const vec3& axis_u,
					const float radians_v, const vec3& axis_v,
					const float radians_w, const vec3& axis_w);

		inline quat mul(const quat& other) const;

		float x;
		float y;
		float z;
		float w;
	};

	quat::quat()
	{
	}

	quat::quat(float qx, float qy, float qz, float qw)
		: x(qx), y(qy), z(qz), w(qw)
	{
	}

	quat::quat(const float radians, const vec3& axis)
	{
		float length = axis.length_squared();

		if(equal(length, 0.0f))
		{
			// zero length axis, so return zero rotation
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 1.0f;
		}

		// get length from squared length
		length = sqrt(length);

		const float inv_norm = 1.0f / length;
		const float cos_half_radians = cos(0.5f * radians);
		const float sin_half_radians = sin(0.5f * radians);

		x = axis.x * sin_half_radians * inv_norm;
		y = axis.y * sin_half_radians * inv_norm;
		z = axis.z * sin_half_radians * inv_norm;
		w = cos_half_radians;
	}

	quat::quat(const float radians_u, const vec3& axis_u,
			   const float radians_v, const vec3& axis_v,
			   const float radians_w, const vec3& axis_w)
	{
		const quat q = quat(radians_u, axis_u).mul(quat(radians_v, axis_v)).mul(quat(radians_w, axis_w));
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
	}

	quat quat::mul(const quat& other) const
	{
		// Source: http://www.cprogramming.com/tutorial/3d/quaternions.html
		return quat(w*other.x + x*other.w + y*other.z - z*other.y,
					w*other.y - x*other.z + y*other.w + z*other.x,
					w*other.z + x*other.y - y*other.x + z*other.w,
					w*other.w - x*other.x - y*other.y - z*other.z);
	}
} // namespace bl
