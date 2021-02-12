#pragma once
#include <chrono>
#include <type_traits>

namespace bl
{
	class timer
	{
	public:
		timer();
		void restart();
		double seconds();
		double milliseconds();
		double microseconds();
		double nanoseconds();

	private:
		typedef typename std::conditional<std::chrono::high_resolution_clock::is_steady,
										  std::chrono::high_resolution_clock,
										  std::chrono::steady_clock>::type clock_type;
		clock_type::time_point _start;
	};
} // namespace bl
