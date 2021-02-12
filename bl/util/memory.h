#pragma once
#include <memory>
#include <type_traits>
#include <utility>

namespace bl
{
	using std::make_shared;
	using std::shared_ptr;
	using std::unique_ptr;
	using std::weak_ptr;
	using std::dynamic_pointer_cast;

	template<typename t_value> struct _unique_if
	{
		typedef unique_ptr<t_value> _single_object;
	};

	template<typename t_value> struct _unique_if<t_value[]>
	{
		typedef unique_ptr<t_value[]> _unknown_bound;
	};

	template<typename t_value, size_t N> struct _unique_if<t_value[N]>
	{
		typedef void _known_bound;
	};

	template<typename t_value, typename ...t_args>
	typename _unique_if<t_value>::_single_object make_unique(t_args&& ...args)
	{
		return unique_ptr<t_value>(new t_value(std::forward<t_args>(args)...));
	}

	template<typename t_value>
	typename _unique_if<t_value>::_unknown_bound make_unique(size_t n)
	{
		typedef typename std::remove_extent<t_value>::type value_type;
		return unique_ptr<t_value>(new value_type[n]());
	}

	template<typename t_value, typename ...t_args>
	typename _unique_if<t_value>::_known_bound make_unique(t_args&&...) = delete;
} // namespace bl
