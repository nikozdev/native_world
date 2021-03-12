#include <nw_pch.hpp>
#include "lua_engine.h"

#include <lua.hpp>
#include <lstate.h>
#include <lobject.h>
namespace NW
{
	lua_engine::lua_engine() :
		m_MemAllocator(mem_arena(nullptr, 0)),
		m_lstate(nullptr) { m_lstate = new_lua_state(); }
	lua_engine::~lua_engine() { del_lua_state(m_lstate); }
	// --getters
	template <> si32 lua_engine::get_lua_value_glb<si32>(cstring name) {
		si32 result = 0;
		if (lua_move_tglb(name, LT_NUM)) {
			result = static_cast<si32>(lua_tonumberx(m_lstate, -1, 0));
		}
		else { result = 0; }
		lua_clear(1);
		return result;
	}
	template <> si32 lua_engine::get_lua_value_loc<si32>(cstring name) {
		si32 result = 0;
		if (lua_move_tloc(name, LT_NUM)) {
			result = static_cast<si32>(lua_tonumberx(m_lstate, -1, 0));
		}
		else { result = 0; }
		lua_clear(1);
		return result;
	}
	template <> si32 lua_engine::get_lua_value<si32>(cstring strLoc, cstring name) {
		si32 result = 0;
		if (lua_move_t(strLoc, name, LT_NUM)) {
			result = static_cast<si32>(lua_tonumberx(m_lstate, -1, 0));
		}
		else { result = 0; }
		lua_clear();
		return result;
	}
	// --setters
	template <> bool lua_engine::set_lua_value_glb<si32>(const si32& tValue, cstring name) {
		lua_pushnumber(m_lstate, tValue);
		m_info.add_type(lua_type(m_lstate, -1));
		if (lua_move_glb(name)) { lua_clear(2); return false; }
		lua_setglobal(m_lstate, name);
		m_info.rmv_type(1);
		return true;
	}
	template <> bool lua_engine::set_lua_value_loc<si32>(const si32& tValue, cstring name) {
		if (m_info.get_stack_type(m_info.get_level() - 1) == LT_TAB) {
			lua_pushnumber(m_lstate, tValue);
			m_info.add_type(lua_type(m_lstate, -1));
			if (lua_move_loc(name)) { lua_clear(2); return false; }
			lua_setfield(m_lstate, -2, name);
			m_info.rmv_type(1);
			return true;
		}
		else { return false; }
	}
	template <> bool lua_engine::set_lua_value<si32>(const si32& tValue, cstring strLoc, cstring name) {
		if (lua_move_t(strLoc, name, LT_NUM)) { lua_clear(); return false; }
		else {
			m_info.add_type(lua_type(m_lstate, -1));
			lua_pushnumber(m_lstate, tValue);
			lua_setfield(m_lstate, -2, name);
			m_info.rmv_type(1);
		}
		lua_clear();
		return true;
	}

