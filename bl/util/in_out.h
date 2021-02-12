#pragma once
#include <iostream>

namespace bl
{
	void print();

	template<typename t_value, typename ...t_args>
	void print(const t_value& value, t_args&& ...args)
	{
		std::cout << value << ' ';
		print(std::forward<t_args>(args)...);
	}

	void wait_enter();
} // namespace bl
