#pragma once
#include <bl/util/algorithm.h>

namespace bl
{
	// ref: http://ndevilla.free.fr/median/median.pdf
	template<typename t_value, typename t_size>
	t_value median_of_3(const t_value*__restrict__ const a, t_size left, t_size right)
	{
		t_size middle = left + (right-left)/2;
		if(a[left] > a[middle])
		{
			swap(left, middle);
		}
		if(a[middle] > a[right])
		{
			swap(middle, right);
		}
		if(a[left] > a[middle])
		{
			swap(left, middle);
		}
		return middle;
	}
} // namespace bl
