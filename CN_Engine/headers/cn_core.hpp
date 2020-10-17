#ifndef CHEER_NIK_CORE_H
#define CHEER_NIK_CORE_H

#ifdef CN_PLATFORM_WINDOWS
	#ifdef CN_BUILD_DLL
		#define CHN_API __declspec(dllexport)
	#else
		#define CHN_API __declspec(dllimport)
	#endif
#else
	//#error CheerNik cannot work with this platform
#endif

#endif