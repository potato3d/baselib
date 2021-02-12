#pragma once
#include <bl/util/string.h>
#include <stdexcept>

namespace bl
{
	class exception : public std::exception
	{
	public:
		exception(const char* title, const char* details, const char* file, const char* function, int line);
		const char* getTitle() const;
		const char* what() const throw();

	private:
		string _title;
		string _what;
	};

	#define BL_THROW(title, details) throw exception(title, details, __FILE__, __PRETTY_FUNCTION__, __LINE__);
} // namespace bl
