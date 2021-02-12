#pragma once
#include <bl/util/algorithm.h>

namespace bl
{
	// ref: http://en.wikipedia.org/wiki/Quicksort#In-place_version
	template<typename t_value, typename t_size>
	t_size partition(t_value*__restrict__ const a, const t_size left, const t_size right, const t_size pivotIdx)
	{
		const t_value pivot = a[pivotIdx];
		swap(a[pivotIdx], a[right]);
		t_size storeIndex = left;
		for(t_size i = left; i < right; ++i)
		{
			if(pivot > a[i])
			{
				swap(a[i], a[storeIndex]);
				++storeIndex;
			}
		}
		swap(a[storeIndex], a[right]);
		return storeIndex;
	}
} // namespace bl
