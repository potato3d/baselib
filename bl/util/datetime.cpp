#include <bl/util/datetime.h>
#include <chrono>

namespace bl
{
	string datetime::date_str()
	{
		std::time_t t = std::time(NULL);
		char s[64];
		std::strftime(s, 63, "%d/%m/%Y", std::localtime(&t));
		return s;
	}

	string datetime::time_str()
	{
		auto p = std::chrono::high_resolution_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(p.time_since_epoch());
		auto s = std::chrono::duration_cast<std::chrono::seconds>(ms);
		auto t = static_cast<std::time_t>(s.count());
		auto fractional_seconds = ms.count() % 1000;
		char stime[64];
		std::strftime(stime, 63, "%H:%M:%S:", std::localtime(&t));
		return stime + str(fractional_seconds);
	}
} //namespace bl
