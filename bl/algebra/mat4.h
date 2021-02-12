#pragma once
#include <bl/algebra/quat.h>
#include <bl/algebra/vec3.h>
#include <algorithm>

namespace bl
{
	class mat4
	{
	public:
		static const mat4 IDENTITY;

		inline static mat4 translation(const vec3& t);
		inline static mat4 rotation(const float radians_u, const vec3& axis_u,
									const float radians_v, const vec3& axis_v,
									const float radians_w, const vec3& axis_w);
		inline static mat4 look_at(const vec3& eye, const vec3& center, const vec3& up);
		inline static mat4 perspective(const float fov_y, const float aspect, const float z_near, const float z_far);

		inline mat4();
		inline explicit mat4(const float* const p);
		inline explicit mat4(const quat& q);
		inline mat4(const float a00, const float a01, const float a02, const float a03,
					const float a10, const float a11, const float a12, const float a13,
					const float a20, const float a21, const float a22, const float a23,
					const float a30, const float a31, const float a32, const float a33);

		inline float& at(int row, int column);
		inline const float& at(int row, int column) const;

		inline float* ptr();
		inline const float* ptr() const;

		inline mat4 mul(const mat4& other) const;
		inline vec3 mul(const vec3& v) const;

		inline mat4 transpose() const;
		inline mat4 inverse() const;

	private:
		float _data[16]; // column-major
	};

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	std::ostream& operator<<(std::ostream& o, const mat4& m);

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	mat4 mat4::translation(const vec3& t)
	{
		return mat4(1.0f, 0.0f, 0.0f, t.x,
					0.0f, 1.0f, 0.0f, t.y,
					0.0f, 0.0f, 1.0f, t.z,
					0.0f, 0.0f, 0.0f, 1.0f);
	}

	mat4 mat4::rotation(const float radians_u, const vec3& axis_u,
						const float radians_v, const vec3& axis_v,
						const float radians_w, const vec3& axis_w)
	{
		return mat4(quat(radians_u, axis_u,
						 radians_v, axis_v,
						 radians_w, axis_w));
	}

	mat4 mat4::look_at(const vec3& eye, const vec3& center, const vec3& up)
	{
		// source: http://www.opengl.org/wiki/GluLookAt_code
		vec3 ze((center - eye).normalized());
		vec3 xe(ze.cross(up).normalized());
		vec3 ye(xe.cross(ze).normalized());

		return mat4( xe.x, xe.y, xe.z, 0.0f,
					 ye.x, ye.y, ye.z, 0.0f,
					-ze.x,-ze.y,-ze.z, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f).mul(mat4::translation(-eye));
	}

	mat4 mat4::perspective(const float fov_y, const float aspect, const float z_near, const float z_far)
	{
		const float cotan_fov = 1.0f / tan(fov_y*0.5f);
		return mat4(cotan_fov/aspect,      0.0f,                           0.0f,                               0.0f,
								0.0f, cotan_fov,                           0.0f,                               0.0f,
								0.0f,      0.0f, -(z_far+z_near)/(z_far-z_near), (2.0f*z_far*z_near)/(z_near-z_far),
								0.0f,      0.0f,                          -1.0f,                              0.0f);
	}

	mat4::mat4()
	{
	}

	mat4::mat4(const float* const p)
	{
		std::copy_n(p, 16, _data);
	}

	mat4::mat4(const quat& q)
	{
		// source: http://www.gamasutra.com/features/19980703/quaternions_01.htm
		const float x2 = q.x + q.x;
		const float y2 = q.y + q.y;
		const float z2 = q.z + q.z;

		const float xx = q.x * x2;
		const float xy = q.x * y2;
		const float xz = q.x * z2;

		const float yy = q.y * y2;
		const float yz = q.y * z2;
		const float zz = q.z * z2;

		const float wx = q.w * x2;
		const float wy = q.w * y2;
		const float wz = q.w * z2;

		at(0,0) = 1.0f-(yy+zz); at(0,1) =        xy-wz; at(0,2) =        xz+wy; at(0,3) = 0.0f;
		at(1,0) = 	     xy+wz; at(1,1) = 1.0f-(xx+zz); at(1,2) =        yz-wx; at(1,3) = 0.0f;
		at(2,0) = 	     xz-wy; at(2,1) =        yz+wx; at(2,2) = 1.0f-(xx+yy); at(2,3) = 0.0f;
		at(3,0) =         0.0f; at(3,1) =         0.0f; at(3,2) =         0.0f; at(3,3) = 1.0f;
	}

