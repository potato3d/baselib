#include <bl/algebra/vec3.h>

namespace bl
{
	const vec3 vec3::UNIT_X(1.0f, 0.0f, 0.0f);
	const vec3 vec3::UNIT_Y(0.0f, 1.0f, 0.0f);
	const vec3 vec3::UNIT_Z(0.0f, 0.0f, 1.0f);

	std::ostream& operator<<(std::ostream& o, const vec3& v)
	{
		o << "[" << v.x << ", " << v.y << ", " << v.z << "]";
		return o;
	}
} // namespace bl
