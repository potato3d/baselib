#include <bl/util/path.h>
#include <bl/util/platform.h>

#include <sys/stat.h>

#if defined(BL_OS_WIN)
#include <io.h>
#elif defined BL_OS_LINUX
#include <dirent.h>
#include <unistd.h>
#else
#error "Unsupported operating system."
#endif

namespace bl
{
	static const string s_invalidSep = "\\"; //!< Standard invalid path separator.
	static const string s_validSep   = "/";  //!< Standard valid path separator.
	#ifdef BL_OS_WIN
	static const vector<string> s_windows_invalid_files = {"AUX",  "NUL",  "CON",  "PRN",  "AUX",  "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7",
													"COM8", "COM9", "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8","LPT9"};
	#endif

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	class dir_iterator
	{
	public:
		dir_iterator(const std::string& dir_path);
		~dir_iterator();

		bool open_dir();
		bool close_dir();
		bool next();
		const char* get_name() const;

	private:
		bool        _opened;
		std::string _path;

		#if defined(BL_OS_WIN)
		intptr_t           _file_id;     //!< Windows file ID.
		struct _finddata_t _file_handle; //!< Windows file handle.
		#elif defined BL_OS_LINUX
		DIR*           _dir;            //!< Linux directory handle.
		struct dirent* _entry;          //!< Linux directory entry.
		#else
		#error "Unsupported operating system."
		#endif
	};

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------

	dir_iterator::dir_iterator(const std::string& dir_path) : _path(dir_path)
	{
		_opened = false;
		#if defined(BL_OS_WIN)
		_file_id = -1;
		#elif defined BL_OS_LINUX
		_dir   = nullptr;
		_entry = nullptr;
		#else
		#error "Unsupported operating system."
		#endif
	}

	dir_iterator::~dir_iterator()
	{
		close_dir();
	}

	bool dir_iterator::open_dir()
	{
		#if defined(BL_OS_WIN)
		const std::string pattern = _path + "/*";
		_opened = ((_file_id = _findfirst(pattern.c_str(), &_file_handle)) != -1);
		#elif defined BL_OS_LINUX
		_opened = (_dir = opendir(_path.c_str())) != nullptr;

		#else
		#error "Unsupported operating system."
		#endif

		return _opened;
	}

	bool dir_iterator::close_dir()
	{
		if(!_opened)
		{
			return false;
		}

		_opened = false;
		#if defined(BL_OS_WIN)
		bool closed = (_findclose(_file_id)) == 0;
		#elif defined BL_OS_LINUX
		bool closed = (closedir(_dir)) == 0;
		#else
		#error "Unsupported operating system."
		#endif

		return closed;
	}

	bool dir_iterator::next()
	{
		if(!_opened)
		{
			return false;
		}

		#if defined(BL_OS_WIN)
		return _findnext(_file_id, &_file_handle) != -1;

		#elif defined(BL_OS_LINUX)
		return (_entry = readdir(_dir)) != nullptr;

		#else
		#error "Unsupported operating system."
		#endif
	}

	const char* dir_iterator::get_name() const
	{
		if(!_opened)
		{
			return "";
		}

		#if defined(BL_OS_WIN)
		return _file_handle.name;

		#elif defined BL_OS_LINUX
		return _entry->d_name;

		#else
		#error "Unsupported operating system."
		#endif
	}


	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	static vector<string> get_directory_entries(const string& pathstr, bool getDirectories)
	{
		string cleaned_path = path::clean(pathstr);
		vector<string> results;

		if(!path::is_directory(cleaned_path))
		{
			return results;
		}

		// Add ending separator if we don't have it.
		cleaned_path = path::join(cleaned_path, s_validSep);

		dir_iterator it(cleaned_path.data());

		if(!it.open_dir())
		{
			return results;
		}

		while(it.next())
		{
			const string entry(it.get_name());

			// If 'isDirFlag', we'll get only folders, otherwise we'll get only files.
			// Always avoid '..' and '.' system folders.
			// Avoid '.svn' folder.
			if(entry == "." || entry == ".." || entry == ".svn")
			{
				continue;
			}

			const string fullPath = path::join(cleaned_path, entry);

			if(getDirectories)
			{
				if(path::is_directory(fullPath))
				{
					results.push_back(fullPath + s_validSep);
				}
			}
			else if(path::is_file(fullPath))
			{
				results.push_back(fullPath);
			}
		}

		it.close_dir();

		return results;
	}