	mat4::mat4(const float a00, const float a01, const float a02, const float a03,
			   const float a10, const float a11, const float a12, const float a13,
			   const float a20, const float a21, const float a22, const float a23,
			   const float a30, const float a31, const float a32, const float a33)
	{
		at(0,0) = a00; at(0,1) = a01; at(0,2) = a02; at(0,3) = a03;
		at(1,0) = a10; at(1,1) = a11; at(1,2) = a12; at(1,3) = a13;
		at(2,0) = a20; at(2,1) = a21; at(2,2) = a22; at(2,3) = a23;
		at(3,0) = a30; at(3,1) = a31; at(3,2) = a32; at(3,3) = a33;
	}

	float& mat4::at(int row, int column)
	{
		return _data[column*4 + row];
	}

	const float& mat4::at(int row, int column) const
	{
		return _data[column*4 + row];
	}

	float* mat4::ptr()
	{
		return _data;
	}

	const float* mat4::ptr() const
	{
		return _data;
	}

	mat4 mat4::mul(const mat4& other) const
	{
		const float a00 = at(0,0), a01 = at(0,1), a02 = at(0,2), a03 = at(0,3);
		const float a10 = at(1,0), a11 = at(1,1), a12 = at(1,2), a13 = at(1,3);
		const float a20 = at(2,0), a21 = at(2,1), a22 = at(2,2), a23 = at(2,3);
		const float a30 = at(3,0), a31 = at(3,1), a32 = at(3,2), a33 = at(3,3);

		const float b00 = other.at(0,0), b01 = other.at(0,1), b02 = other.at(0,2), b03 = other.at(0,3);
		const float b10 = other.at(1,0), b11 = other.at(1,1), b12 = other.at(1,2), b13 = other.at(1,3);
		const float b20 = other.at(2,0), b21 = other.at(2,1), b22 = other.at(2,2), b23 = other.at(2,3);
		const float b30 = other.at(3,0), b31 = other.at(3,1), b32 = other.at(3,2), b33 = other.at(3,3);

		return mat4(a00*b00+a01*b10+a02*b20+a03*b30, a00*b01+a01*b11+a02*b21+a03*b31, a00*b02+a01*b12+a02*b22+a03*b32, a00*b03+a01*b13+a02*b23+a03*b33,
					a10*b00+a11*b10+a12*b20+a13*b30, a10*b01+a11*b11+a12*b21+a13*b31, a10*b02+a11*b12+a12*b22+a13*b32, a10*b03+a11*b13+a12*b23+a13*b33,
					a20*b00+a21*b10+a22*b20+a23*b30, a20*b01+a21*b11+a22*b21+a23*b31, a20*b02+a21*b12+a22*b22+a23*b32, a20*b03+a21*b13+a22*b23+a23*b33,
					a30*b00+a31*b10+a32*b20+a33*b30, a30*b01+a31*b11+a32*b21+a33*b31, a30*b02+a31*b12+a32*b22+a33*b32, a30*b03+a31*b13+a32*b23+a33*b33);
	}

	vec3  mat4::mul(const vec3& v) const
	{
		const float w = 1.0f;
		const float d = 1.0f / (at(3,0)*v.x + at(3,1)*v.y + at(3,2)*v.z + at(3,3)*w);

		return vec3(d*(at(0,0)*v.x + at(0,1)*v.y + at(0,2)*v.z + at(0,3)*w),
					d*(at(1,0)*v.x + at(1,1)*v.y + at(1,2)*v.z + at(1,3)*w),
					d*(at(2,0)*v.x + at(2,1)*v.y + at(2,2)*v.z + at(2,3)*w));
	}

	mat4 mat4::transpose() const
	{
		return mat4(at(0,0), at(1,0), at(2,0), at(3,0),
					at(0,1), at(1,1), at(2,1), at(3,1),
					at(0,2), at(1,2), at(2,2), at(3,2),
					at(0,3), at(1,3), at(2,3), at(3,3));
	}

