#if false
//#ifndef LUA_WRAPS_H
#define LUA_WRAPS_H

#include <lua/nw_lua_core.h>

#include <sys/nw_data_sys.h>
#include <sys/nw_gui_sys.h>

#include <lua.hpp>
#include <lstate.h>
#include <lobject.h>

/// Lua Wrapper functions for native types and functions
/// -- Here are manipulators for the lua virtual machiene to make scripting for the engine
/// -- I want here to be some rules of syntax to make it better for reading:
/// ==| We can integrate "_system" types and "cpp_" functions for global manipulations
/// ==| Systems allows to create native type instances and this provides them own metatable
/// ==| Native type in lua it's just a pointer (ud/lud) with the bound metatable of system
/// ==| '.' accessor in lua is used for global objects, ':' for instances:
/// ent0:create_cmp(type) | entity_system.create_cmp(ent0, "type")
namespace NW
{
	// -- Support methods
	static inline AEntityCmp* GetCmpByArg(LuaState* pLState, Int32 nIdx) {
		Int32 nTypeArg = lua_type(pLState, nIdx);
		if (nTypeArg == LT_LUD) { return static_cast<AEntityCmp*>(lua_touserdata(pLState, nIdx)); }
		else if (nTypeArg == LT_NUM) { return nullptr; }
		else { return nullptr; }
	}
	AEntityCmp* CreateCmpOfEnt(AEntity* pEnt, const char* strCmpName) {
		if (pEnt == nullptr) return nullptr;
		if (strcmp(strCmpName, "cmp_transform_2d") == 0) {
			return pEnt->AddCmp<Transform2dCmp>();
		}
		else if (strcmp(strCmpName, "cmp_transform_3d") == 0) {
			return pEnt->AddCmp<Transform3dCmp>();
		}
		else if (strcmp(strCmpName, "cmp_graphics_2d") == 0) {
			return pEnt->AddCmp<Graphics2dCmp>();
		}
		else if (strcmp(strCmpName, "cmp_graphics_3d") == 0) {
			return pEnt->AddCmp<Graphics2dCmp>();
		}
		return nullptr;
	}
	AEntityCmp* GetCmpOfEnt(AEntity* pEnt, const char* strCmpName) {
		if (pEnt == nullptr) return nullptr;
		if (strcmp(strCmpName, "cmp_transform_2d") == 0 || strcmp(strCmpName, "cmp_transform_3d") == 0) {
			return pEnt->GetCmp<ATransformCmp>();
		}
		else if (strcmp(strCmpName, "cmp_graphics_2d") == 0 || strcmp(strCmpName, "cmp_graphics_3d") == 0) {
			return pEnt->GetCmp<AGraphicsCmp>();
		}
		return nullptr;
	}
	// -- Support methods
	static inline AEntity* GetEntByArg(LuaState* pLState, Int32 nIdx) {
		Int32 nTypeArg = lua_type(pLState, nIdx);
		if (nTypeArg == LT_STR) { return ADataRes::GetDataRes<AEntity>(lua_tostring(pLState, nIdx)); }
		else if (nTypeArg == LT_LUD) { return static_cast<AEntity*>(lua_touserdata(pLState, nIdx)); }
		else if (nTypeArg == LT_NUM) { return ADataRes::GetDataRes<AEntity>(lua_tonumber(pLState, nIdx)); }
		else { return nullptr; }
	}
	class NW_API LuaCmpSys
	{
	public:
		static LuaRegTable lrtCmpSys;
	public:
		// -- CmpMethods
		/// In Lua: cmp_sys.get_cmp_ent(cmp_ref/cmp_id)
		static inline Int32 GetCmpEnt(LuaState* pLState) {
			AEntityCmp* pCmp = GetCmpByArg(pLState, -1);

			if (pCmp != nullptr) {
				//lua_getglobal(pLState, "cmp_sys");
				//lua_setmetatable(pLState, -2);
				lua_pushlightuserdata(pLState, pCmp->GetEntity());
			}
			else { lua_pushnil(pLState); }
			return 1;
		}
		/// In Lua: cmp_sys.get_cmp_id(cmp_ref/cmp_id)
		static inline Int32 GetCmpId(LuaState* pLState) {
			AEntityCmp* pCmp = GetCmpByArg(pLState, -1);

			if (pCmp != nullptr) { lua_pushnumber(pLState, pCmp->GetId()); }
			else { lua_pushnil(pLState); }
			return 1;
		}
		/// In Lua: cmp_sys.get_cmp_name(cmp_ref/cmp_id)
		static inline Int32 GetCmpName(LuaState* pLState) {
			AEntityCmp* pCmp = GetCmpByArg(pLState, -1);

			if (pCmp != nullptr) { lua_pushstring(pLState, pCmp->GetName()); }
			else { lua_pushnil(pLState); }
			return 1;
		}
		// -- CmpSys methods
		/// In Lua: cmp_sys.get_cmp(ent_name/ent_ref/ent_id, cmp_name)
		static Int32 GetCmp(LuaState* pLState) {
			AEntity* pEnt = GetEntByArg(pLState, -2);
			if (lua_type(pLState, -1) != LT_STR) { lua_pop(pLState, 1); lua_pushnil(pLState); return 1; }
			AEntityCmp* pCmp = GetCmpOfEnt(pEnt, lua_tostring(pLState, -1));

			if (pCmp != nullptr) {
				lua_pushlightuserdata(pLState, pCmp);
				//lua_getglobal(pLState, "cmp_sys");
				//lua_setmetatable(pLState, -2);
			}
			else { lua_pushnil(pLState); }
			return 1;
		}
		/// In Lua: cmp_sys.create_cmp(ent_name/ent_ref/ent_id, cmp_name)
		static inline Int32 CreateCmp(LuaState* pLState) {
			AEntity* pEnt = GetEntByArg(pLState, -2);
			if (lua_type(pLState, -1) != LT_STR) { lua_pop(pLState, 1); lua_pushnil(pLState); return 1; }
			AEntityCmp* pCmp = CreateCmpOfEnt(pEnt, lua_tostring(pLState, -1));

			if (pCmp != nullptr) {
				lua_pushlightuserdata(pLState, pCmp);
				//lua_getglobal(pLState, "cmp_sys");
				//lua_setmetatable(pLState, -2);
			}
			else { lua_pushnil(pLState); }
			return 1;
		}
		/// In Lua: cmp_sys.destroy_cmp(ent/name/ent_ref/ent_id, cmp_name)
		/// or cmp_sys.destroy_cmp(cmp_ref/cmp_id)
		static inline Int32 DestroyCmp(LuaState* pLState) {
			AEntityCmp* pCmp = nullptr;
			Int32 nTypeArg = lua_type(pLState, -1);
			if (nTypeArg == LT_LUD) {	// Destroy by ref
				pCmp = static_cast<AEntityCmp*>(lua_touserdata(pLState, -1));
				pCmp->GetEntity()->RmvACmp(pCmp->GetTypeInfo());
				MemSys::DelT<AEntityCmp>(pCmp);
			}
			else {
				AEntity* pEnt = GetEntByArg(pLState, -2);
				if (pEnt == nullptr) { return 0; }
				else if (nTypeArg == LT_STR) {	// Destroy by name/id
					if (pCmp = GetCmpOfEnt(pEnt, lua_tostring(pLState, -1))) {
						pEnt->RmvACmp(pCmp->GetTypeInfo());
					}
				}
				else if (nTypeArg == LT_NUM) {
					if (pCmp = GetCmpByArg(pLState, -1)) {
						pEnt->RmvACmp(pCmp->GetTypeInfo());
						MemSys::DelT<AEntityCmp>(pCmp);
					}
				}
			}
			return 0;
		}
		// -- Metamethods
		static inline Int32 Index(LuaState* pLState) {
			AEntityCmp* pCmp = static_cast<AEntityCmp*>(lua_touserdata(pLState, -2));
			const char* strKey = lua_tostring(pLState, -1);

			if (strcmp(strKey, "cmp_id") == 0) {
				lua_pushnumber(pLState, pCmp->GetId());
				return 1;
			}
			else if (strcmp(strKey, "name") == 0) {
				lua_pushstring(pLState, pCmp->GetName());
				return 1;
			}
			else if (strcmp(strKey, "is_enabled") == 0) {
				lua_pushboolean(pLState, pCmp->IsEnabled());
				return 1;
			}
			else {
				lua_getglobal(pLState, &lrtCmpSys.strName[0]);
				lua_pushstring(pLState, strKey);
				lua_rawget(pLState, -2);
				lua_type(pLState, -3);
				lua_type(pLState, -2);
				lua_type(pLState, -1);
				return 1;
			}

			if (pCmp != nullptr) {	// If we need a component - it is inside the ent_sys table
				lua_getglobal(pLState, &lrtCmpSys.strName[0]);
				lua_pushstring(pLState, strKey);
				lua_rawget(pLState, -2);
				return 1;
			}
			lua_pushnil(pLState);
			return 1;
		}
		static inline Int32 NewIndex(LuaState* pLState) {
			AEntityCmp* pCmp = static_cast<AEntityCmp*>(lua_touserdata(pLState, -3));
			const char* strKey = lua_tostring(pLState, -2);

			if (lua_type(pLState, -1) == LT_BOL) {
				if (strcmp(strKey, "is_enabled") == 0) {
					pCmp->SetEnabled(lua_toboolean(pLState, -1));
					return 0;
				}
			}

			return 0;
		}
	};
	/// LuaWrap for EntitySystem
	/// -- Contains metamethods and default methods for entity
	/// -- The idea is that we use some global system to manipulate entities in the native code from lua
	/// -- System works with handlers of entities. It doesn't essentialy ocntains all these objects
	/// -- All we need to do is to provide by entity references lua side and take them for entity methods
	/// -- To extend entity object in lua we simply need to set ent_sys as metatable for EVERY ent which
	/// can be returned from native code as lightuserdata
	/// -- Otherwise I can keep lua ent simply as a handler and work with it same as LuaAPI with lua_State
	/// -- Sytems are translators for both languages. They allow to 
	struct NW_API LuaEntSys
	{
	public:
		static LuaRegTable lrtEntSys;
	public:
		// -- Ent Methods
		/// In Lua: "ent_sys.get_ent_id(ent_name/ent_ref/ent_id)"
		static inline Int32 GetEntId(LuaState* pLState) {
			Int32 nTypeEnt = lua_type(pLState, -1);
			AEntity* pEnt = nullptr;

			if ((pEnt = GetEntByArg(pLState, -1)) == nullptr) { lua_pop(pLState, 1); lua_pushnil(pLState); return 1; }
			lua_pushstring(pLState, &pEnt->GetName()[0]);
			return 1;
		}
		/// In Lua: "ent_sys.get_ent_name(ent_name/ent_ref/ent_id)"
		static inline Int32 GetEntName(LuaState* pLState) {
			AEntity* pEnt = nullptr;
			if ((pEnt = GetEntByArg(pLState, -1)) == nullptr) { lua_pop(pLState, 1); lua_pushnil(pLState); return 1; }

			lua_pushstring(pLState, pEnt->GetName());
			return 1;
		}
		/// In Lua: "ent_sys.get_over_ent(ent_name/ent_ref/ent_id)"
		static inline Int32 GetOverEnt(LuaState* pLState) {
			AEntity* pEnt = nullptr;
			if ((pEnt = GetEntByArg(pLState, -1)) == nullptr) { lua_pop(pLState, 1); lua_pushnil(pLState); return 1; }

			lua_pushlightuserdata(pLState, pEnt->GetOverEnt());
			return 1;
		}
		/// In Lua: "ent_sys.is_ent_enabled(ent_name/ent_ref/ent_id)"
		static inline Int32 IsEntEnabled(LuaState* pLState) {
			AEntity* pEnt = nullptr;
			if ((pEnt = GetEntByArg(pLState, -1)) == nullptr) { lua_pop(pLState, 1); lua_pushnil(pLState); return 1; }
			
			lua_pushboolean(pLState, pEnt->IsEnabled());
			return 1;
		}
		/// In Lua: "ent_sys.set_ent_name(ent_name/ent_ref/ent_id, "name")"
		static inline Int32 SetEntName(LuaState* pLState) {
			AEntity* pEnt = nullptr;
			if ((pEnt = GetEntByArg(pLState, -2)) == nullptr) { lua_pop(pLState, 2); lua_pushnil(pLState); return 1; }
			
			Int32 nTypeArg = lua_type(pLState, -1);
			if (nTypeArg != LT_STR) { lua_pop(pLState, 1); return 0; }
			
			pEnt->SetName(lua_tostring(pLState, -1));
			return 0;
		}
		/// In Lua: "ent_sys.set_ent_enabled(ent_name/ent_ref/ent_id, true/false)"
		static inline Int32 SetEntEnabled(LuaState* pLState) {
			AEntity* pEnt = nullptr;
			if ((pEnt = GetEntByArg(pLState, -2)) == nullptr) { lua_pop(pLState, 2); lua_pushnil(pLState); return 1; }
		
			Int32 nTypeArg = lua_type(pLState, -1);
			if (nTypeArg != LT_BOL) { lua_pop(pLState, 1); return 0; }
			
			pEnt->SetEnabled(lua_toboolean(pLState, -1));
			return 0;
		}
		/// In Lua: "ent_sys.set_over_ent(ent_name/ent_ref/ent_id, over_ent_str/over_ent_ref/over_ent_id)"
		static inline Int32 SetOverEnt(LuaState* pLState) {
			AEntity* pEnt = nullptr;
			if ((pEnt = GetEntByArg(pLState, -2)) == nullptr) { lua_pop(pLState, 2); lua_pushnil(pLState); return 1; }
			AEntity* pOverEnt = nullptr;
			if ((pEnt = GetEntByArg(pLState, -1)) == nullptr) { lua_pop(pLState, 1); lua_pushnil(pLState); return 1; }

			pEnt->SetOverEnt(pOverEnt);
			return 0;
		}
		// -- EntSys Methods
		/// In Lua: "ent_sys.get_ent(ent_name/ent_id)"
		static inline Int32 GetEnt(LuaState* pLState) {
			AEntity* pEnt = nullptr;
			if ((pEnt = GetEntByArg(pLState, -2)) == nullptr) { lua_pop(pLState, 1); lua_pushnil(pLState); return 1; }

			// if entity pointer doesn't still have metatable - it has to be set
			lua_pushlightuserdata(pLState, pEnt);
			//lua_getglobal(pLState, &lrtEntSys.strName[0]);
			//lua_setmetatable(pLState, -2);
			// but if lightuserdata of ent has already been "metatabled" - there is no changes
			return 1;
		}
		/// In Lua: "ent_sys.create_ent(void/ent_name)"
		static inline Int32 CreateEnt(LuaState* pLState) {
			Int32 nType = lua_type(pLState, -1);
			AEntity* pEnt = MemSys::NewT<AEntity>();
			
			if (nType == LT_STR) { pEnt->SetName(lua_tostring(pLState, -1)); }
			else { lua_pop(pLState, -1); }
			
			// to maintain object syntax (ent_n:func() instead of only ent_sys.func(ent_n))
			lua_pushlightuserdata(pLState, pEnt);
			//lua_getglobal(pLState, &lrtEntSys.strName[0]);
			//lua_setmetatable(pLState, -2);
			return 1;
		}
		/// In Lua: "ent_sys.destroy_ent(ent_name/ent_ref/ent_id)"
		static inline Int32 DestroyEnt(LuaState* pLState) {
			Int32 nType = lua_type(pLState, -1);
			if (nType == LT_STR) {			// destroy by name
				//Scene::Get().DestroyEntity(lua_tostring(pLState, -1));
			}
			else if (nType == LT_LUD) {		// destroy by reference
				//Scene::Get().DestroyEntity(static_cast<AEntity*>(lua_touserdata(pLState, -1)));
			}
			else if (nType == LT_NUM) {		// destroy by id
				//Scene::Get().DestroyEntity(lua_tonumber(pLState, -1));
			}
			return 0;
		}

