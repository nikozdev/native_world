#ifndef NW_LUA_ENGINE_H
#define NW_LUA_ENGINE_H

#include <lua_core.hpp>
#include <lua/lua_tools.h>

namespace NW
{
	/// LuaVirtualMachiene singleton class
	/// -- Handles lua state
	/// -- Performs current script of the engine state
	class NW_API lua_engine
	{
	public:
		lua_engine();
		lua_engine(lua_engine& copy) = delete;
		~lua_engine();
		// --getters
		inline lua_state* get_lua_state() { return m_lstate; }
		inline const lua_engine_info& get_info() const { return m_info; }
		template <typename vtype> vtype get_lua_value_glb(cstr name);
		template <typename vtype> vtype get_lua_value_loc(cstr name);
		template <typename vtype> vtype get_lua_value(cstr loc, cstr name);
		// --setters
		template <typename vtype> bool set_lua_value_glb(const vtype& value, cstr name);
		template <typename vtype> bool set_lua_value_loc(const vtype& value, cstr name);
		template <typename vtype> bool set_lua_value(const vtype& tValue, cstr loc, cstr name);
		// --predicates
		inline bit is_running() const { return m_is_running; }
		// --operators
		void operator=(lua_engine& copy) = delete;
		// --core_methods
		lua_state* new_lua_state();
		void del_lua_state(lua_state* lstate);
		bool load_script(const dstr& script);
		bool run_script_str(cstr script);
		bool run_script_file(cstr file_path);
		bool call_lua_func_glb(cstr name);
		bool call_lua_func_loc(cstr name);
		bool call_lua_func(cstr loc, cstr name);
		bool lua_reg(lua_func_info& reg_info);
		bool lua_reg(lua_table_info& reg_info);
	private:
		bit m_is_running;
		mem_arena m_MemAllocator;

		lua_state* m_lstate;
		lua_engine_info m_info;
	private:	// Implementation Methods
		inline bool lua_move_glb(cstr name);
		inline bool lua_move_loc(cstr name);
		inline bool lua_move(const char* strLoc, const char* name);

		inline bool lua_move_tglb(cstr name, lua_types ltype);
		inline bool lua_move_tloc(cstr name, lua_types ltype);
		inline bool lua_move_t(cstr loc, cstr name, lua_types ltype);
		
		inline void lua_clear();
		inline void lua_clear(v1si count);
		inline bool lua_push(lua_func_info& reg_info);
		inline bool lua_push(lua_table_info& r_reg_tab);
		inline void write_err(const char* strFormat, ...);
		inline void write_info();

		static v1si lua_write(lua_state* lstate, ptr block_ptr, size block_size, ptr writer);
		static cstr lua_read(lua_state* lstate, ptr reader, size* block_size);
		static ptr lua_alloc(ptr allocator, ptr block_ptr, size new_size, size old_size);
		static ptr lua_warn(ptr warner, cstr msg, v1si code);
	};
}

#endif	// NW_LUA_ENGINE_H