	// --==<core_methods>==--
	//lua_table_info Luacmp_sys::lrtcmp_sys = lua_table_info("cmp_sys");
	//lua_table_info Luaent_sys::lrtent_sys = lua_table_info("ent_sys");
	lua_state* lua_engine::new_lua_state()
	{
		lua_state* lstate = lua_newstate(lua_engine::lua_alloc , &m_MemAllocator);
		//lua_state* pLState = luaL_newstate();
		//lua_setallocf(pLState, lua_engine::LuaAllocate, &m_MemAllocator);

		luaL_openlibs(lstate);
		m_lstate = lstate;
#if false
		{
			// Cmp methods
			lua_func_info lrfGetCmpId = lua_func_info("get_cmp_id", Luacmp_sys::GetCmpId);
			Luacmp_sys::lrtcmp_sys.lrfMethods.push_back(&lrfGetCmpId);
			lua_func_info lrfGetCmpName = lua_func_info("get_cmp_name", Luacmp_sys::GetCmpName);
			Luacmp_sys::lrtcmp_sys.lrfMethods.push_back(&lrfGetCmpName);
			lua_func_info lrfGetCmpEnt = lua_func_info("get_cmp_ent", Luacmp_sys::GetCmpEnt);
			Luacmp_sys::lrtcmp_sys.lrfMethods.push_back(&lrfGetCmpEnt);
			// cmp_sys methods
			lua_func_info lrfGetCmp = lua_func_info("get_cmp", Luacmp_sys::GetCmp);
			Luacmp_sys::lrtcmp_sys.lrfMethods.push_back(&lrfGetCmp);
			lua_func_info lrfCreateCmp = lua_func_info("create_cmp", Luacmp_sys::CreateCmp);
			Luacmp_sys::lrtcmp_sys.lrfMethods.push_back(&lrfCreateCmp);
			lua_func_info lrfDestroyCmp = lua_func_info("destroy_cmp", Luacmp_sys::DestroyCmp);
			Luacmp_sys::lrtcmp_sys.lrfMethods.push_back(&lrfDestroyCmp);
			// Metamethods
			lua_func_info lrfIndex = lua_func_info("__index", Luacmp_sys::Index);
			Luacmp_sys::lrtcmp_sys.lrfMethods.push_back(&lrfIndex);
			lua_func_info lrfNewIndex = lua_func_info("__newindex", Luacmp_sys::NewIndex);
			Luacmp_sys::lrtcmp_sys.lrfMethods.push_back(&lrfNewIndex);

			LuaReg(Luacmp_sys::lrtcmp_sys);
		}
		{
			// Ent methods
			lua_func_info lrfGetEntId = lua_func_info("get_ent_id", Luaent_sys::GetEntId);
			Luaent_sys::lrtent_sys.lrfMethods.push_back(&lrfGetEntId);
			lua_func_info lrfGetEntName = lua_func_info("get_ent_name", Luaent_sys::GetEntName);
			Luaent_sys::lrtent_sys.lrfMethods.push_back(&lrfGetEntName);
			lua_func_info lrfGetOverEnt = lua_func_info("get_over_ent", Luaent_sys::GetOverEnt);
			Luaent_sys::lrtent_sys.lrfMethods.push_back(&lrfGetOverEnt);

			lua_func_info lrfSetName = lua_func_info("set_ent_name", Luaent_sys::SetEntName);
			Luaent_sys::lrtent_sys.lrfMethods.push_back(&lrfSetName);
			lua_func_info lrfSetEntEnabled = lua_func_info("set_ent_enabled", Luaent_sys::SetEntEnabled);
			Luaent_sys::lrtent_sys.lrfMethods.push_back(&lrfSetEntEnabled);
			lua_func_info lrfSetOverEnt = lua_func_info("set_over_ent", Luaent_sys::SetOverEnt);
			Luaent_sys::lrtent_sys.lrfMethods.push_back(&lrfSetOverEnt);
			// ent_sys methods
			lua_func_info lrfGetEnt = lua_func_info("get_ent", Luaent_sys::GetEnt);
			Luaent_sys::lrtent_sys.lrfMethods.push_back(&lrfGetEnt);
			lua_func_info lrfCreateEnt = lua_func_info("create_ent", Luaent_sys::CreateEnt);
			Luaent_sys::lrtent_sys.lrfMethods.push_back(&lrfCreateEnt);
			lua_func_info lrfDestroyEnt = lua_func_info("destroy_ent", Luaent_sys::DestroyEnt);
			Luaent_sys::lrtent_sys.lrfMethods.push_back(&lrfDestroyEnt);
			// Metamethods
			lua_func_info lrfIndex = lua_func_info("__index", Luaent_sys::Index);
			Luaent_sys::lrtent_sys.lrfMethods.push_back(&lrfIndex);
			lua_func_info lrfNewIndex = lua_func_info("__newindex", Luaent_sys::NewIndex);
			Luaent_sys::lrtent_sys.lrfMethods.push_back(&lrfNewIndex);

			LuaReg(Luaent_sys::lrtent_sys);
		}
#endif

		return lstate;
	}
	void lua_engine::del_lua_state(lua_state* lstate) { lua_close(lstate); }

	bool lua_engine::load_script(const dstring& script)
	{
		lua_reader_state lstate_reader;
		lstate_reader.chunk = &script;
		si32 resultult = 0;
		//lua_load(m_lstate, lua_engine::lua_read, &lstate_reader, "lua_chunk", "r");
		
		return resultult == LUA_OK;
	}

