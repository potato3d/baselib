#pragma once

// ref: http://pt.wikipedia.org/wiki/Shell_sort#C.C3.B3digo_em_C
namespace bl
{
	template<typename t_value, typename t_size>
	void shell_sort(t_value*__restrict__ const a, const t_size size)
	{
		t_size gap = 1;
		do
		{
			gap = 3*gap+1;
		}
		while(gap < size);
		do
		{
			gap /= 3;
			for(t_size i = gap; i < size; i++)
			{
				const t_value value = a[i];
				t_size j = i - gap;
				while(j >= 0 && value < a[j])
				{
					a[j+gap] = a[j];
					j -= gap;
				}
				a[j+gap] = value;
			}
		}
		while(gap > 1);
	}
} // namespace bl
