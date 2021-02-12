#pragma once
#include <bl/util/algorithm.h>

// ref: http://en.wikipedia.org/wiki/Cocktail_sort
namespace bl
{
	template<typename t_value, typename t_size>
	void cocktail_sort(t_value*__restrict__ const a, const t_size size)
	{
		// `begin` and `end` marks the first and last index to check
		t_size begin = 0;
		t_size end = size - 2;
		bool swapped = false;
		do
		{
			swapped = false;
			for(t_size i = begin; i <= end; ++i)
			{
				if(a[i] > a[i+1])
				{
					swap(a[i], a[i+1]);
					swapped = true;
				}
			}
			if(swapped == false)
			{
				break;
			}
			swapped = false;
			// decreases `end` because the elements after `end` are in correct order
			--end;
			for(t_size i = end; i >= begin; --i)
			{
				if(a[i] > a[i+1])
				{
					swap(a[i], a[i+1]);
					swapped = true;
				}
			}
			// increases `begin` because the elements before `begin` are in correct order
			++begin;
		}
		while(swapped);
	}
} // namespace bl
