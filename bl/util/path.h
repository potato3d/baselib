#pragma once
#include <bl/util/containers.h>
#include <bl/util/string.h>

namespace bl
{
	class path
	{
	public:
		static string working_directory();
		static string to_absolute(const string& pathstr);

		static string separator();
		static string join(const string& pathstr1, const string& pathstr2);

		static string clean(const string& pathstr);
		static string get_directory(const string& pathstr);
		static string get_basename(const string& pathstr);
		static string get_extension(const string& pathstr);

		static bool exists(const string& pathstr);
		static bool is_directory(const string& pathstr);
		static bool is_file(const string& pathstr);
		static bool is_symlink(const string& pathstr);

		static vector<string> list_files(const string& pathstr);
		static vector<string> list_folders(const string& pathstr);
	};
} // namespace bl
