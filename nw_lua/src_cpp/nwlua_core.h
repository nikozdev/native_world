#ifndef NWLUA_CORE_H
#define NWLUA_CORE_H

#include <stdarg.h>
#include <stddef.h>

#include "nwlua_config.h"
// lua information
#define NWLUA_VERSION_MAJOR	"5"
#define NWLUA_VERSION_MINOR	"4"
#define NWLUA_VERSION_RELEASE	"2"
#define NWLUA_VERSION_NUM			    504
#define NWLUA_VERSION_RELEASE_NUM		(NWLUA_VERSION_NUM * 100 + 0)
#define NWLUA_VERSION	"Lua " NWLUA_VERSION_MAJOR "." NWLUA_VERSION_MINOR
#define NWLUA_RELEASE	NWLUA_VERSION "." NWLUA_VERSION_RELEASE
#define NWLUA_COPYRIGHT	NWLUA_RELEASE "  Copyright (C) 1994-2020 Lua.org, PUC-Rio"
#define NWLUA_AUTHORS	"R. Ierusalimschy, L. H. de Figueiredo, W. Celes"
// signatures for compiled code
#define NWLUA_SIGNATURE	"\x1bLua"
// option for multiple returns in 'lua_pcall' and 'lua_call'
#define NWLUA_MULTRET	(-1)
// pseudo-indices
// (-NWLUAI_MAXSTACK is the minimum valid index;
// we keep some free empty space after that to help overflow detection)
#define NWLUA_REGISTRYINDEX	(-NWLUAI_MAXSTACK - 1000)
#define lua_upvalueindex(i)	(NWLUA_REGISTRYINDEX - (i))
// thread status
#define NWLUA_OK		0
#define NWLUA_YIELD	1
#define NWLUA_ERRRUN	2
#define NWLUA_ERRSYNTAX	3
#define NWLUA_ERRMEM	4
#define NWLUA_ERRERR	5

typedef struct lua_State lua_State;

// basic types
#define NWLUA_TNONE		(-1)

#define NWLUA_TNIL		0
#define NWLUA_TBOOLEAN		1
#define NWLUA_TLIGHTUSERDATA	2
#define NWLUA_TNUMBER		3
#define NWLUA_TSTRING		4
#define NWLUA_TTABLE		5
#define NWLUA_TFUNCTION		6
#define NWLUA_TUSERDATA		7
#define NWLUA_TTHREAD		8

#define NWLUA_NUMTYPES		9

// minimum Lua stack available to a C function
#define NWLUA_MINSTACK	20
// predefined values in the registry
#define NWLUA_RIDX_MAINTHREAD	1
#define NWLUA_RIDX_GLOBALS	2
#define NWLUA_RIDX_LAST		NWLUA_RIDX_GLOBALS
// type of numbers in lua
typedef NWLUA_NUMBER lua_Number;
// type for integer functions
typedef NWLUA_INTEGER lua_Integer;
// unsigned integer type
typedef NWLUA_UNSIGNED lua_Unsigned;
// type for continuation-function contexts
typedef NWLUA_KCONTEXT lua_KContext;

