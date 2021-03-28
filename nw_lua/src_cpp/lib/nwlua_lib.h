#ifndef NWLUA_LIB_H
#define NWLUA_LIB_H
#include "nwlua_core.h"
// version suffix for environment variable names
#define NWLUA_VERSUFFIX	"_" NWLUA_VERSION_MAJOR "_" NWLUA_VERSION_MINOR

NWLUAMOD_API int (luaopen_base) (lua_State *L);

#define NWLUA_COLIBNAME	"coroutine"
NWLUAMOD_API int (luaopen_coroutine) (lua_State *L);

#define NWLUA_TABLIBNAME	"table"
NWLUAMOD_API int (luaopen_table) (lua_State *L);

#define NWLUA_IOLIBNAME	"io"
NWLUAMOD_API int (luaopen_io) (lua_State *L);

#define NWLUA_OSLIBNAME	"os"
NWLUAMOD_API int (luaopen_os) (lua_State *L);

#define NWLUA_STRLIBNAME	"string"
NWLUAMOD_API int (luaopen_string) (lua_State *L);

#define NWLUA_UTF8LIBNAME	"utf8"
NWLUAMOD_API int (luaopen_utf8) (lua_State *L);

#define NWLUA_MATHLIBNAME	"math"
NWLUAMOD_API int (luaopen_math) (lua_State *L);

#define NWLUA_DBLIBNAME	"debug"
NWLUAMOD_API int (luaopen_debug) (lua_State *L);

#define NWLUA_LOADLIBNAME	"package"
NWLUAMOD_API int (luaopen_package) (lua_State *L);

// open all previous libraries
NWLUA_API void (luaL_openlibs) (lua_State *L);

#if !defined(lua_assert)
#	define lua_assert(x)	((void)0)
#endif

#endif	// NWLUALIB_H
