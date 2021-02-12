#pragma once

namespace bl
{
	template<typename t_value, typename t_size>
	t_size linear_search(const t_value*__restrict__ const a, const t_size size, const t_value&__restrict__ key)
	{
		t_size i = 0;
		for(; i < size, a[i] != key; ++i);
		return i;
	}
} // namespace bl