	bool lua_engine::run_script_str(cstring script)
	{
		bool result = true;
		luaL_loadstring(m_lstate, &script[0]);
		m_info.add_type(lua_type(m_lstate, -1));
		if (lua_pcallk(m_lstate, 0, 0, 0, 0, 0) != LUA_OK) {
			write_err("could not run the script");
			result = false;
		}
		else { result = true; }
		lua_clear(1);

		return result;
	}
	bool lua_engine::run_script_file(cstring file_path)
	{
		bool result = true;
		if (luaL_dofile(m_lstate, &file_path[0]) != LUA_OK) {
			write_err("could not run the script");
			result = false;
		}
		else { result = true; }
		return result;
	}
	
	bool lua_engine::call_lua_func_glb(cstring name)
	{
		bool result = true;
		result = lua_move_tglb(name, LT_FNC);
		if (result) { result = (lua_pcallk(m_lstate, 0, 0, 0, 0, 0) == LUA_OK); lua_clear(1); }
		else { write_err("function \"{str}\" was not found;", name); }
		return result;
	}
	bool lua_engine::call_lua_func_loc(cstring name)
	{
		bool bResult = true;
		bResult = lua_move_tloc(name, LT_FNC);
		if (bResult) { bResult = (lua_pcallk(m_lstate, 0, 0, 0, 0, 0) == LUA_OK); lua_clear(1); }
		else { write_err("function \"{str}\" was not found;", name); }
		return bResult;
	}
	bool lua_engine::call_lua_func(cstring strLoc, cstring name)
	{
		bool bResult = true;
		bResult = lua_move_t(strLoc, name, LT_FNC);
		if (bResult) { bResult = (lua_pcallk(m_lstate, 0, 0, 0, 0, 0) == LUA_OK); lua_clear(); }
		else { write_err("function \"{str}\" was not found;", name); }
		return bResult;
	}

	bool lua_engine::lua_reg(lua_func_info& reg_info)
	{
		bool bIsGlb = strcmp(reg_info.loc, "") == 0;
		if (bIsGlb) {	// The function is global
			if (lua_move_glb(&reg_info.name[0])) { // this name is already defined globally
				lua_clear(1);
				return false;
			}
		}
		else if (lua_move(reg_info.loc, reg_info.name)) { // this name is already defined locally
			lua_clear();
			return false;
		}
		if (!lua_push(reg_info)) { // the function is on the top
			lua_clear();
			return false;
		}
		if (bIsGlb) { lua_setglobal(m_lstate, reg_info.name); m_info.rmv_type(1); }
		else { lua_setfield(m_lstate, -2, reg_info.name); m_info.rmv_type(1); lua_clear(); }
		
		lua_clear();
		return true;
	}
	bool lua_engine::lua_reg(lua_table_info& r_reg_tab)
	{	// registration of lua table is same as a registration of lua function
		bool bIsGlb = (strcmp(&r_reg_tab.loc[0], "") == 0);
		if (bIsGlb) {
			if (lua_move_glb(r_reg_tab.name)) {
				lua_clear(1);
				return false;
			}
		}
		else if (lua_move(r_reg_tab.loc, r_reg_tab.name)) {
			lua_clear();
			return false;
		}
		if (!lua_push(r_reg_tab)) {
			lua_clear();
			return false;
		}
		if (bIsGlb) { lua_setglobal(m_lstate, r_reg_tab.name); m_info.rmv_type(1); }
		else { lua_setfield(m_lstate, -2, r_reg_tab.name); m_info.rmv_type(1); lua_clear(); }

		return true;
	}
	// --==</core_methods>==--