	mat4 mat4::inverse() const
	{
		// source: http://www.geometrictools.com/LibMathematics/Algebra/Wm5Matrix4.inl
		const float a0 = at(0,0) * at(1,1) - at(0,1) * at(1,0);
		const float a1 = at(0,0) * at(1,2) - at(0,2) * at(1,0);
		const float a2 = at(0,0) * at(1,3) - at(0,3) * at(1,0);
		const float a3 = at(0,1) * at(1,2) - at(0,2) * at(1,1);
		const float a4 = at(0,1) * at(1,3) - at(0,3) * at(1,1);
		const float a5 = at(0,2) * at(1,3) - at(0,3) * at(1,2);
		const float b0 = at(2,0) * at(3,1) - at(2,1) * at(3,0);
		const float b1 = at(2,0) * at(3,2) - at(2,2) * at(3,0);
		const float b2 = at(2,0) * at(3,3) - at(2,3) * at(3,0);
		const float b3 = at(2,1) * at(3,2) - at(2,2) * at(3,1);
		const float b4 = at(2,1) * at(3,3) - at(2,3) * at(3,1);
		const float b5 = at(2,2) * at(3,3) - at(2,3) * at(3,2);

		const float det = a0*b5 - a1*b4 + a2*b3 + a3*b2 - a4*b1 + a5*b0;

		if(!equal(det, 0.0f))
		{
			const float inv_det = 1.0f / det;

			mat4 inv;
			inv.at(0,0) = +at(1,1)*b5 - at(1,2)*b4 + at(1,3)*b3;
			inv.at(1,0) = -at(1,0)*b5 + at(1,2)*b2 - at(1,3)*b1;
			inv.at(2,0) = +at(1,0)*b4 - at(1,1)*b2 + at(1,3)*b0;
			inv.at(3,0) = -at(1,0)*b3 + at(1,1)*b1 - at(1,2)*b0;
			inv.at(0,1) = -at(0,1)*b5 + at(0,2)*b4 - at(0,3)*b3;
			inv.at(1,1) = +at(0,0)*b5 - at(0,2)*b2 + at(0,3)*b1;
			inv.at(2,1) = -at(0,0)*b4 + at(0,1)*b2 - at(0,3)*b0;
			inv.at(3,1) = +at(0,0)*b3 - at(0,1)*b1 + at(0,2)*b0;
			inv.at(0,2) = +at(3,1)*a5 - at(3,2)*a4 + at(3,3)*a3;
			inv.at(1,2) = -at(3,0)*a5 + at(3,2)*a2 - at(3,3)*a1;
			inv.at(2,2) = +at(3,0)*a4 - at(3,1)*a2 + at(3,3)*a0;
			inv.at(3,2) = -at(3,0)*a3 + at(3,1)*a1 - at(3,2)*a0;
			inv.at(0,3) = -at(2,1)*a5 + at(2,2)*a4 - at(2,3)*a3;
			inv.at(1,3) = +at(2,0)*a5 - at(2,2)*a2 + at(2,3)*a1;
			inv.at(2,3) = -at(2,0)*a4 + at(2,1)*a2 - at(2,3)*a0;
			inv.at(3,3) = +at(2,0)*a3 - at(2,1)*a1 + at(2,2)*a0;

			inv.at(0,0) *= inv_det;
			inv.at(0,1) *= inv_det;
			inv.at(0,2) *= inv_det;
			inv.at(0,3) *= inv_det;
			inv.at(1,0) *= inv_det;
			inv.at(1,1) *= inv_det;
			inv.at(1,2) *= inv_det;
			inv.at(1,3) *= inv_det;
			inv.at(2,0) *= inv_det;
			inv.at(2,1) *= inv_det;
			inv.at(2,2) *= inv_det;
			inv.at(2,3) *= inv_det;
			inv.at(3,0) *= inv_det;
			inv.at(3,1) *= inv_det;
			inv.at(3,2) *= inv_det;
			inv.at(3,3) *= inv_det;

			return inv;
		}
		else
		{
			return mat4(0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);
		}
	}
} // namespace bl