	static bool is_win_absolute(const string& pathstr)
	{
		return  (pathstr.size() > 1) &&
				(std::isalpha(pathstr[0])) &&
				(pathstr[1] == ':');
	}

	static bool has_valid_characters(const string& pathstr)
	{
		// Windows invalid characters.
	#if defined BL_OS_WIN
		if(pathstr.contains_any("*?\"<>|"))
		{
			return false;
		}

		// Windows reserved filenames.
		string uppercasePath = pathstr;
		uppercasePath.to_uppercase();
		vector<string> items = uppercasePath.split(s_validSep, false);

		for(const auto& item : items)
		{
			if(s_windows_invalid_files.contains(item))
			{
				return false;
			}
		}

	#elif defined BL_OS_LINUX
		// Windows classic absolute path.
		if(is_win_absolute(pathstr))
		{
			return false;
		}
	#else
	#error "Unsupported operating system."
	#endif
		return true;
	}

	static void clean_network_path(string& pathstr)
	{
		bool fwd_slashes = starts_with(pathstr, "//");
		bool bwd_slashes = starts_with(pathstr, "\\\\");

		if(fwd_slashes || bwd_slashes)
		{
	#ifdef BL_OS_WIN
			// Change '//' to '\\'.
			if(fwd_slashes)
			{
				pathstr.remove(0, 2);
				pathstr = "\\\\" + pathstr;
			}

	#elif defined BL_OS_LINUX
			if(bwd_slashes)
			{
				pathstr.erase(0, 2);
				pathstr = "//" + pathstr;
			}
	#else
	#error "Unsupported operating system."
	#endif
			while(pathstr.size() > 2 && (pathstr[2] == s_validSep[0] || pathstr[2] == s_invalidSep[0]))
			{
				pathstr.erase(2, 1);
			}
		}
		// Linux network path.
		else if(starts_with(pathstr, "smb://"))
		{
	#ifdef BL_OS_WIN
			// Change 'smb://' to '\\'.
			pathstr.remove(0, 6);
			pathstr = "\\\\" + pathstr;
	#elif defined BL_OS_LINUX
	#else
	#error "Unsupported operating system."
	#endif
		}
		// Linux network path.
		else if(starts_with(pathstr, "sftp://"))
		{
	#ifdef BL_OS_WIN
			// Change 'sftp://' to '\\'.
			pathstr.remove(0, 7);
			pathstr = "\\\\" + pathstr;
	#elif defined BL_OS_LINUX
	#else
	#error "Unsupported operating system."
	#endif
		}
	}

	static void remove_redundant_characters(string& pathstr)
	{
		trim_both(pathstr, " ");

		if(pathstr.empty())
		{
			return;
		}

	#if defined BL_OS_WIN
		// We just want one ':', so we'll delete all except the first one.
		if(pathstr.count(":") > 1)
		{
			pathstr.replace_all_skip(":", "", 1);
		}

	#elif defined BL_OS_LINUX
	#else
	#error "Unsupported operating system."
	#endif

		// check for network identifier
		string networkPrefix;

		if(starts_with(pathstr, "\\\\"))
		{
			networkPrefix = "\\\\";
		}
		if(starts_with(pathstr, "//"))
		{
			networkPrefix = "//";
		}
		if(starts_with(pathstr, "smb://"))
		{
			networkPrefix = "smb://";
		}
		if(starts_with(pathstr, "sftp://"))
		{
			networkPrefix = "sftp://";
		}

		// Remove prefix from path
		pathstr.erase(0, networkPrefix.size());

		// Normalize separators.
		replace(pathstr, s_invalidSep, s_validSep);

		// Remove redundants separators.
		auto doubleSeparator = str(s_validSep) + s_validSep;

		while(contains(pathstr, doubleSeparator))
		{
			replace(pathstr, doubleSeparator, s_validSep);
		}

		// '.' in the end is useless, so we'll remove it.
		while(ends_with(pathstr, "."))
		{
			pathstr.erase(pathstr.size()-1, 1);
		}

		// Restore network prefix
		pathstr = networkPrefix + pathstr;
	}

