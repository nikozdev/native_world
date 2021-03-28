#ifndef NWLUA_LIB_AUXIAURY_H
#define NWLUA_LIB_AUXIAURY_H

#include <stddef.h>
#include <stdio.h>

#include "nwlua_core.h"


/* global table */
#define NWLUA_GNAME	"_G"


typedef struct luaL_Buffer luaL_Buffer;


/* extra error code for 'luaL_loadfilex' */
#define NWLUA_ERRFILE     (NWLUA_ERRERR+1)


/* key, in the registry, for table of loaded modules */
#define NWLUA_LOADED_TABLE	"_LOADED"


/* key, in the registry, for table of preloaded loaders */
#define NWLUA_PRELOAD_TABLE	"_PRELOAD"


typedef struct luaL_Reg {
  const char *name;
  lua_CFunction func;
} luaL_Reg;


#define NWLUAL_NUMSIZES	(sizeof(lua_Integer)*16 + sizeof(lua_Number))

NWLUA_API void (luaL_checkversion_) (lua_State *L, lua_Number ver, size_t sz);
#define luaL_checkversion(L)  \
	  luaL_checkversion_(L, NWLUA_VERSION_NUM, NWLUAL_NUMSIZES)

NWLUA_API int (luaL_getmetafield) (lua_State *L, int obj, const char *e);
NWLUA_API int (luaL_callmeta) (lua_State *L, int obj, const char *e);
NWLUA_API const char *(luaL_tolstring) (lua_State *L, int idx, size_t *len);
NWLUA_API int (luaL_argerror) (lua_State *L, int arg, const char *extramsg);
NWLUA_API int (luaL_typeerror) (lua_State *L, int arg, const char *tname);
NWLUA_API const char *(luaL_checklstring) (lua_State *L, int arg,
                                                          size_t *l);
NWLUA_API const char *(luaL_optlstring) (lua_State *L, int arg,
                                          const char *def, size_t *l);
NWLUA_API lua_Number (luaL_checknumber) (lua_State *L, int arg);
NWLUA_API lua_Number (luaL_optnumber) (lua_State *L, int arg, lua_Number def);

NWLUA_API lua_Integer (luaL_checkinteger) (lua_State *L, int arg);
NWLUA_API lua_Integer (luaL_optinteger) (lua_State *L, int arg,
                                          lua_Integer def);

NWLUA_API void (luaL_checkstack) (lua_State *L, int sz, const char *msg);
NWLUA_API void (luaL_checktype) (lua_State *L, int arg, int t);
NWLUA_API void (luaL_checkany) (lua_State *L, int arg);

NWLUA_API int   (luaL_newmetatable) (lua_State *L, const char *tname);
NWLUA_API void  (luaL_setmetatable) (lua_State *L, const char *tname);
NWLUA_API void *(luaL_testudata) (lua_State *L, int ud, const char *tname);
NWLUA_API void *(luaL_checkudata) (lua_State *L, int ud, const char *tname);

NWLUA_API void (luaL_where) (lua_State *L, int lvl);
NWLUA_API int (luaL_error) (lua_State *L, const char *fmt, ...);

NWLUA_API int (luaL_checkoption) (lua_State *L, int arg, const char *def,
                                   const char *const lst[]);

NWLUA_API int (luaL_fileresult) (lua_State *L, int stat, const char *fname);
NWLUA_API int (luaL_execresult) (lua_State *L, int stat);


/* predefined references */
#define NWLUA_NOREF       (-2)
#define NWLUA_REFNIL      (-1)

NWLUA_API int (luaL_ref) (lua_State *L, int t);
NWLUA_API void (luaL_unref) (lua_State *L, int t, int ref);

NWLUA_API int (luaL_loadfilex) (lua_State *L, const char *filename,
                                               const char *mode);

#define luaL_loadfile(L,f)	luaL_loadfilex(L,f,NULL)

NWLUA_API int (luaL_loadbufferx) (lua_State *L, const char *buff, size_t sz,
                                   const char *name, const char *mode);
NWLUA_API int (luaL_loadstr) (lua_State *L, const char *s);

NWLUA_API lua_State *(luaL_newstate) (void);

NWLUA_API lua_Integer (luaL_len) (lua_State *L, int idx);

NWLUA_API void luaL_addgsub (luaL_Buffer *b, const char *s,
                                     const char *p, const char *r);
NWLUA_API const char *(luaL_gsub) (lua_State *L, const char *s,
                                    const char *p, const char *r);

NWLUA_API void (luaL_setfuncs) (lua_State *L, const luaL_Reg *l, int nup);

NWLUA_API int (luaL_getsubtable) (lua_State *L, int idx, const char *fname);

NWLUA_API void (luaL_traceback) (lua_State *L, lua_State *L1,
                                  const char *msg, int level);

NWLUA_API void (luaL_requiref) (lua_State *L, const char *modname,
                                 lua_CFunction openf, int glb);

/*
** ===============================================================
** some useful macros
** ===============================================================
*/


#define luaL_newlibtable(L,l)	\
  lua_createtable(L, 0, sizeof(l)/sizeof((l)[0]) - 1)

#define luaL_newlib(L,l)  \
  (luaL_checkversion(L), luaL_newlibtable(L,l), luaL_setfuncs(L,l,0))

