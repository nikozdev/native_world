#ifndef NW_CORE_HPP
#define NW_CORE_HPP
/// This is the main header of the engine
/// Here are all relevant macros and configurations

// --==<linking>==--
#if (defined NW_PLATFORM_WINDOWS)
	#if defined NW_LINK_DYNAMIC
		#ifdef NW_BUILD_DLL
			#define NW_API __declspec(dllexport)
		#elif defined NW_BUILD_EXE
			#define NW_API __declspec(dllimport)
		#endif
	#else
		#define NW_API
	#endif
#endif	// NW_PLATFORM
// --==</linking>==--

// --==<configurations>==--
// --gui_library_choice
#define NW_GUI_NATIVE	1 << 1
#define NW_GUI_IMGUI	1 << 2
#define NW_GUI			NW_GUI_IMGUI | NW_GUI_NATIVE
// --==</configurations>==--

// --==</support_macroses>==--

// --==<pragmas>==--
#pragma warning(disable : 4005)
#pragma warning(disable : 4081)
#pragma warning(disable : 4099)
#pragma warning(disable : 4102)
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4552)
#pragma warning(disable : 4996)
// --==</pragmas>==--

#include <nw_pch.hpp>

namespace NW
{
	class NW_API CoreEngine;
	class NW_API CoreWindow;
}

#endif // NW_CORE_HPP

/// 18.10.2020
/// To save your changes in github:
/// --Create repo in your project directory with solution
/// --make .gitingore file where are files you don't want to save
/// --Write in the nw:
/// -->"git clone {url of your repo}" - copy all files from repository into the curr dir
/// -->"git status" - to get info about files you are working with
/// -->"git add *" - add all files except of .gitignore list
/// -->"git reset ." - remove all files from "download stack"
/// -->"git commit -m {"commentary"}"
/// -->"git push origin master" - push all files in repository