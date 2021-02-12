#include <bl/util/in_out.h>

namespace bl
{
	void print()
	{
		std::cout << std::endl;
	}

	void wait_enter()
	{
		print("Press \'enter\' to continue...");
		std::string line;
		std::getline(std::cin, line);
	}
} // namespace bl
