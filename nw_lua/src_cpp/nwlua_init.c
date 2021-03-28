/*
** $Id: linit.c $
** Initialization of libraries for lua.c and other clients
** See Copyright Notice in lua_core.h
*/


#define linit_c
#define NWLUA_LIB

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
**  luaL_getsubtable(L, NWLUA_REGISTRYINDEX, NWLUA_PRELOAD_TABLE);
**  lua_pushcfunction(L, luaopen_modname);
**  lua_setfield(L, -2, modname);
**  lua_pop(L, 1);  // remove PRELOAD table
*/

#include "nwlua_prefix.h"


#include <stddef.h>

#include "nwlua_core.h"

#include "lib/nwlua_lib.h"
#include "lib/nwlua_lib_aux.h"


/*
** these libs are loaded by lua.c and are readily available to any Lua
** program
*/
static const luaL_Reg loadedlibs[] = {
  {NWLUA_GNAME, luaopen_base},
  {NWLUA_LOADLIBNAME, luaopen_package},
  {NWLUA_COLIBNAME, luaopen_coroutine},
  {NWLUA_TABLIBNAME, luaopen_table},
  {NWLUA_IOLIBNAME, luaopen_io},
  {NWLUA_OSLIBNAME, luaopen_os},
  {NWLUA_STRLIBNAME, luaopen_string},
  {NWLUA_MATHLIBNAME, luaopen_math},
  {NWLUA_UTF8LIBNAME, luaopen_utf8},
  {NWLUA_DBLIBNAME, luaopen_debug},
  {NULL, NULL}
};


NWLUA_API void luaL_openlibs (lua_State *L) {
  const luaL_Reg *lib;
  /* "require" functions from 'loadedlibs' and set results to global table */
  for (lib = loadedlibs; lib->func; lib++) {
    luaL_requiref(L, lib->name, lib->func, 1);
    lua_pop(L, 1);  /* remove lib */
  }
}

