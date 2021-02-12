#include <bl/util/string.h>

namespace bl
{
	void replace(string& astr, const string& old_str, const string& new_str)
	{
		auto i = astr.find(old_str);
		while(i != std::string::npos)
		{
			astr.replace(i, old_str.size(), new_str);
			i = astr.find(old_str, i + new_str.size());
		}
	}

	bool operator<(const string& a, const string& b)
	{
		return std::lexicographical_compare(begin(a), end(a), begin(b), end(b));
	}

	string operator+(const string& astr, const char* chars)
	{
		return astr + str(chars);
	}

	void trim_left(string& astr, const string& char_set)
	{
		astr.erase(0, find_first_idx_not_of(astr, char_set));
	}

	void trim_right(string& astr, const string& char_set)
	{
		astr.erase(find_last_idx_not_of(astr, char_set) + 1);
	}

	void trim_both(string& astr, const string& char_set)
	{
		trim_left(astr, char_set);
		trim_right(astr, char_set);
	}

	vector<string> split(const string& astr, string::size_type idx)
	{
		vector<string> ret;
		ret.push_back(astr.substr(0, idx));
		ret.push_back(astr.substr(idx, astr.size()-idx));
		return ret;
	}

	vector<string> split(const string& astr, const string& separator, bool keep_empty_parts /*= false*/)
	{
		vector<string> ret;

		if(astr.empty() || separator.empty())
		{
			return ret;
		}

		auto n = astr.find(separator);

		if(n == std::string::npos)
		{
			ret.push_back(astr);
			return ret;
		}

		string::size_type nOld = 0;

		do
		{
			// Found separator in the n-th position. If n == nOld, we found an empty part.
			if(n != nOld || keep_empty_parts)
			{
				ret.push_back(astr.substr(nOld, n - nOld));
			}

			n   += separator.size();
			nOld = n;

			n = astr.find(separator, n);
		}
		while(n != std::string::npos);

		// Get everything after the last separator, unless it's empty and 'keepEmptyParts' is false.
		if(nOld < astr.size() - 1 || keep_empty_parts)
		{
			ret.push_back(astr.substr(nOld, astr.size() - nOld));
		}

		return ret;
	}

	void to_uppercase(string& astr)
	{
		transform(begin(astr), end(astr), begin(astr), static_cast<int (*)(int)>(toupper));
	}

	void to_lowercase(string& astr)
	{
		transform(begin(astr), end(astr), begin(astr), static_cast<int (*)(int)>(toupper));
	}

} // namespace bl

