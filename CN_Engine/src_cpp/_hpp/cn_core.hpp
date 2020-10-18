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
#define CN_LOG(loc) ("LOG::" + #loc + "::LINE_" + __LINE__ + ":\n")

#define CN_ENABLE_ASSERTIONS
#ifdef CN_ENABLE_ASSERTIONS
	#define CN_ASSERT(expr, comment) if (!expr) { \
		printf("CN::CORE::ASSERT::LINE_%d: %s\n", __LINE__, comment); \
		__debugbreak(); }
#else
	#define CN_ASSET (x, ...)
#endif

#define BIT_MV(num) num << 1
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