	static void solve_relative_path(string& input)
	{
		string currentStr;
		string lastStr;

		string::size_type firstIdxBeforeSep = 0;
		string::size_type idxStr            = 0;

		auto itr = std::begin(input);

		while(itr != std::end(input))
		{
			if(*itr != '/')
			{
				currentStr += (*itr);
			}

			if(*itr == '/' || itr == std::end(input) - 1)
			{
				// We don't want to remove '.' from the beginning on Linux. On Windows it doesn't make any difference, so we'll remove it.
				if(currentStr == ".")
				{
					#if defined BL_OS_LINUX
					{
						if(lastStr.empty())
						{
							lastStr = currentStr;
							currentStr = "";
							idxStr++;
							itr++;
							continue;
						}
					}
					#endif

					// If we're at the last index, we don't want to remove the last separator.
					if(itr == std::end(input) - 1)
					{
						input.erase(idxStr, 1);
					}
					else
					{
						input.erase(idxStr - 1, 2);
					}

					idxStr            = 0;
					firstIdxBeforeSep = 0;
					lastStr = "";
					currentStr = "";
					itr = std::begin(input);
					continue;
				}
				// We don't want to remove '..' from the beginning.
				// Also, we don't want '..' to remove another '..'.
				// Also, we don't want '..' to remove '.' on Linux.
				else if(currentStr == ".." && lastStr != "" && lastStr != "..")
				{
					#if defined BL_OS_LINUX
					{
						if(lastStr == ".")
						{
							lastStr = currentStr;
							currentStr = "";
							idxStr++;
							itr++;
							continue;
						}
					}
					#endif

					// c:
					if(is_win_absolute(lastStr))
					{
						input.erase(idxStr - 3, 3);
					}
					else
					{
						input.erase(firstIdxBeforeSep, idxStr - firstIdxBeforeSep + 1);
					}

					idxStr            = 0;
					firstIdxBeforeSep = 0;
					lastStr = "";
					currentStr = "";
					itr = std::begin(input);
					continue;
				}
				else
				{
					firstIdxBeforeSep = idxStr - currentStr.size();
					lastStr           = currentStr;
					currentStr = "";
				}
			}

			idxStr++;
			itr++;
		}
	}

	static void decompose(const string& pathstr, string& directory, string& basename, string& extension)
	{
		directory = basename = extension = "";

		string cleaned_path = path::clean(pathstr);

		if(cleaned_path.empty())
		{
			return;
		}

		// Path that ends in a separator is a dir.
		if(ends_with(cleaned_path, s_validSep))
		{
			directory = cleaned_path;
			return;
		}

		// If got here, we have a base name.
		string::size_type last_valid_separator = cleaned_path.rfind(s_validSep);

		#if defined BL_OS_WIN
		string::size_type dotIdx = cleaned_path.rfind(".");
		#elif defined BL_OS_LINUX
		string::size_type dotIdx = cleaned_path.find(".", last_valid_separator == string::npos? 0 : last_valid_separator);
		#else
		#error "Unsupported operating system."
		#endif

		directory = cleaned_path.substr(0, last_valid_separator + 1);

		// Base name doesn't have extension.
		if(dotIdx == std::string::npos)
		{
			basename = cleaned_path.substr(last_valid_separator + 1);
		}
		// Base name has extension.
		else
		{
			extension = cleaned_path.substr(dotIdx + 1);
			basename  = cleaned_path.substr(last_valid_separator + 1, dotIdx - last_valid_separator - 1);
		}
	}

