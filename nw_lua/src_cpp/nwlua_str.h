#ifndef NWLUA_STRING_H
#define NWLUA_STRING_H

#include "nwlua_gc.h"
#include "nwlua_obj.h"
#include "nwlua_stt.h"

/*
** Memory-allocation error message must be preallocated (it cannot
** be created after memory is exhausted)
*/
#define MEMERRMSG       "not enough memory"


/*
** Size of a TString: Size of the header plus space for the string
** itself (including final '\0').
*/
#define sizelstring(l)  (offsetof(TString, contents) + ((l) + 1) * sizeof(char))

#define luaS_newliteral(L, s)	(luaS_newlstr(L, "" s, \
                                 (sizeof(s)/sizeof(char))-1))


/*
** test whether a string is a reserved word
*/
#define isreserved(s)	((s)->tt == NWLUA_VSHRSTR && (s)->extra > 0)


/*
** equality for short strings, which are always internalized
*/
#define eqshrstr(a,b)	check_exp((a)->tt == NWLUA_VSHRSTR, (a) == (b))


NWLUAI_FUNC unsigned int luaS_hash (const char *str, size_t l, unsigned int seed);
NWLUAI_FUNC unsigned int luaS_hashlongstr (TString *ts);
NWLUAI_FUNC int luaS_eqlngstr (TString *a, TString *b);
NWLUAI_FUNC void luaS_resize (lua_State *L, int newsize);
NWLUAI_FUNC void luaS_clearcache (global_State *g);
NWLUAI_FUNC void luaS_init (lua_State *L);
NWLUAI_FUNC void luaS_remove (lua_State *L, TString *ts);
NWLUAI_FUNC Udata *luaS_newudata (lua_State *L, size_t s, int nuvalue);
NWLUAI_FUNC TString *luaS_newlstr (lua_State *L, const char *str, size_t l);
NWLUAI_FUNC TString *luaS_new (lua_State *L, const char *str);
NWLUAI_FUNC TString *luaS_createlngstrobj (lua_State *L, size_t l);

#endif  // NWLUASTRING_H