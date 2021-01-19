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

// --==<support_macroses>==--
///Translate anything to the C string(char array)
#define NW_CSTR(anything) (#anything)
/// Translate anything to the string
#define NW_STR(anything) (std::string(anything))
/// Formating of text same as printf()
#define NW_STR_FORM(format, args);
/// Get directory of cpp file
#define NW_STR_PART_L(str, symb) ( ( str.substr(0, str.rfind(symb) + 1) ).c_str() )
/// Get name of cpp file
#define NW_STR_PART_R(str, symb) ( ( str.substr(str.rfind(symb) + 1, str.size()) ).c_str() )
/// Get file name from absolute path
#define NW_FNAME_APATH(path) ( NW_STR_PART_R(std::string(path), '\\') )
/// Get file directory from absolute path
#define NW_FDIR_APATH(path) ( NW_STR_PART_L(std::string(path), '\\') )

// --debug_macro
// -->__VA_ARGS__ - variadic agruments = arguments given as ...
#if (defined NW_DEBUG)
	#define NW_BREAK() __debugbreak();
	/// Print the calling location and the message
	/// --Writes LOG::[location]::LINE_{calling_line}:
	///	====< msg >====\n
	#define NW_LOG(msg) NW_LOG_LOC(NW_FNAME_APATH(__FILE__));			\
		std::cout << "====< " << msg << " >====\n";
	/// --Takes format string where [param] means next string
	/// --Example: NW_LOG_F("Format message [param] sometext", nextString)
	/// --Writes: LOG::[FILE_NAME]::LILE_[CURR_LINE]:
	///	====< Format message nextString sometext >====
	#define NW_LOG_F(format, ...) {										\
		const char* msg = format; int i = 0;							\
		char ch = 'a';	std::stringstream sStr;							\
		while(ch != '\0') { ch = msg[i];								\
			if (ch == '{') while(true) { ch = msg[i + 1];				\
				if (ch == '\0') break;									\
				else if (ch == '}'){sStr << __VA_ARGS__; break;}		\
				i++; }													\
			else {sStr << msg[i];}										\
		i++; }															\
		NW_LOG(sStr.str());												\
	}
	/// --Writes LOG::[location]::LINE_{calling_line}:\n
	#define NW_LOG_LOC(loc) std::cout << "LOG::[" << loc <<				\
			"]::LINE_" << __LINE__ << std::endl
	#define NW_ASSERT(expr, comment) if (!(expr)) { std::cout <<		\
		"NW::ASSERT::[" << __FILE__ <<									\
		"]::LINE_" << __LINE__ << ":\n" << comment << ">>\n";			\
	NW_BREAK(); }
	#define NW_ERR(comment) NW_ASSERT(false, comment);

	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
	// --opengl_debug
namespace NW
{
	extern inline void OglClearErr();
	extern inline bool OglErrLog(const char* funcName, const char* file, int line);
}
	#define GL_CALL(function) OGL_ClearErr();\
		function;\
		NW_ASSERT(OglErrLog(#function, NW_FNAME_APATH((std::string)__FILE__), __LINE__), "GL_ERROR: ")
	#endif // NW_GRAPHICS
#else
	#define NW_LOG(loc, what);
	#define NW_LOG_LC(loc, what);
	#define NW_BREAK();
	#define NW_ASSERT(expr, comment);
	#define NW_ERR(function);

	#define GL_CALL(function);
#endif // NW_DEBUG

#define NW_BIND_FN(func) (std::bind(&func, this, std::placeholders::_1))
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