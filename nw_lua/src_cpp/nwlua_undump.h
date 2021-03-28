/*
** $Id: lua_undump.h $
** load precompiled Lua chunks
** See Copyright Notice in lua_core.h
*/

#ifndef NWLUA_DUMP_H
#define NWLUA_DUMP_H

#include "nwlua_limit.h"
#include "nwlua_obj.h"
#include "nwlua_zbuf.h"


/* data to catch conversion errors */
#define NWLUAC_DATA	"\x19\x93\r\n\x1a\n"

#define NWLUAC_INT	0x5678
#define NWLUAC_NUM	cast_num(370.5)

/*
** Encode major-minor version in one byte, one nibble for each
*/
#define MYINT(s)	(s[0]-'0')  /* assume one-digit numerals */
#define NWLUAC_VERSION	(MYINT(NWLUA_VERSION_MAJOR)*16+MYINT(NWLUA_VERSION_MINOR))

#define NWLUAC_FORMAT	0	/* this is the official format */

/* load one chunk; from lundump.c */
NWLUAI_FUNC LClosure* luaU_undump (lua_State* L, ZIO* Z, const char* name);

/* dump one chunk; from ldump.c */
NWLUAI_FUNC int luaU_dump (lua_State* L, const Proto* f, lua_Writer w,
                         void* data, int strip);

#endif  // NWLUA_DUMP_H