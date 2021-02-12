#pragma once
#include <bl/search/binary.h>
#include <cstring>

namespace bl
{
	// ref: http://en.wikipedia.org/wiki/Insertion_sort
	template<typename t_value, typename t_size>
	void insertion_sort(t_value*__restrict__ const a, const t_size size)
	{
		for(t_size i = 1; i < size; ++i)
		{
			const t_value value = a[i];
			t_size idx = i;
			while(idx > 0 && value < a[idx-1])
			{
				a[idx] = a[idx-1];
				--idx;
			}
			a[idx] = value;
		}
	}

	// ref: http://jeffreystedfast.blogspot.com.br/2007/02/binary-insertion-sort.html
	template<typename t_value, typename t_size>
	void insertion_sort_binary(t_value*__restrict__ const a, const t_size size)
	{
		for(t_size i = 1; i < size; ++i)
		{
			const t_size ins = binary_search_iter(a, i, a[i]);
			if(ins < i)
			{
				const t_value tmp = a[i];
				for(t_size j = i-1; j >= ins; --j)
				{
					a[j+1] = a[j];
				}
				a[ins] = tmp;
			}
		}
	}

	// http://jeffreystedfast.blogspot.com.br/2007/02/binary-insertion-sort.html
	template<typename t_value, typename t_size>
	void insertion_sort_binary_move(t_value*__restrict__ const a, const t_size size)
	{
		for(t_size i = 1; i < size; ++i)
		{
			const t_size ins = binary_search_iter(a, i, a[i]);
			if(ins < i)
			{
				const t_value tmp = a[i];
				memmove(a+ins+1, a+ins, sizeof(t_value)*(i-ins));
				a[ins] = tmp;
			}
		}
	}
} // namespace bl
