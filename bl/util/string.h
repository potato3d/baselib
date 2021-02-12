#pragma once
#include <bl/util/algorithm.h>
#include <bl/util/containers.h>
#include <sstream>

namespace bl
{
	using std::string;
	using std::stringstream;
	struct str_holder;

	void replace(string& astr, const string& old_str, const string& new_str);

	template<typename t_value>
	string str(t_value value)
	{
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}

	template<typename ...t_args>
	string str(const string& base, t_args&& ...args)
	{
		string ret(base);
		array<str_holder, sizeof...(t_args)> args_array = {{args...}};
		for_idx(i, args_array)
		{
			replace(ret, "%" + str(i), args_array[i].the_str);
		}
		return ret;
	}

	template<typename t_value, typename enable_if<static_not<is_same<t_value, string>>::value>::type>
	string operator+(const string& astr, const t_value& value)
	{
		return astr + str(value);
	}

	string operator+(const string& astr, const char* chars);

	bool operator<(const string& a, const string& b);

	void trim_left(string& astr, const string& char_set);

	void trim_right(string& astr, const string& char_set);

	void trim_both(string& astr, const string& char_set);

	vector<string> split(const string& astr, string::size_type idx);

	vector<string> split(const string& astr, const string& separator, bool keep_empty_parts = false);

	void to_uppercase(string& astr);

	void to_lowercase(string& astr);

	struct str_holder
	{
	public:
	  template<typename t_value>
	  str_holder(t_value value)
		  : the_str(str(value))
	  {
	  }
	  string the_str;
	};
} // namespace bl
