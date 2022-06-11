#ifndef NW_LUA_LIB_H
#define NW_LUA_LIB_H
#include "nw_lua_core.h"
// version suffix for environment variable names
#define NW_LUA_VERSUFFIX	"_" NW_LUA_VERSION_MAJOR "_" NW_LUA_VERSION_MINOR

NW_LUAMOD_API int (luaopen_base) (lua_State *L);

#define NW_LUA_COLIBNAME	"coroutine"
NW_LUAMOD_API int (luaopen_coroutine) (lua_State *L);

#define NW_LUA_TABLIBNAME	"table"
NW_LUAMOD_API int (luaopen_table) (lua_State *L);

#define NW_LUA_IOLIBNAME	"io"
NW_LUAMOD_API int (luaopen_io) (lua_State *L);

#define NW_LUA_OSLIBNAME	"os"
NW_LUAMOD_API int (luaopen_os) (lua_State *L);

#define NW_LUA_STRLIBNAME	"string"
NW_LUAMOD_API int (luaopen_string) (lua_State *L);

#define NW_LUA_UTF8LIBNAME	"utf8"
NW_LUAMOD_API int (luaopen_utf8) (lua_State *L);

#define NW_LUA_MATHLIBNAME	"math"
NW_LUAMOD_API int (luaopen_math) (lua_State *L);

#define NW_LUA_DBLIBNAME	"debug"
NW_LUAMOD_API int (luaopen_debug) (lua_State *L);

#define NW_LUA_LOADLIBNAME	"package"
NW_LUAMOD_API int (luaopen_package) (lua_State *L);

// open all previous libraries
NW_LUA_API void (luaL_openlibs) (lua_State *L);

#if !defined(lua_assert)
#	define lua_assert(x)	((void)0)
#endif

#endif	// NW_LUALIB_H
