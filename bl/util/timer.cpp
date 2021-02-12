#include <bl/util/timer.h>

namespace bl
{
	timer::timer()
	{
		restart();
	}

	void timer::restart()
	{
		_start = clock_type::now();
	}

	double timer::seconds()
	{
		return nanoseconds() * 1e-9;
	}

	double timer::milliseconds()
	{
		return nanoseconds() * 1e-6;
	}

	double timer::microseconds()
	{
		return nanoseconds() * 1e-3;
	}

	double timer::nanoseconds()
	{
		return static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(clock_type::now() - _start).count());
	}
} // namespace bl