// type for C functions registered with lua
typedef int (*lua_CFunction) (lua_State *L);
// type for continuation functions
typedef int (*lua_KFunction) (lua_State *L, int status, lua_KContext ctx);
// type for functions that read/write blocks when loading/dumping Lua chunks
typedef const char * (*lua_Reader) (lua_State *L, void *ud, size_t *sz);
typedef int (*lua_Writer) (lua_State *L, const void *p, size_t sz, void *ud);
// type for memory-allocation functions
typedef void * (*lua_Alloc) (void *ud, void *ptr, size_t osize, size_t nsize);
// type for warning functions
typedef void (*lua_WarnFunction) (void *ud, const char *msg, int tocont);
// generic extra include file
#if defined(NWLUA_USER_H)
#   include NWLUA_USER_H
#endif
// rcs ident string
extern const char lua_ident[];
// state manipulation
NWLUA_API lua_State *(lua_newstate) (lua_Alloc f, void *ud);
NWLUA_API void       (lua_close) (lua_State *L);
NWLUA_API lua_State *(lua_newthread) (lua_State *L);
NWLUA_API int        (lua_resetthread) (lua_State *L);
// error callback function
NWLUA_API lua_CFunction (lua_atpanic) (lua_State *L, lua_CFunction panicf);
// version getter
NWLUA_API lua_Number (lua_version) (lua_State *L);
// basic stack manipulation
NWLUA_API int   (lua_absindex) (lua_State *L, int idx);
NWLUA_API int   (lua_gettop) (lua_State *L);
NWLUA_API void  (lua_settop) (lua_State *L, int idx);
NWLUA_API void  (lua_pushvalue) (lua_State *L, int idx);
NWLUA_API void  (lua_rotate) (lua_State *L, int idx, int n);
NWLUA_API void  (lua_copy) (lua_State *L, int fromidx, int toidx);
NWLUA_API int   (lua_checkstack) (lua_State *L, int n);
NWLUA_API void  (lua_xmove) (lua_State *from, lua_State *to, int n);
// getters (stack -> C)
NWLUA_API int             (lua_isnumber) (lua_State *L, int idx);
NWLUA_API int             (lua_isstring) (lua_State *L, int idx);
NWLUA_API int             (lua_iscfunction) (lua_State *L, int idx);
NWLUA_API int             (lua_isinteger) (lua_State *L, int idx);
NWLUA_API int             (lua_isuserdata) (lua_State *L, int idx);
NWLUA_API int             (lua_type) (lua_State *L, int idx);
NWLUA_API const char     *(lua_typename) (lua_State *L, int tp);
NWLUA_API lua_Number      (lua_tonumberx) (lua_State *L, int idx, int *isnum);
NWLUA_API lua_Integer     (lua_tointegerx) (lua_State *L, int idx, int *isnum);
NWLUA_API int             (lua_toboolean) (lua_State *L, int idx);
NWLUA_API const char     *(lua_tolstring) (lua_State *L, int idx, size_t *len);
NWLUA_API lua_Unsigned    (lua_rawlen) (lua_State *L, int idx);
NWLUA_API lua_CFunction   (lua_tocfunction) (lua_State *L, int idx);
NWLUA_API void	       *(lua_touserdata) (lua_State *L, int idx);
NWLUA_API lua_State      *(lua_tothread) (lua_State *L, int idx);
NWLUA_API const void     *(lua_topointer) (lua_State *L, int idx);
// comparison and arithmetic functions
#define NWLUA_OPADD	0	/* ORDER TM, ORDER OP */
#define NWLUA_OPSUB	1
#define NWLUA_OPMUL	2
#define NWLUA_OPMOD	3
#define NWLUA_OPPOW	4
#define NWLUA_OPDIV	5
#define NWLUA_OPIDIV	6
#define NWLUA_OPBAND	7
#define NWLUA_OPBOR	8
#define NWLUA_OPBXOR	9
#define NWLUA_OPSHL	10
#define NWLUA_OPSHR	11
#define NWLUA_OPUNM	12
#define NWLUA_OPBNOT	13

NWLUA_API void  (lua_arith) (lua_State *L, int op);

#define NWLUA_OPEQ	0
#define NWLUA_OPLT	1
#define NWLUA_OPLE	2

NWLUA_API int   (lua_rawequal) (lua_State *L, int idx1, int idx2);
NWLUA_API int   (lua_compare) (lua_State *L, int idx1, int idx2, int op);

// setters (C -> stack)
NWLUA_API void        (lua_pushnil) (lua_State *L);
NWLUA_API void        (lua_pushnumber) (lua_State *L, lua_Number n);
NWLUA_API void        (lua_pushinteger) (lua_State *L, lua_Integer n);
NWLUA_API const char *(lua_pushlstring) (lua_State *L, const char *s, size_t len);
NWLUA_API const char *(lua_pushstring) (lua_State *L, const char *s);
NWLUA_API const char *(lua_pushvfstring) (lua_State *L, const char *fmt,
                                                      va_list argp);
