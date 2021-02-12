#pragma once

namespace bl
{
	class vec4
	{
	public:
		inline vec4();
		inline vec4(float vx, float vy, float vz, float vw);

		float x;
		float y;
		float z;
		float w;
	};

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	vec4::vec4()
	{
	}

	vec4::vec4(float vx, float vy, float vz, float vw)
		: x(vx), y(vy), z(vz), w(vw)
	{
	}
} // namespace bl
