#pragma once
#if !defined(SAG_COM) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
	#define BL_OS_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
	#define BL_OS_WIN32
#elif defined(__linux__) || defined(__linux)
	#define BL_OS_LINUX
	#if !defined(__i386__) || defined(__x86_64__)
		#define BL_OS_LINUX64
	#else
		#define BL_OS_LINUX32
	#endif
#endif

#if defined(BL_OS_WIN32) || defined(BL_OS_WIN64)
	#define BL_OS_WIN
#endif

#if defined(BL_OS_LINUX64) || defined(BL_OS_WIN64)
	#define BL_OS_64
#endif

#if defined(BL_OS_WIN32) || defined(BL_OS_LINUX32)
	#define BL_OS_32
#endif

#if defined(BL_OS_WIN)
	#define BL_DLL_PUBLIC __declspec(dllexport)
	#define BL_DLL_PRIVATE
#elif defined(BL_OS_LINUX)
	#define BL_DLL_PUBLIC __attribute__((visibility("default")))
	#define BL_DLL_PRIVATE __attribute__((visibility("hidden")))
#else
	#error Operating system unknown or unsupported.
#endif