NWLUA_API const char *(lua_pushfstring) (lua_State *L, const char *fmt, ...);
NWLUA_API void  (lua_pushcclosure) (lua_State *L, lua_CFunction fn, int n);
NWLUA_API void  (lua_pushboolean) (lua_State *L, int b);
NWLUA_API void  (lua_pushlightuserdata) (lua_State *L, void *p);
NWLUA_API int   (lua_pushthread) (lua_State *L);
// getters (Lua -> stack)
NWLUA_API int (lua_getglobal) (lua_State *L, const char *name);
NWLUA_API int (lua_gettable) (lua_State *L, int idx);
NWLUA_API int (lua_getfield) (lua_State *L, int idx, const char *k);
NWLUA_API int (lua_geti) (lua_State *L, int idx, lua_Integer n);
NWLUA_API int (lua_rawget) (lua_State *L, int idx);
NWLUA_API int (lua_rawgeti) (lua_State *L, int idx, lua_Integer n);
NWLUA_API int (lua_rawgetp) (lua_State *L, int idx, const void *p);
NWLUA_API void  (lua_createtable) (lua_State *L, int narr, int nrec);
NWLUA_API void *(lua_newuserdatauv) (lua_State *L, size_t sz, int nuvalue);
NWLUA_API int   (lua_getmetatable) (lua_State *L, int objindex);
NWLUA_API int  (lua_getiuservalue) (lua_State *L, int idx, int n);
// setters (stack -> Lua)
NWLUA_API void  (lua_setglobal) (lua_State *L, const char *name);
NWLUA_API void  (lua_settable) (lua_State *L, int idx);
NWLUA_API void  (lua_setfield) (lua_State *L, int idx, const char *k);
NWLUA_API void  (lua_seti) (lua_State *L, int idx, lua_Integer n);
NWLUA_API void  (lua_rawset) (lua_State *L, int idx);
NWLUA_API void  (lua_rawseti) (lua_State *L, int idx, lua_Integer n);
NWLUA_API void  (lua_rawsetp) (lua_State *L, int idx, const void *p);
NWLUA_API int   (lua_setmetatable) (lua_State *L, int objindex);
NWLUA_API int   (lua_setiuservalue) (lua_State *L, int idx, int n);
// 'load' and 'call' functions (load and run Lua code)
NWLUA_API void  (lua_callk) (lua_State *L, int nargs, int nresults, lua_KContext ctx, lua_KFunction k);
#define lua_call(L,n,r)		lua_callk(L, (n), (r), 0, NULL)
NWLUA_API int   (lua_pcallk) (lua_State *L, int nargs, int nresults, int errfunc, lua_KContext ctx, lua_KFunction k);
#define lua_pcall(L,n,r,f)	lua_pcallk(L, (n), (r), (f), 0, NULL)
NWLUA_API int   (lua_load) (lua_State *L, lua_Reader reader, void *dt, const char *chunkname, const char *mode);
NWLUA_API int (lua_dump) (lua_State *L, lua_Writer writer, void *data, int strip);
// coroutine functions
NWLUA_API int  (lua_yieldk)     (lua_State *L, int nresults, lua_KContext ctx, lua_KFunction k);
NWLUA_API int  (lua_resume)     (lua_State *L, lua_State *from, int narg, int *nres);
NWLUA_API int  (lua_status)     (lua_State *L);
NWLUA_API int (lua_isyieldable) (lua_State *L);
#define lua_yield(L,n)		lua_yieldk(L, (n), 0, NULL)
// warning-related functions
NWLUA_API void (lua_setwarnf) (lua_State *L, lua_WarnFunction f, void *ud);
NWLUA_API void (lua_warning)  (lua_State *L, const char *msg, int tocont);
// garbage-collection function and options
#define NWLUA_GCSTOP		0
#define NWLUA_GCRESTART		1
#define NWLUA_GCCOLLECT		2
#define NWLUA_GCCOUNT		3
#define NWLUA_GCCOUNTB		4
#define NWLUA_GCSTEP		5
#define NWLUA_GCSETPAUSE		6
#define NWLUA_GCSETSTEPMUL	7
#define NWLUA_GCISRUNNING		9
#define NWLUA_GCGEN		10
#define NWLUA_GCINC		11

NWLUA_API int (lua_gc) (lua_State *L, int what, ...);

// miscellaneous functions
NWLUA_API int   (lua_error) (lua_State *L);
NWLUA_API int   (lua_next) (lua_State *L, int idx);
NWLUA_API void  (lua_concat) (lua_State *L, int n);
NWLUA_API void  (lua_len)    (lua_State *L, int idx);
NWLUA_API size_t   (lstringtonumber) (lua_State *L, const char *s);
NWLUA_API lua_Alloc (lua_getallocf) (lua_State *L, void **ud);
NWLUA_API void      (lua_setallocf) (lua_State *L, lua_Alloc f, void *ud);
NWLUA_API void  (lua_toclose) (lua_State *L, int idx);

// --==<useful_macros>==--
#define lua_getextraspace(L)	((void *)((char *)(L) - NWLUA_EXTRASPACE))

#define lua_tonumber(L,i)	lua_tonumberx(L,(i),NULL)
#define lua_tointeger(L,i)	lua_tointegerx(L,(i),NULL)

#define lua_pop(L,n)		lua_settop(L, -(n)-1)

#define lua_newtable(L)		lua_createtable(L, 0, 0)

#define lua_register(L,n,f) (lua_pushcfunction(L, (f)), lua_setglobal(L, (n)))

#define lua_pushcfunction(L,f)	lua_pushcclosure(L, (f), 0)

#define lua_isfunction(L,n)	(lua_type(L, (n)) == NWLUA_TFUNCTION)
#define lua_istable(L,n)	(lua_type(L, (n)) == NWLUA_TTABLE)
#define lua_islightuserdata(L,n)	(lua_type(L, (n)) == NWLUA_TLIGHTUSERDATA)
#define lua_isnil(L,n)		(lua_type(L, (n)) == NWLUA_TNIL)
#define lua_isboolean(L,n)	(lua_type(L, (n)) == NWLUA_TBOOLEAN)
#define lua_isthread(L,n)	(lua_type(L, (n)) == NWLUA_TTHREAD)
#define lua_isnone(L,n)		(lua_type(L, (n)) == NWLUA_TNONE)
#define lua_isnoneornil(L, n)	(lua_type(L, (n)) <= 0)

