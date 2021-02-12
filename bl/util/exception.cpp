#include <bl/util/exception.h>

namespace bl
{
	exception::exception(const char* title, const char* details, const char* file, const char* function, int line)
		: _title(title), _what(str("%0\n%1(%2) %3", details, file, line, function))
	{
	}

	const char* exception::getTitle() const
	{
		return _title.data();
	}

	const char* exception::what() const throw()
	{
		return _what.data();
	}
} // namespace bl
