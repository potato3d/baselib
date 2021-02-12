#include <bl/util/dynamic_lib.h>
#include <bl/util/algorithm.h>
#include <bl/util/path.h>

#if defined BL_OS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined BL_OS_LINUX
#include <dlfcn.h>
#else
#error "Unsupported operating system."
#endif

namespace bl
{
	static string get_last_error()
	{
		string err = "unknown error";
	#ifdef BL_OS_WIN
		LPVOID lpMsgBuf;
		DWORD dw = GetLastError();

		FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					dw,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					reinterpret_cast<LPTSTR>(&lpMsgBuf),
					0, NULL );

		if(lpMsgBuf != nullptr)
		{
			err = lpMsgBuf;
			LocalFree(lpMsgBuf);
		}
	#elif defined BL_OS_LINUX
		err = dlerror();
	#else
	#error "unsupported operating system"
	#endif
		return err;
	}

	bool dynamic_lib::has_valid_extension(const string& pathstr)
	{
	#if defined BL_OS_WIN
	#define WIN32_LEAN_AND_MEAN
		const string ext = "dll";
	#elif defined BL_OS_LINUX
		const string ext = "so";
	#else
	#error "Unsupported operating system."
	#endif

		return contains(path::get_extension(pathstr), ext);
	}

	dynamic_lib::dynamic_lib(const string& pathstr)
		: _filepath(pathstr),
		  _loaded(false),
		  _handleptr(nullptr)
	{
	}

	dynamic_lib::~dynamic_lib()
	{
		unload();
	}

	bool dynamic_lib::load()
	{
		if(_loaded)
		{
			unload();
		}

		_lasterror = "no error";

		if(!path::exists(_filepath))
		{
			_lasterror = str("Cannot load '%0' (error = No such file or directory)", _filepath);
			return false;
		}

		if(!has_valid_extension(_filepath))
		{
			_lasterror = str("Cannot load '%0' (error = Wrong extension)", _filepath);
			return false;
		}

	#if defined(BL_OS_WIN)
		// Avoid 'Bad Image' message box.
		UINT oldmode = SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX);
		_handleptr = LoadLibraryA(_filepath.ptr());
		SetErrorMode(oldmode);
	#elif defined BL_OS_LINUX
		// Relocations are performed when the object is loaded.
		_handleptr = dlopen(_filepath.data(), RTLD_NOW);
	#else
	#error "Unsupported operating system."
	#endif

		if(!_handleptr)
		{
			_loaded  = false;
			_lasterror = str("Cannot load '%0' (error = %1)", _filepath, get_last_error());
		}
		else
		{
			_loaded = true;
			_lasterror = "no error";
		}

		return _loaded;
	}

	bool dynamic_lib::unload()
	{
		if(_loaded)
		{
	#ifdef BL_OS_WIN
			_loaded = !FreeLibrary(reinterpret_cast<HINSTANCE>(_handleptr));
	#elif defined BL_OS_LINUX
			_loaded = dlclose(_handleptr);
	#else
	#error "Unsupported operating system."
	#endif
		}

		if(_loaded)
		{
			_lasterror = get_last_error();
		}
		else
		{
			_handleptr = nullptr;
			_lasterror = "no error";
		}

		return !_loaded;
	}

	const string& dynamic_lib::filepath()
	{
		return _filepath;
	}

	const string& dynamic_lib::last_error()
	{
		return _lasterror;
	}

	dynamic_lib::func_type dynamic_lib::_resolve_function(const string& name)
	{
		func_type address = nullptr;

	#if defined(BL_OS_WIN32)
		typedef __stdcall int (*Ptr)();
		*reinterpret_cast<Ptr*>(&address) = GetProcAddress(static_cast<HINSTANCE>(_handleptr), name.ptr());
		if(!address)
		{
			_lasterror = string("dynamic_lib::_resolve_symbol: Symbol '%0' undefined in '%1'", name, _filepath);
			return nullptr;
		}
	#elif defined(BL_OS_WIN64)
		typedef __stdcall long long int (*Ptr)();
		*reinterpret_cast<Ptr*>(&address) = GetProcAddress(static_cast<HINSTANCE>(_handleptr), name.ptr());
		if(!address)
		{
			_lasterror = string("dynamic_lib::_resolve_symbol: Symbol '%0' undefined in '%1'", name, _filepath);
			return nullptr;
		}
	#elif defined BL_OS_LINUX
		*reinterpret_cast<void**>(&address) = dlsym(_handleptr, name.data());
		const char* err = dlerror();
		if(err)
		{
			_lasterror = str("Symbol '%0' undefined in '%1' (%2)", name, _filepath, err);
			return nullptr;
		}
	#else
	#error "Unsupported operating system."
	#endif

		_lasterror = "";
		return address;
	}
} // namespace bl
