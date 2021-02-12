#pragma once
#include <bl/util/partition.h>
#include <bl/select/median_of_3.h>
#include <bl/sort/insertion.h>

// ref: http://en.wikipedia.org/wiki/Quicksort#In-place_version
namespace bl
{
	template<typename t_value, typename t_size>
	void quick_sort(t_value*__restrict__ const a, const t_size left, const t_size right)
	{
		if(right - left > 32)
		{
			const t_size pivotIndex = median_of_3(a, left, right);
			const t_size pivotNewIndex = partition(a, left, right, pivotIndex);
			quick_sort(a, left, pivotNewIndex-1);
			quick_sort(a, pivotNewIndex+1, right);
		}
		else
		{
			insertion_sort(a+left, right-left+1);
		}
	}
} // namespace bl