	// --==<implementation_methods>==--
	inline bool lua_engine::lua_move_glb(cstring name)
	{
		lua_getglobal(m_lstate, &name[0]);
		m_info.add_type(lua_type(m_lstate, -1));
		if (m_info.get_last_type() == LT_NIL) { // value is not found
			lua_clear(1);
			return false;
		}
		return true;
	}
	inline bool lua_engine::lua_move_loc(cstring name)
	{
		if (m_info.get_last_type() != LT_TAB) { return false; }
		lua_getfield(m_lstate, -1, &name[0]);
		m_info.add_type(lua_type(m_lstate, -1));
		if (m_info.get_last_type() == LT_NIL) {		// Value is not found
			lua_clear(1);
			return false;
		}
		return true;
	}
	inline bool lua_engine::lua_move(cstring strLoc, cstring name)
	{	// get on the top of the stack some value due to string "tab1.tab2.value"
		dstring str_buf("", 256);		// String buffer for the currently serched name
		io_stream_str stm = io_stream_str(dstring(strLoc) + '.' + name);

		// start with the global namespace
		stm.getline(&str_buf[0], str_buf.size(), '.');
		if (!lua_move_glb(&str_buf[0])) { return false; }

		while (!stm.eof()) {	// Works only if with the global name is char '.'
			stm.getline(&str_buf[0], str_buf.size(), '.');
			if (!lua_move_loc(&str_buf[0])) { return false; }
		}
		//// We have found a value by the full path. It can be anything
		return true;
	}

	inline bool lua_engine::lua_move_tglb(cstring name, lua_types nLuaType) {
		if (lua_move_glb(name)) { return m_info.get_last_type() == nLuaType; }
		else { return false; }
	}
	inline bool lua_engine::lua_move_tloc(cstring name, lua_types nLuaType) {
		if (lua_move_loc(name)) { return m_info.get_last_type() == nLuaType; }
		else { return false; }
	}
	inline bool lua_engine::lua_move_t(cstring strLoc, cstring name, lua_types nLuaType) {
		if (lua_move(strLoc, name)) { return m_info.get_last_type() == nLuaType; }
		else { return false; }
	}

	inline void lua_engine::lua_clear() {
		ui8 unLevel = lua_gettop(m_lstate);
		lua_clear(unLevel);
	}
	inline void lua_engine::lua_clear(ui8 count)
	{
		m_info.rmv_type(count);
		if (count > lua_gettop(m_lstate)) { return; }
		lua_pop(m_lstate, count);
	}

	inline bool lua_engine::lua_push(lua_func_info& reg_info)
	{
		if (reg_info.native_func == nullptr) { return false; }
		if (strcmp(reg_info.name, "") == 0) { return false; }

		lua_up_values up_vals_cpy = reg_info.up_vals;
		for (ui16 uvi = 0; uvi < up_vals_cpy.size(); uvi++) { // throw all the upvalues on the top of the stack
			if (up_vals_cpy.top() == nullptr) {
				lua_clear();
				write_err("native function has null pointers as upvalues");
				return false;
			}
			lua_pushlightuserdata(m_lstate, up_vals_cpy.top());
			up_vals_cpy.pop();
		}
		// here all upvalues are poped out of the stack
		lua_pushcclosure(m_lstate, reg_info.native_func, reg_info.up_vals.size());
		m_info.add_type(lua_type(m_lstate, -1));
		return true;
	}
	inline bool lua_engine::lua_push(lua_table_info& r_reg_tab)
	{
		if (strcmp(r_reg_tab.name, "") == 0) { return false; }
		
		lua_createtable(m_lstate, 0, 0);
		m_info.add_type(lua_type(m_lstate, -1));
		for (auto pRegMeth : r_reg_tab.methods) {
			if (!lua_push(*pRegMeth)) { lua_clear(); return false; }
			else { lua_setfield(m_lstate, -2, pRegMeth->name); m_info.rmv_type(1); }
		}
		if (r_reg_tab.meta_table != nullptr) {
			if (!lua_push(*r_reg_tab.meta_table)) { lua_clear(); return false; }
			lua_pushstring(m_lstate, r_reg_tab.meta_table->name);
			lua_setfield(m_lstate, -2, "__name");  // metatable.__name = tname
			lua_pushvalue(m_lstate, -1);
			lua_setfield(m_lstate, LUA_REGISTRYINDEX, r_reg_tab.meta_table->name);  // registry.name = metatable
			lua_setmetatable(m_lstate, -2); m_info.rmv_type(1);
		}
		return true;
	}