		// -- MetaMethods
		static inline Int32 Index(LuaState* pLState) {
			AEntity* pEnt = static_cast<AEntity*>(lua_touserdata(pLState, -2));
			const char* strKey = lua_tostring(pLState, -1);
			AEntityCmp* pCmp = nullptr;

			if (strcmp(strKey, "id") == 0) {
				lua_pushnumber(pLState, pEnt->GetId());
				return 1;
			}
			else if (strcmp(strKey, "name") == 0) {
				lua_pushstring(pLState, pEnt->GetName());
				return 1;
			}
			else if (strcmp(strKey, "is_enabled") == 0) {
				lua_pushboolean(pLState, pEnt->IsEnabled());
				return 1;
			}
			else {
				lua_getglobal(pLState, &lrtEntSys.strName[0]);
				lua_pushstring(pLState, strKey);
				lua_rawget(pLState, -2);
				return 1;
			}

			if (pCmp != nullptr) {	// If we need a component - it is inside the ent_sys table
				lua_getglobal(pLState, &lrtEntSys.strName[0]);
				lua_pushstring(pLState, strKey);
				lua_rawget(pLState, -2);
				return 1;
			}
			lua_pushnil(pLState);
			return 1;
		}
		static inline Int32 NewIndex(LuaState* pLState) {
			AEntity* pEnt = static_cast<AEntity*>(lua_touserdata(pLState, -3));
			const char* strKey = lua_tostring(pLState, -2);

			if (lua_type(pLState, -1) == LT_BOL) {
				if (strcmp(strKey, "is_enabled") == 0) {
					pEnt->SetEnabled(lua_toboolean(pLState, -1));
					return 0;
				}
			}

			return 0;
		}
	};
}

#endif	// LUA_WRAPS_H