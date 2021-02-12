#pragma once
#include <future>

namespace bl
{
	using std::future;

	template<typename t_function, typename ...t_args>
	auto async(t_function function, t_args&& ...args) -> decltype(std::async(std::launch::async, function, std::forward<t_args>(args)...))
	{
		return std::async(std::launch::async, function, std::forward<t_args>(args)...);
	}
} // namespace bl