	inline void lua_engine::write_info()
	{
		ui8 unLevel = lua_gettop(m_lstate);
		m_info.rmv_type(unLevel);
		for (ui16 ti = 0; ti < unLevel; ti++) { m_info.add_type(lua_type(m_lstate, -ti)); }
		
		log_sys::write_info(
			"Lua_stack_info\n"
			"status: {int};\nvalues: {int}\nstack_size: {int};",
			m_lstate->status, m_lstate->stack->val, m_lstate->stacksize
		);
		
		std::cout << m_info;
	}
	inline void lua_engine::write_err(cstring strFormat, ...)
	{
		va_list valArgs;
		va_start(valArgs, strFormat);
		dstring err_commentLog = str_format_var(strFormat, valArgs);
		va_end(valArgs);
		if (lua_isstring(m_lstate, -1)) { err_commentLog += lua_tolstring(m_lstate, -1, 0); }
		lua_clear();
		NW_ERR(&err_commentLog[0]);
	}
	// --==</implementation_methods>==--

	// --==<lua_delegates>==--
	int lua_engine::lua_write(lua_state* lstate, ptr block_ptr, size block_size, ptr writter)
	{
		return 0;
	}
	cstring lua_engine::lua_read(lua_state* lstate, ptr reader, size* block_size)
	{
		lua_reader_state* lstate_reader = static_cast<lua_reader_state*>(reader);
		if (lstate_reader->is_read) {
			return nullptr;
		}
		else {
			lstate_reader->is_read = true;
			*block_size = lstate_reader->chunk->length();
			return &(*lstate_reader->chunk)[0];
		}
		return nullptr;
	}
	void* lua_engine::lua_alloc(ptr allocator, ptr block_ptr, size old_size, size new_size)
	{
		a_mem_allocator* mem_allocator = static_cast<a_mem_allocator*>(allocator);
		if (block_ptr != nullptr) {
			if (new_size == 0) { mem_allocator->dealloc(block_ptr, old_size); }
			else if (old_size > 0) { block_ptr = mem_allocator->realloc(block_ptr, old_size, new_size); }
		}
		else if (new_size > 0) { block_ptr = mem_allocator->alloc(new_size); }
		return block_ptr;
	}
	// --==</lua_delegates>==--
}

