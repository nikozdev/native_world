#ifndef NW_CORE_HPP
#define NW_CORE_HPP

// --==<configuration>==--
#if defined NW_BUILD_EXE
	#define NW_API
#elif defined NW_BUILD_LIB
	#define NW_API
#else
	#if defined NW_BUILD_DLL
		#define NW_API __declspec(dllexport)
	#else
		#define NW_API __declspec(dllimport)
	#endif
#endif
// --==</configuration>==--

#include <nw_pch.hpp>

namespace NW
{
	class NW_API CoreEngine;
	class NW_API CoreWindow;
}

#endif // NW_CORE_HPP