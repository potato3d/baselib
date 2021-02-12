#pragma once

namespace bl
{
	// ref: http://en.wikipedia.org/wiki/Binary_search_algorithm
	template<typename t_value, typename t_size>
	t_size binary_search_rec(const t_value*__restrict__ const a, const t_size low, const t_size high, const t_value&__restrict__ key)
	{
		if(low == high)
		{
			return low;
		}
		const t_size mid =  low + ((high - low) / 2);
		if(key > a[mid])
		{
			return binary_search_rec(a, mid+1, high, key);
		}
		else if (key < a[mid])
		{
			return binary_search_rec(a, low, mid, key);
		}
		else
		{
			return mid;
		}
	}

	// ref: http://jeffreystedfast.blogspot.com.br/2007/02/binary-insertion-sort.html
	template<typename t_value, typename t_size>
	t_size binary_search_iter(const t_value*__restrict__ const a, const t_size size, const t_value&__restrict__ key)
	{
		t_size low = 0;
		t_size high = size;
		t_size mid = size / 2;
		do
		{
			if(key > a[mid])
			{
				low = mid+1;
			}
			else if(key < a[mid])
			{
				high = mid;
			}
			else
			{
				break;
			}
			mid = low + ((high - low) / 2);
		}
		while(low < high);
		return mid;
	}
} // namespace bl
