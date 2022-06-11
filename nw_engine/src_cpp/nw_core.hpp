#ifndef NW_CORE_H
#define NW_CORE_H
/// This is the main header of the engine
/// Here are all relevant macros and configurations

// --------<Linking&Platforms>--------
#if defined NW_PLATFORM_WINDOWS
    #if defined NW_LINK_DYNAMIC
        #ifdef NW_BUILD_DLL
            #define NW_API __declspec(dllexport)
	    #elif defined NW_BUILD_EXE
		    #define NW_API __declspec(dllimport)
        #endif
    #elif defined NW_LINK_STATIC
		#define NW_API
    #endif
#else
	//#error CheerNik cannot work with this platform
#endif
// --------</Linking&Platforms>--------

// --------<Configurations>--------
// --Window choice
#define NW_WINDOW_CONSOLE		1 << 1	// 0b0001
#define NW_WINDOW_WIN			1 << 2	// 0b0010
#define NW_WINDOW_GLFW			1 << 3	// 0b0100

#define NW_WINDOW				NW_WINDOW_GLFW
// --Window choice

// --Mathematics library choice
#define NW_MATH_NATIVE			1 << 1	// 0b0001
#define NW_MATH_GLM				1 << 2	// 0b0010
#define NW_MATH					NW_MATH_GLM
// --Mathematics library choice

// --Gui library choice
#define NW_GUI_NATIVE			1 << 2
#define NW_GUI_COUT				1 << 1
#define NW_GUI_IMGUI			1 << 3
#define NW_GUI					NW_GUI_IMGUI | NW_GUI_NATIVE
// --Gui library choice

// --Graphics choice
#define NW_GRAPHICS_COUT 0b0001
#define NW_GRAPHICS_WIN 0b0010
#define NW_GRAPHICS_OGL 0b0100
#define NW_GRAPHICS_DX 0b1000

#define NW_GRAPHICS NW_GRAPHICS_OGL
// --Graphics choice

// --Graphics Library Choice
#if (NW_GRAPHICS & NW_GRAPHICS_WIN)
	#define NW_GL_USE_NATIVE
#elif (NW_GRAPHICS & NW_GRAPHICS_WIN)
	#define NW_GL_USE_WINAPI
#elif (NW_GRAPHICS & NW_GRAPHICS_OGL)
	#define NW_GL_USE_GLAD
#endif // NW_GL_USE
// --Graphics Library Choice

#if (NW_WINDOW & NW_WINDOW_GLFW)
	#define GLFW_INCLUDE_NONE
#endif	// NW_WINDOW

// --Standard libraries choice
#define NW_LIBS_NATIVE_PTR			0b0000'0001
#define NW_LIBS_STD_PTR				0b0000'0010

#define NW_LIBS_NATIVE_STR			0b0000'0100
#define NW_LIBS_STD_STR				0b0000'1000

#define NW_LIBS_NATIVE_COLLECTION	0b0001'0000
#define NW_LIBS_STD_COLLECTION		0b0010'0000

#define NW_LIBS (NW_LIBS_STD_PTR | NW_LIBS_STD_STR | NW_LIBS_NATIVE_STR | NW_LIBS_STD_COLLECTION)
// --Standard libraries choice

// --------</Configurations>--------

// --------<SupportMacroses>--------
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

// --Debug macro
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
	// --OpenGL debug
namespace NW
{
	extern inline void OGL_ClearErr();
	extern inline bool OGL_ErrLog(const char* funcName, const char* file, int line);
}
	#define GL_CALL(function) OGL_ClearErr();\
		function;\
		NW_ASSERT(OGL_ErrLog(#function, NW_FNAME_APATH((std::string)__FILE__), __LINE__), "GL_ERROR: ")
	// --OpenGL debug
	#endif // NW_GRAPHICS
#else
	#define NW_LOG(loc, what);
	#define NW_LOG_LC(loc, what);
	#define NW_ASSERT(expr, comment);
	#define NW_BREAK();
	#define NW_ERR(function);

	#define GL_CALL(function);
#endif // NW_DEBUG
// --Debug Macro

#define NW_BIND_FN(func) (std::bind(&func, this, std::placeholders::_1))
// --------</SupportMacroses>--------

// --------<Pragmas>--------
#pragma warning(disable : 4005)
#pragma warning(disable : 4081)
#pragma warning(disable : 4099)
#pragma warning(disable : 4102)
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4552)
#pragma warning(disable : 4996)
// --------</Pragmas>--------
#endif // NW_CORE_H

// --native_world library
#include <nw_def.hpp>
// --native_world library

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