#pragma once

// ref: http://users.encs.concordia.ca/~chvatal/notes/hsort.html
namespace bl
{
	template<typename t_size>
	t_size floor_of_lg(const t_size n, t_size*__restrict__ const power)
	{
		/************************************************************
		  *  computes: the largest power of 2 that is at most n      *
		  *  returns:  the exponent in this power:                   *
		  *            n =   1   2   3   4   5   6   7   8   9  ...  *
		  *        power =   1   2   2   4   4   4   4   8   8  ...  *
		  *   exponent k =   0   1   1   2   2   2   2   3   3  ...  *
		  ***********************************************************/

		t_size k = 0;
		for(*power=1; 2*(*power)<=n; (*power)*=2)
		{
			++k;
		}
		return k;
	}

	template<typename t_value, typename t_size>
	void sift_down(t_value*__restrict__ const a, const t_size n, t_size vacant, const t_value missing, const t_size drop)
	{
		const t_size memo=vacant;
		t_size child, parent;
		t_size count, next_peek;

		count=0, next_peek=(drop+1)/2;

		child=2*(vacant+1);
		while(child<n)
		{
			if(a[child] < a[child-1])
				child--;
			a[vacant]=a[child], vacant=child, child=2*(vacant+1);

			count++;
			if (count==next_peek)
			{
				if(a[(vacant-1)/2] <= missing)
					break;
				else
					next_peek=(count+drop+1)/2;
			}
		}

		if(child==n)
		{
			a[vacant]=a[n-1], vacant=n-1;
		}

		parent=(vacant-1)/2;
		while(vacant>memo)
		{
			if(a[parent] < missing)
			{
				a[vacant]=a[parent], vacant=parent, parent=(vacant-1)/2;
			}
			else
				break;
		}
		a[vacant]=missing;
	}

	template<typename t_value, typename t_size>
	void make_heap(t_value*__restrict__ const a, const t_size n)
	{
		t_size k, drop, first;

		drop=1, first=n/2-1;
		for(k=first; k>=0; k--)
		{
			if(k==(first-1)/2)
			{
				++drop, first=k;
			}
			sift_down(a, n, k, a[k], drop);
		}
	}

	template<typename t_value, typename t_size>
	void heap_sort(t_value*__restrict__ const a, const t_size size)
	{
		t_size k, drop, last;
		t_value temp;

		make_heap(a,size);

		drop = floor_of_lg(size-1, &last);
		for(k=size-1; k>0; k--)
		{
			temp=a[k], a[k]=a[0];
			sift_down(a, k, 0, temp, drop);
			if (k==last)
			{
				drop--, last/=2;
			}
		}
	}
} // namespace bl