	static bool exists_impl(const string& pathstr, struct stat* info)
	{
		if(pathstr.empty())
		{
			return false;
		}

		string cleaned_path = path::clean(pathstr);

		if(cleaned_path.empty())
		{
			return false;
		}

		bool ret = stat(cleaned_path.data(), info) == 0;
		if(!ret)
		{
			string newPath = path::join(cleaned_path, s_validSep);
			ret = stat(newPath.data(), info) == 0;
		}
		return ret;
	}

	// ---------------------------------------------------------------------------------------------------------------------------------------------------------

	string path::working_directory()
	{
		const int   size = 2048;
		string pathstr;
		pathstr.resize(size);

		// It's an ultra safe check, but we're not able to enter this IF as we depend on the 'getcwd' system function.
		if(getcwd(&pathstr.front(), size) == nullptr)
		{
			return "";
		}

		return clean(pathstr) + s_validSep;
	}

	string path::to_absolute(const string& pathstr)
	{
		string cleaned_path = clean(pathstr);

	#ifdef BL_OS_WIN
		if(cleaned_path.starts_with("\\\\") || cleaned_path.starts_with(s_validSep) || is_win_absolute(cleaned_path))
		{
			return cleaned_path;
		}
	#elif defined BL_OS_LINUX
		if(starts_with(cleaned_path, "smb://") || starts_with(cleaned_path, "sftp://") || starts_with(cleaned_path, s_validSep))
		{
			return cleaned_path;
		}
	#else
	#error "Unsupported operating system."
	#endif

		return working_directory() + cleaned_path;
	}

	string path::separator()
	{
		return s_validSep;
	}

	string path::join(const string& pathstr1, const string& pathstr2)
	{
		string path1 = clean(pathstr1);
		trim_right(path1, s_validSep);

		string path2 = clean(pathstr2);
		trim_left(path2, s_validSep);

		return path1 + s_validSep + pathstr2;
	}

	string path::clean(const string& pathstr)
	{
		string result = pathstr;
		trim_both(result, " ");

		// We won't clean empty strings and invalid paths
		if(result.empty() || !has_valid_characters(result))
		{
			return "";
		}

		clean_network_path(result);

		// remove extra colons, separators and useless characters.
		remove_redundant_characters(result);

		// Solve relative paths (i.e '.' and '..').
		solve_relative_path(result);

		return result;
	}

	string path::get_directory(const string& pathstr)
	{
		string directory_name, base_name, extension;
		decompose(pathstr, directory_name, base_name, extension);
		return directory_name;
	}

	string path::get_basename(const string& pathstr)
	{
		string directory_name, base_name, extension;
		decompose(pathstr, directory_name, base_name, extension);
		return base_name;
	}

	string path::get_extension(const string& pathstr)
	{
		string directory_name, base_name, extension;
		decompose(pathstr, directory_name, base_name, extension);
		return extension;
	}

	bool path::exists(const string& pathstr)
	{
		struct stat info;
		return exists_impl(pathstr, &info);
	}

	bool path::is_directory(const string& pathstr)
	{
		struct stat info;
		bool ret = exists_impl(pathstr, &info);
		if(!ret)
		{
			return false;
		}
		return S_ISDIR(info.st_mode) != 0;
	}

	bool path::is_file(const string& pathstr)
	{
		return !is_directory(pathstr);
	}

	bool path::is_symlink(const string& pathstr)
	{
	#if defined BL_OS_WIN
		return !pathstr.empty();
	#elif defined(BL_OS_LINUX)
		vector<char> buffer(1024, '\0');
		return readlink(pathstr.data(), buffer.data(), static_cast<size_t>(buffer.size() - 1)) >= 0;
	#else
		#error "Unsupported operating system."
	#endif

	}

	vector<string> path::list_files(const string& pathstr)
	{
		return get_directory_entries(pathstr, false);
	}

	vector<string> path::list_folders(const string& pathstr)
	{
		return get_directory_entries(pathstr, true);
	}
} // namespace bl
