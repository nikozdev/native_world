/*
** $Id: linit.c $
** Initialization of libraries for lua.c and other clients
** See Copyright Notice in lua_core.h
*/


#define linit_c
#define NW_LUA_LIB

/*
** If you embed Lua in your program and need to open the standard
** libraries, call luaL_openlibs in your program. If you need a
** different set of libraries, copy this file to your project and edit
** it to suit your needs.
**
** You can also *preload* libraries, so that a later 'require' can
** open the library, which is already linked to the application.
** For that, do the following code:
**
**  luaL_getsubtable(L, NW_LUA_REGISTRYINDEX, NW_LUA_PRELOAD_TABLE);
**  lua_pushcfunction(L, luaopen_modname);
**  lua_setfield(L, -2, modname);
**  lua_pop(L, 1);  // remove PRELOAD table
*/

#include "nw_lua_prefix.h"


#include <stddef.h>

#include "nw_lua_core.h"

#include "lib/nw_lua_lib.h"
#include "lib/nw_lua_lib_aux.h"


/*
** these libs are loaded by lua.c and are readily available to any Lua
** program
*/
static const luaL_Reg loadedlibs[] = {
  {NW_LUA_GNAME, luaopen_base},
  {NW_LUA_LOADLIBNAME, luaopen_package},
  {NW_LUA_COLIBNAME, luaopen_coroutine},
  {NW_LUA_TABLIBNAME, luaopen_table},
  {NW_LUA_IOLIBNAME, luaopen_io},
  {NW_LUA_OSLIBNAME, luaopen_os},
  {NW_LUA_STRLIBNAME, luaopen_string},
  {NW_LUA_MATHLIBNAME, luaopen_math},
  {NW_LUA_UTF8LIBNAME, luaopen_utf8},
  {NW_LUA_DBLIBNAME, luaopen_debug},
  {NULL, NULL}
};


NW_LUA_API void luaL_openlibs (lua_State *L) {
  const luaL_Reg *lib;
  /* "require" functions from 'loadedlibs' and set results to global table */
  for (lib = loadedlibs; lib->func; lib++) {
    luaL_requiref(L, lib->name, lib->func, 1);
    lua_pop(L, 1);  /* remove lib */
  }
}