/// What are the functions I am going to add into the lua scripts?
//inline const lua_func_info* lua_engine::GetRegFunction(cstring name) {
//	RegFunctions::iterator itRegFunc = m_RegFunctions.find(name);
//	return itRegFunc == m_RegFunctions.end() ? nullptr : &itRegFunc->second;
//}
//inline const lua_table_info* lua_engine::GetRegTable(cstring name) {
//	RegTables::iterator itRegTab = m_RegTables.find(name);
//	return itRegTab == m_RegTables.end() ? nullptr : &itRegTab->second;
//}
// --setters
//void lua_engine::AddRegFunction(lua_func_info& reg_info) {
//	if (HasRegFunction(&reg_info.name[0]) || HasFunction(&reg_info.name[0])) { return; }
//	String strAbsName = "";
//	ui16 unLevel = 0;
//
//	if (reg_info.fnNative != nullptr) { PushRegFunction(reg_info.fnNative, reg_info.UpValues); }
//	else { return; }
//
//	if (strcmp(reg_info.strLoc, "") == 0) {
//		strAbsName = strAbsName + reg_info.name;
//		lua_setglobal(m_lstate, &strAbsName[0]);
//	}
//	else {	// Find the location table
//		strAbsName = strAbsName + reg_info.strLoc + '.' + reg_info.name;
//		unLevel = lua_move(&reg_info.strLoc[0]);
//		si32 ltype = lua_type(m_lstate, -1);
//		if (ltype == LUA_TTABLE) {
//			 Get the function on the top again
//			lua_pushvalue(m_lstate, -2); unLevel++;
//			 Stack now has ...[-3]function[-2]table[-1]function
//			 Set that function as a part of the table - function goes out
//			lua_setfield(m_lstate, -2, &reg_info.name[0]); unLevel--;
//			lua_clear(unLevel);
//		}
//		else { return; }
//	}
//	m_RegFunctions.insert({ &strAbsName[0], reg_info });
//}
//void lua_engine::RemoveRegFunction(cstring name) {
//	RegFunctions::iterator itRegFunc = m_RegFunctions.find(name);
//	if (itRegFunc == m_RegFunctions.end()) { return; }
//	m_RegFunctions.erase(itRegFunc);
//}
//void lua_engine::RemoveRegTable(cstring name) {
//	RegTables::iterator itRegTab = m_RegTables.find(name);
//	if (itRegTab == m_RegTables.end()) { return; }
//	m_RegTables.erase(itRegTab);
//}
// -- Predicates
//bool lua_engine::HasTable(cstring name) {
//	ui16 unLevel = lua_move(name);
//	bool bHasTab = lua_type(m_lstate, -1) == LUA_TTABLE;
//	lua_clear(unLevel);
//	return bHasTab;
//}
//bool lua_engine::HasRegTable(cstring name) { return m_RegTables.find(name) != m_RegTables.end(); }
//bool lua_engine::HasFunction(cstring name) {
//	ui16 unLevel = lua_move(name);
//	bool bHasFunc = lua_type(m_lstate, -1) == LUA_TFUNCTION;
//	lua_clear(unLevel);
//	return bHasFunc;
//}
//bool lua_engine::HasRegFunction(cstring name) { return m_RegFunctions.find(name) != m_RegFunctions.end(); }
/// This is registration of native types in Lua tables
/// In lua virtual machiene will be created a table with default types and it will be like a class
/// But to use default "__meta" methods it is essential to set metatable from the "fnCreate" function
/// It is enough to "lua_getmetatable(LuaNative, "{RegTable_name}_meta")" and "setmetatable(LuaNative)"
//void lua_engine::AddRegTable(lua_table_info& r_reg_tab) {
//	String strAbsName = "";
//	ui16 unLevel = 0;
//	if (HasRegTable(&strAbsName[0]) || HasTable(&strAbsName[0])) { return; }
//	if (strcmp(r_reg_tab.strLoc, "") == 0) {	// Location is global, set the table globally
//		strAbsName = strAbsName + r_reg_tab.name;
//		lua_createtable(m_lstate, 0, 0);
//		unLevel++;
//		lua_pushvalue(m_lstate, -1);	// Keep on the stack the second pointer
//		lua_setglobal(m_lstate, &strAbsName[0]);	// Set the table and pop it from the stack
//	}
//	else {	// Find the overtable (location)
//		ui16 unLevel = lua_move(&r_reg_tab.strLoc[0]);
//		if (lua_type(m_lstate, -1) == LUA_TTABLE) {		// The over table is on the stack
//			lua_table_info* pOverTab = &r_reg_tab;
//			// Set the table as a field of the over table
//			lua_createtable(m_lstate, 0, 0);
//			unLevel++;
//			lua_pushvalue(m_lstate, -1);
//			lua_setfield(m_lstate, -2, &r_reg_tab.name[0]);
//		}
//		else {
//			lua_pop(m_lstate, 1);
//			WriteErrLog("Over table of \"{str}\" table was not found!", &r_reg_tab.name[0]);
//			return;
//		}
//	}
//	if (r_reg_tab.fnCreate != nullptr) {		// Set the constructor method "Create"
//		PushRegFunction(*r_reg_tab.fnCreate, r_reg_tab.UpValues);
//		lua_setfield(m_lstate, -2, "create");
//	}
//	// New metatable with special methods specially for this type
//	luaL_newmetatable(m_lstate, &(String(r_reg_tab.name) + "_meta")[0]);
//	unLevel++;
//	// "__gc" - called whenever the object is garbage collected;
//	if (r_reg_tab.fnDestroy != nullptr) {
//		PushRegFunction(*r_reg_tab.fnDestroy, r_reg_tab.UpValues);
//		lua_setfield(m_lstate, -2, "__gc");
//	}
//	// "__index" - called whenever we get some value from the table;
//	if (r_reg_tab.fnIndex != nullptr) {
//		PushRegFunction(*r_reg_tab.fnNewIndex, r_reg_tab.UpValues);
//		lua_setfield(m_lstate, -2, "__index");
//	}
//	// "__newindex" - called whenever we set some value into the table
//	if (r_reg_tab.fnNewIndex != nullptr) {
//		PushRegFunction(*r_reg_tab.fnNewIndex, r_reg_tab.UpValues);
//		lua_setfield(m_lstate, -2, "__newindex");
//	}
//	lua_setmetatable(m_lstate, -2);
//
//	lua_clear(unLevel);
//	m_RegTables.insert({ &strAbsName[0], r_reg_tab });
//}