#define luaL_argcheck(L, cond,arg,extramsg)	\
		((void)((cond) || luaL_argerror(L, (arg), (extramsg))))

#define luaL_argexpected(L,cond,arg,tname)	\
		((void)((cond) || luaL_typeerror(L, (arg), (tname))))

#define luaL_checkstring(L,n)	(luaL_checklstring(L, (n), NULL))
#define luaL_optstring(L,n,d)	(luaL_optlstring(L, (n), (d), NULL))

#define luaL_typename(L,i)	lua_typename(L, lua_type(L,(i)))

#define luaL_dofile(L, fn) \
	(luaL_loadfile(L, fn) || lua_pcall(L, 0, NWLUA_MULTRET, 0))

#define luaL_dostring(L, s) \
	(luaL_loadstr(L, s) || lua_pcall(L, 0, NWLUA_MULTRET, 0))

#define luaL_getmetatable(L,n)	(lua_getfield(L, NWLUA_REGISTRYINDEX, (n)))

#define luaL_opt(L,f,n,d)	(lua_isnoneornil(L,(n)) ? (d) : f(L,(n)))

#define luaL_loadbuffer(L,s,sz,n)	luaL_loadbufferx(L,s,sz,n,NULL)


/* push the value used to represent failure/error */
#define luaL_pushfail(L)	lua_pushnil(L)


/*
** {======================================================
** Generic Buffer manipulation
** =======================================================
*/

struct luaL_Buffer {
  char *b;  /* buffer address */
  size_t size;  /* buffer size */
  size_t n;  /* number of characters in buffer */
  lua_State *L;
  union {
    NWLUAI_MAXALIGN;  /* ensure maximum alignment for buffer */
    char b[NWLUAL_BUFFERSIZE];  /* initial buffer */
  } init;
};


#define luaL_bufflen(bf)	((bf)->n)
#define luaL_buffaddr(bf)	((bf)->b)


#define luaL_addchar(B,c) \
  ((void)((B)->n < (B)->size || luaL_prepbuffsize((B), 1)), \
   ((B)->b[(B)->n++] = (c)))

#define luaL_addsize(B,s)	((B)->n += (s))

#define luaL_buffsub(B,s)	((B)->n -= (s))

NWLUA_API void (luaL_buffinit) (lua_State *L, luaL_Buffer *B);
NWLUA_API char *(luaL_prepbuffsize) (luaL_Buffer *B, size_t sz);
NWLUA_API void (luaL_addlstring) (luaL_Buffer *B, const char *s, size_t l);
NWLUA_API void (luaL_addstr) (luaL_Buffer *B, const char *s);
NWLUA_API void (luaL_addvalue) (luaL_Buffer *B);
NWLUA_API void (luaL_pushresult) (luaL_Buffer *B);
NWLUA_API void (luaL_pushresultsize) (luaL_Buffer *B, size_t sz);
NWLUA_API char *(luaL_buffinitsize) (lua_State *L, luaL_Buffer *B, size_t sz);

#define luaL_prepbuffer(B)	luaL_prepbuffsize(B, NWLUAL_BUFFERSIZE)

/* }====================================================== */



/*
** {======================================================
** File handles for IO library
** =======================================================
*/

/*
** A file handle is a userdata with metatable 'NWLUA_FILEHANDLE' and
** initial structure 'luaL_Stream' (it may contain other fields
** after that initial structure).
*/

#define NWLUA_FILEHANDLE          "FILE*"


typedef struct luaL_Stream {
  FILE *f;  /* stream (NULL for incompletely created streams) */
  lua_CFunction closef;  /* to close stream (NULL for closed streams) */
} luaL_Stream;

/* }====================================================== */

/*
** {==================================================================
** "Abstraction Layer" for basic report of messages and errors
** ===================================================================
*/

/* print a string */
#if !defined(lua_writestring)
#define lua_writestring(s,l)   fwrite((s), sizeof(char), (l), stdout)
#endif

/* print a newline and flush the output */
#if !defined(lua_writeline)
#define lua_writeline()        (lua_writestring("\n", 1), fflush(stdout))
#endif

/* print an error message */
#if !defined(lua_writestringerror)
#define lua_writestringerror(s,p) \
        (fprintf(stderr, (s), (p)), fflush(stderr))
#endif

/* }================================================================== */


/*
** {============================================================
** Compatibility with deprecated conversions
** =============================================================
*/
#if defined(NWLUA_COMPAT_APIINTCASTS)

#define luaL_checkunsigned(L,a)	((lua_Unsigned)luaL_checkinteger(L,a))
#define luaL_optunsigned(L,a,d)	\
	((lua_Unsigned)luaL_optinteger(L,a,(lua_Integer)(d)))

#define luaL_checkint(L,n)	((int)luaL_checkinteger(L, (n)))
#define luaL_optint(L,n,d)	((int)luaL_optinteger(L, (n), (d)))

#define luaL_checklong(L,n)	((long)luaL_checkinteger(L, (n)))
#define luaL_optlong(L,n,d)	((long)luaL_optinteger(L, (n), (d)))

#endif
/* }============================================================ */



#endif  // NWLUA_LIB_AUXIAURY_H