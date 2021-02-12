#pragma once
#include <algorithm>
#include <iterator>

namespace bl
{
	using std::begin;
	using std::end;
	using std::reverse_iterator;
	using std::iterator_traits;
	using std::distance;
	using std::find;
	using std::find_first_of;
	using std::not2;
	using std::equal_to;
	using std::swap;
	using std::remove_reference;
	using std::enable_if;
	using std::is_same;
	using std::transform;
	using std::toupper;
	using std::tolower;

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// general functions
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

#define for_idx(i, c) for(auto (i) = static_cast<typename remove_reference<decltype((c))>::type::size_type>(0); (i) < c.size(); ++(i))

	template <typename T> using static_not = std::integral_constant<bool, !T::value>;

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// iterator functions
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	template<typename t_itr, typename t_sitr>
	t_itr find_first_not_of(t_itr first, t_itr last, t_sitr s_first, t_sitr s_last)
	{
		return find_first_of(first, last, s_first, s_last, not2(equal_to<typename iterator_traits<t_itr>::value_type>()));
	}

	template<typename t_itr>
	t_itr find_last(t_itr first, t_itr last, const typename iterator_traits<t_itr>::value_type& value)
	{
		return find(reverse_iterator<t_itr>(first), reverse_iterator<t_itr>(last), value).base();
	}

	template<typename t_itr, typename t_sitr>
	t_itr find_last_of(t_itr first, t_itr last, t_sitr s_first, t_sitr s_last)
	{
		return find_first_of(reverse_iterator<t_itr>(first), reverse_iterator<t_itr>(last), reverse_iterator<t_sitr>(s_first), reverse_iterator<t_sitr>(s_last)).base();
	}

	template<typename t_itr, typename t_sitr>
	t_itr find_last_not_of(t_itr first, t_itr last, t_sitr s_first, t_sitr s_last)
	{
		return find_first_not_of(reverse_iterator<t_itr>(first), reverse_iterator<t_itr>(last), reverse_iterator<t_sitr>(s_first), reverse_iterator<t_sitr>(s_last)).base();
	}

	template<typename t_itr>
	bool contains(t_itr first, t_itr last, const typename iterator_traits<t_itr>::value_type& value)
	{
		return find(first, last, value) != last;
	}

	template<typename t_itr, typename t_sitr>
	bool contains(t_itr first, t_itr last, t_sitr s_first, t_sitr s_last)
	{
		return search(first, last, s_first, s_last) != last;
	}

	template<typename t_itr, typename t_sitr>
	bool contains_any_of(t_itr first, t_itr last, t_sitr s_first, t_sitr s_last)
	{
		return find_first_of(first, last, s_first, s_last) != last;
	}

	template<typename t_itr, typename t_sitr>
	bool starts_with(t_itr first, t_itr last, t_sitr s_first, t_sitr s_last)
	{
		return search(first, last, s_first, s_last) == first;
	}

	template<typename t_itr, typename t_sitr>
	bool ends_with(t_itr first, t_itr last, t_sitr s_first, t_sitr s_last)
	{
		return search(reverse_iterator<t_itr>(first), reverse_iterator<t_itr>(last), reverse_iterator<t_sitr>(s_first), reverse_iterator<t_sitr>(s_last)) == (reverse_iterator<t_itr>(last) + distance(s_first, s_last));
	}

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// container functions
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	template<typename t_container>
	typename t_container::iterator find(const t_container& container, const typename t_container::value_type& value)
	{
		return find(begin(container), end(container), value);
	}

	template<typename t_container, typename t_values>
	typename t_container::iterator find_first_of(const t_container& container, const t_values& values)
	{
		return find_first_of(begin(container), end(container), begin(values), end(values));
	}

	template<typename t_container, typename t_values>
	typename t_container::iterator find_first_not_of(const t_container& container, const t_values& values)
	{
		return find_first_not_of(begin(container), end(container), begin(values), end(values));
	}

	template<typename t_container>
	typename t_container::iterator find_last(const t_container& container, const typename t_container::value_type& value)
	{
		return find_last(begin(container), end(container), value);
	}

	template<typename t_container, typename t_values>
	typename t_container::iterator find_last_of(const t_container& container, const t_values& values)
	{
		return find_last_of(begin(container), end(container), begin(values), end(values));
	}

	template<typename t_container, typename t_values>
	typename t_container::iterator find_last_not_of(const t_container& container, const t_values& values)
	{
		return find_last_not_of(begin(container), end(container), begin(values), end(values));
	}

	template<typename t_container>
	typename t_container::size_type find_idx(const t_container& container, const typename t_container::value_type& value)
	{
		auto itr = find(begin(container), end(container), value);
		if(itr != end(container))
		{
			return static_cast<typename t_container::size_type>(distance(begin(container), itr));
		}
		return container.size();
	}

	template<typename t_container, typename t_values>
	typename t_container::size_type find_first_idx_of(const t_container& container, const t_values& values)
	{
		auto itr = find_first_of(begin(container), end(container), begin(values), end(values));
		if(itr != end(container))
		{
			return static_cast<typename t_container::size_type>(distance(begin(container), itr));
		}
		return container.size();
	}

	template<typename t_container, typename t_values>
	typename t_container::size_type find_first_idx_not_of(const t_container& container, const t_values& values)
	{
		auto itr = find_first_not_of(begin(container), end(container), begin(values), end(values));
		if(itr != end(container))
		{
			return static_cast<typename t_container::size_type>(distance(begin(container), itr));
		}
		return container.size();
	}

	template<typename t_container>
	typename t_container::size_type find_last_idx(const t_container& container, const typename t_container::value_type& value)
	{
		auto itr = find_last(begin(container), end(container), value);
		if(itr != end(container))
		{
			return static_cast<typename t_container::size_type>(distance(begin(container), itr));
		}
		return container.size();
	}

	template<typename t_container, typename t_values>
	typename t_container::size_type find_last_idx_of(const t_container& container, const t_values& values)
	{
		auto itr = find_last_of(begin(container), end(container), begin(values), end(values));
		if(itr != end(container))
		{
			return static_cast<typename t_container::size_type>(distance(begin(container), itr));
		}
		return container.size();
	}

	template<typename t_container, typename t_values>
	typename t_container::size_type find_last_idx_not_of(const t_container& container, const t_values& values)
	{
		auto itr = find_last_not_of(begin(container), end(container), begin(values), end(values));
		if(itr != end(container))
		{
			return static_cast<typename t_container::size_type>(distance(begin(container), itr));
		}
		return container.size();
	}

	template<typename t_container>
	bool contains(const t_container& container, const typename t_container::value_type& value)
	{
		return contains(begin(container), end(container), value);
	}

	template<typename t_container, typename t_values>
	bool contains(const t_container& container, const t_values& values)
	{
		return contains(begin(container), end(container), begin(values), end(values));
	}

	template<typename t_container, typename t_values>
	bool contains_any_of(const t_container& container, const t_values& values)
	{
		return contains_any_of(begin(container), end(container), begin(values), end(values));
	}

	template<typename t_container, typename t_values>
	bool starts_with(const t_container& container, const t_values& values)
	{
		return starts_with(begin(container), end(container), begin(values), end(values));
	}

	template<typename t_container, typename t_values>
	bool ends_with(const t_container& container, const t_values& values)
	{
		return ends_with(begin(container), end(container), begin(values), end(values));
	}
} // namespace bl
