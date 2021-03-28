#ifndef NWLUA_DEBUG_H
#define NWLUA_DEBUG_H

#include "nwlua_stt.h"

#define pcRel(pc, p)	(cast_int((pc) - (p)->code) - 1)

/* Active Lua function (given call info) */
#define ci_func(ci)		(clLvalue(s2v((ci)->func)))


#define resethookcount(L)	(L->hookcount = L->basehookcount)

// mark for entries in 'lineinfo' array that has absolute information in
// 'abslineinfo' array
#define ABSLINEINFO	(-0x80)

NWLUAI_FUNC int luaG_getfuncline (const Proto *f, int pc);
NWLUAI_FUNC const char *luaG_findlocal (lua_State *L, CallInfo *ci, int n,
                                                    StkId *pos);
NWLUAI_FUNC l_noret luaG_typeerror (lua_State *L, const TValue *o,
                                                const char *opname);
NWLUAI_FUNC l_noret luaG_forerror (lua_State *L, const TValue *o,
                                               const char *what);
NWLUAI_FUNC l_noret luaG_concaterror (lua_State *L, const TValue *p1,
                                                  const TValue *p2);
NWLUAI_FUNC l_noret luaG_opinterror (lua_State *L, const TValue *p1,
                                                 const TValue *p2,
                                                 const char *msg);
NWLUAI_FUNC l_noret luaG_tointerror (lua_State *L, const TValue *p1,
                                                 const TValue *p2);
NWLUAI_FUNC l_noret luaG_ordererror (lua_State *L, const TValue *p1,
                                                 const TValue *p2);
NWLUAI_FUNC l_noret luaG_runerror (lua_State *L, const char *fmt, ...);
NWLUAI_FUNC const char *luaG_addinfo (lua_State *L, const char *msg,
                                                  TString *src, int line);
NWLUAI_FUNC l_noret luaG_errormsg (lua_State *L);
NWLUAI_FUNC int luaG_traceexec (lua_State *L, const Instruction *pc);

#endif  // NWLUADEBUG_H