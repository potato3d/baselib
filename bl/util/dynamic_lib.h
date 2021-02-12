#pragma once
#include <bl/util/function.h>
#include <bl/util/platform.h>
#include <bl/util/string.h>

namespace bl
{
	class dynamic_lib
	{
	public:
		static bool has_valid_extension(const string& pathstr);

		dynamic_lib(const string& pathstr);
		~dynamic_lib();

		bool load();
		bool unload();

		const string& filepath();
		const string& last_error();

		template<typename t_function>
		t_function resolve_function(const string& name);

	private:
		typedef void(*func_type)();
		func_type _resolve_function(const string& name);

		string _filepath;
		string _lasterror;
		bool _loaded;
		void* _handleptr;
	};

	template<typename t_function>
	t_function dynamic_lib::resolve_function(const string& name)
	{
		if(!_loaded)
		{
			return nullptr;
		}
		return reinterpret_cast<t_function>(_resolve_function(name));
	}
} // namespace bl
