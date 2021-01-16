#ifndef LUA_DECL_HPP
#define LUA_DECL_HPP

#include <nw_core.hpp>

struct lua_State;

#define LUA_TNONE		    (-1)
#define LUA_TNIL		    0
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER		    3
#define LUA_TSTRING		    4
#define LUA_TTABLE		    5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD		    8
#define LUA_NUMTYPES		9
#define LUA_TUPVAL			LUA_NUMTYPES

namespace NW
{
	class NW_API LuaEngine;
	
	struct NW_API LuaEngineInfo;
	struct NW_API LuaRegFunc;
	struct NW_API LuaRegTable;

	using LuaState = lua_State;
	using LuaNativeFunc = int(*)(lua_State*);
	using LuaUpValues = DStack<void*>;

	enum LuaTypes : Int16 {
		LT_NON			= LUA_TNONE,
		LT_NIL			= LUA_TNIL,
		LT_BOL			= LUA_TBOOLEAN,
		LT_LUD			= LUA_TLIGHTUSERDATA,
		LT_NUM			= LUA_TNUMBER,
		LT_STR			= LUA_TSTRING,
		LT_TAB			= LUA_TTABLE,
		LT_FNC			= LUA_TFUNCTION,
		LT_USD			= LUA_TUSERDATA,
		LT_THR			= LUA_TTHREAD,
		LT_UPV			= LUA_TUPVAL,
	};
}
#endif	// LUA_DECL_HPP