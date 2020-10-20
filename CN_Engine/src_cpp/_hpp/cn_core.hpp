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
/// --------</Configurations>--------

/// --------<TypeDefs>--------
typedef unsigned int UInt;
typedef unsigned short UShort;
typedef unsigned char UChar;
typedef unsigned char UChar;
/// --------</TypeDefs>--------

/// --------<SupportMacroses>--------
#define CN_CSTR(anything) (#anything)
#define CN_STR(anything) (std::string(#anything))

#define CN_ENABLE_DEBUG
#ifdef CN_ENABLE_DEBUG
	#define CN_LOG(loc, what) (std::cout << "LOG::" << loc << "::LINE_" << __LINE__ << ":\n" << what << std::endl)
	#define CN_ASSERT(expr, comment) if (!expr) { std::cout << \
		"CN::ASSERT::" << __FILE__ << "::LINE_" << __LINE__ << ":\n" << comment << std::endl; \
		__debugbreak(); }
#else
	#define CN_LOG(loc, what) std::cout << std::endl;
	#define CN_ASSERT(expr, comment) std::cout << std::endl;
#endif

#define BIT_MV(num) 1 << num
/// --------</SupportMacroses>--------

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