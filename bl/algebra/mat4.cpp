#include <bl/algebra/mat4.h>

namespace bl
{
	const mat4 mat4::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f,
							  0.0f, 1.0f, 0.0f, 0.0f,
							  0.0f, 0.0f, 1.0f, 0.0f,
							  0.0f, 0.0f, 0.0f, 1.0f);

	std::ostream& operator<<(std::ostream& o, const mat4& m)
	{
		o << "["
		  << m.at(0,0) << ", " << m.at(0,1) << ", " << m.at(0,2) << ", " << m.at(0,3) << ", "
		  << m.at(1,0) << ", " << m.at(1,1) << ", " << m.at(1,2) << ", " << m.at(1,3) << ", "
		  << m.at(2,0) << ", " << m.at(2,1) << ", " << m.at(2,2) << ", " << m.at(2,3) << ", "
		  << m.at(3,0) << ", " << m.at(3,1) << ", " << m.at(3,2) << ", " << m.at(3,3)
		  << "]";
		return o;
	}
} // namespace bl
