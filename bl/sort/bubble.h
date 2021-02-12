#pragma once
#include <bl/util/algorithm.h>

// ref: http://en.wikipedia.org/wiki/Bubble_sort
namespace bl
{
	template<typename t_value, typename t_size>
	void bubble_sort(t_value*__restrict__ const a, const t_size size)
	{
		t_size n = size;
		do
		{
			t_size newn = 0;
			for(t_size i = 1; i < n; ++i)
			{
				if(a[i-1] > a[i])
				{
					swap(a[i-1], a[i]);
					newn = i;
				}
			}
			n = newn;
		}
		while(n > 0);
	}
} // namespace bl