#define lua_pushliteral(L, s)	lua_pushstring(L, "" s)

#define lua_pushglobalua_tab(L)  \
	((void)lua_rawgeti(L, NWLUA_REGISTRYINDEX, NWLUA_RIDX_GLOBALS))

#define lua_tostring(L,i)	lua_tolstring(L, (i), NULL)


#define lua_insert(L,idx)	lua_rotate(L, (idx), 1)

#define lua_remove(L,idx)	(lua_rotate(L, (idx), -1), lua_pop(L, 1))

#define lua_replace(L,idx)	(lua_copy(L, -1, (idx)), lua_pop(L, 1))
// --==</useful_macros>==--


// --==<compatibility_macros>==--
#if defined(NWLUA_COMPAT_APIINTCASTS)

#define lua_pushunsigned(L,n)	lua_pushinteger(L, (lua_Integer)(n))
#define lua_tounsignedx(L,i,is)	((lua_Unsigned)lua_tointegerx(L,i,is))
#define lua_tounsigned(L,i)	lua_tounsignedx(L,(i),NULL)

#endif

#define lua_newuserdata(L,s)	lua_newuserdatauv(L,s,1)
#define lua_getuservalue(L,idx)	lua_getiuservalue(L,idx,1)
#define lua_setuservalue(L,idx)	lua_setiuservalue(L,idx,1)

#define NWLUA_NUMTAGS		NWLUA_NUMTYPES
// --==</compatibility_macros>==--

// --==<debug_api>==--
// event codes
#define NWLUA_HOOKCALL	0
#define NWLUA_HOOKRET	1
#define NWLUA_HOOKLINE	2
#define NWLUA_HOOKCOUNT	3
#define NWLUA_HOOKTAILCALL 4
// event masks
#define NWLUA_MASKCALL	(1 << NWLUA_HOOKCALL)
#define NWLUA_MASKRET	(1 << NWLUA_HOOKRET)
#define NWLUA_MASKLINE	(1 << NWLUA_HOOKLINE)
#define NWLUA_MASKCOUNT	(1 << NWLUA_HOOKCOUNT)

typedef struct lua_Debug lua_Debug;  // activation record

// functions to be called by the debugger in specific events
typedef void (*lua_Hook) (lua_State *L, lua_Debug *ar);

NWLUA_API int (lua_getstack) (lua_State *L, int level, lua_Debug *ar);
NWLUA_API int (lua_getinfo) (lua_State *L, const char *what, lua_Debug *ar);
NWLUA_API const char *(lua_getlocal) (lua_State *L, const lua_Debug *ar, int n);
NWLUA_API const char *(lua_setlocal) (lua_State *L, const lua_Debug *ar, int n);
NWLUA_API const char *(lua_getupvalue) (lua_State *L, int funcindex, int n);
NWLUA_API const char *(lua_setupvalue) (lua_State *L, int funcindex, int n);

NWLUA_API void *(lua_upvalueid) (lua_State *L, int fidx, int n);
NWLUA_API void  (lua_upvaluejoin) (lua_State *L, int fidx1, int n1,
                                               int fidx2, int n2);

NWLUA_API void (lua_sethook) (lua_State *L, lua_Hook func, int mask, int count);
NWLUA_API lua_Hook (lua_gethook) (lua_State *L);
NWLUA_API int (lua_gethookmask) (lua_State *L);
NWLUA_API int (lua_gethookcount) (lua_State *L);

NWLUA_API int (lua_setcstacklimit) (lua_State *L, unsigned int limit);

struct lua_Debug {
    int event;
    const char* name;	/* (n) */
    const char* namewhat;	/* (n) 'global', 'local', 'field', 'method' */
    const char* what;	/* (S) 'Lua', 'C', 'main', 'tail' */
    const char* source;	/* (S) */
    size_t srclen;	/* (S) */
    int currentline;	/* (l) */
    int linedefined;	/* (S) */
    int lastlinedefined;	/* (S) */
    unsigned char nups;	/* (u) number of upvalues */
    unsigned char nparams;/* (u) number of parameters */
    char isvararg;        /* (u) */
    char istailcall;	/* (t) */
    unsigned short ftransfer;   /* (r) index of first value transferred */
    unsigned short ntransfer;   /* (r) number of transferred values */
    char short_src[NWLUA_IDSIZE]; /* (S) */
    /* private part */
    struct CallInfo* i_ci;  /* active function */
};
// --==</debug_api>==--

/******************************************************************************
* Copyright (C) 1994-2020 Lua.org, PUC-Rio.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

#endif  // NWLUACORE_H