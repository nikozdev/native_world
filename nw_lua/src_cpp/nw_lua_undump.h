/*
** $Id: lua_undump.h $
** load precompiled Lua chunks
** See Copyright Notice in lua_core.h
*/

#ifndef NW_LUA_DUMP_H
#define NW_LUA_DUMP_H

#include "nw_lua_limit.h"
#include "nw_lua_obj.h"
#include "nw_lua_zbuf.h"


/* data to catch conversion errors */
#define NW_LUAC_DATA	"\x19\x93\r\n\x1a\n"

#define NW_LUAC_INT	0x5678
#define NW_LUAC_NUM	cast_num(370.5)

/*
** Encode major-minor version in one byte, one nibble for each
*/
#define MYINT(s)	(s[0]-'0')  /* assume one-digit numerals */
#define NW_LUAC_VERSION	(MYINT(NW_LUA_VERSION_MAJOR)*16+MYINT(NW_LUA_VERSION_MINOR))

#define NW_LUAC_FORMAT	0	/* this is the official format */

/* load one chunk; from lundump.c */
NW_LUAI_FUNC LClosure* luaU_undump (lua_State* L, ZIO* Z, const char* name);

/* dump one chunk; from ldump.c */
NW_LUAI_FUNC int luaU_dump (lua_State* L, const Proto* f, lua_Writer w,
                         void* data, int strip);

#endif  // NW_LUA_DUMP_H
