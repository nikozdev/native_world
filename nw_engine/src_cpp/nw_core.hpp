#ifndef NW_CORE_HPP
#define NW_CORE_HPP
/// This is the main header of the engine
/// Here are all relevant macros and configurations

// --==<linking>==--
#if defined NW_LINK_DYNAMIC
#ifdef NW_BUILD_DLL
#define NW_API __declspec(dllexport)
#elif defined NW_BUILD_EXE
#define NW_API __declspec(dllimport)
#endif
#elif defined NW_LINK_STATIC
#define NW_API
#endif
// --==</linking>==--

// --==<configurations>==--
// --window_choice
#define NW_WINDOW_GLFW			1 << 1
#define NW_WINDOW				NW_WINDOW_GLFW
// --window_choice

// --gui_library_choice
#define NW_GUI_NATIVE			1 << 1
#define NW_GUI_IMGUI			1 << 2
#define NW_GUI					NW_GUI_IMGUI | NW_GUI_NATIVE

// --graphics_api_choice
#define NW_GRAPHICS_OGL		1 << 1

#define NW_GRAPHICS NW_GRAPHICS_OGL

#if (NW_WINDOW & NW_WINDOW_GLFW)
	#define GLFW_INCLUDE_NONE
#endif	// NW_WINDOW
#define NW_GLOBAL_MEMORY	1 << 24
// --==</configurations>==--

// --debug_macro
#if (defined NW_DEBUG)
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
	// --opengl_debug
namespace NW
{
	extern inline void OglClearErr();
	extern inline bool OglErrLog(const char* funcName, const char* file, int line);
}
	#define GL_CALL(function) OGL_ClearErr();\
		function;\
		NWL_ASSERT(OglErrLog(#function, NW_FNAME_APATH((std::string)__FILE__), __LINE__), "GL_ERROR: ")
	#endif // NW_GRAPHICS
	#else
	#define GL_CALL(function);
#endif // NW_DEBUG
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
#endif // NW_CORE_HPP

#include <nw_pch.hpp>
#include <nw_def.hpp>

/// 18.10.2020
/// To save your changes in github:
/// --Create repo in your project directory with solution
/// --make .gitingore file where are files you don't want to save
/// --Write in the cmd:
/// -->"git clone {url of your repo}" - copy all files from repository into the curr dir
/// -->"git status" - to get info about files you are working with
/// -->"git add *" - add all files except of .gitignore list
/// -->"git reset ." - remove all files from "download stack"
/// -->"git commit -m {"commentary"}"
/// -->"git push origin master" - push all files in repository