#ifndef CHEER_NIK_CORE_H
#define CHEER_NIK_CORE_H
/// This is the main header of the engine
/// Here are all relevant macros and configurations

/// --------<Platforms>--------
#ifdef CN_PLATFORM_WINDOWS
	#ifdef CN_BUILD_DLL
		#define CN_API __declspec(dllexport)
	#else
		#define CN_API __declspec(dllimport)
	#endif
#else
	//#error CheerNik cannot work with this platform
#endif
/// --------</Platforms>--------

/// --------<Configurations>--------
#define USE_EVSYS
#define CN_ENABLE_DEBUG
/// --------</Configurations>--------

/// --------<TypeDefs>--------
typedef unsigned int UInt;
typedef unsigned short UShort;
typedef unsigned char UChar;
typedef unsigned char UChar;
/// --------</TypeDefs>--------

/// --------<SupportMacroses>--------
///Translate anything to the C string(char array)
#define CN_CSTR(anything) (#anything)
/// Translate anything to the string
#define CN_STR(anything) (std::string(#anything))
/// Formating of text same as printf()
#define CN_STR_FORM(format, args);
/// Get directory of cpp file
#define CN_STR_PART_L(str, symb) ( ( str.substr(0, str.rfind(symb) + 1) ).c_str() )
/// Get name of cpp file
#define CN_STR_PART_R(str, symb) ( ( str.substr(str.rfind(symb) + 1, str.size()) ).c_str() )
/// Get file name from absolute path
#define CN_FNAME_APATH(path) ( CN_STR_PART_R(std::string(path), '\\') )
/// Get file directory from absolute path
#define CN_FDIR_APATH(path) ( CN_STR_PART_L(std::string(path), '\\') )

// Debug macro
#ifdef CN_ENABLE_DEBUG
	#define CN_LOG(msg) ( CN_LOG_LC(CN_FNAME_APATH(__FILE__), msg) )
	#define CN_LOG_LC(loc, what) (std::cout << "LOG::[" << loc << "]::LINE_" << __LINE__ << ":\n" << what << std::endl)
	#define CN_ASSERT(expr, comment) if (!expr) { std::cout << \
		"CN::ASSERT::" << __FILE__ << "::LINE_" << __LINE__ << ":\n" << comment << std::endl; \
		__debugbreak(); }
	#define CN_ERR(comment) CN_ASSERT(false, comment);
//OpenGL debug
extern void GL_clearError();
extern bool GL_log(const char* funcName, const char* file, int line);
	#define GL_CALL(function) GL_clearError();\
	function;\
	CN_ASSERT(GL_log(#function, CN_FNAME_APATH((std::string)__FILE__), __LINE__), "GL_ERROR: ")

#else
	#define CN_LOG(loc, what);
	#define CN_LOG_LC(loc, what);
	#define CN_ASSERT(expr, comment);
	#define CN_ERR(function);

	#define GL_CALL(function);
#endif

#define BIT_MV(num) 1 << num
/// --------</SupportMacroses>--------

/// --------<SupportFunctions>--------
/// --------</SupportFunctions>--------

#endif

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