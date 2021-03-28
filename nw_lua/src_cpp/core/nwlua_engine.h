#ifndef NWLUA_ENGINE_H
#define NWLUA_ENGINE_H

#include <nwlua_core.hpp>
#if (defined NW_API)
#include <lua/nwlua_tools.h>

#ifndef NWLUA_CORE_HPP
#define NWLUA_CORE_HPP

#include <nw_core.hpp>

struct lua_State;

#define NWLUA_TNONE		    (-1)
#define NWLUA_TNIL		    0
#define NWLUA_TBOOLEAN		1
#define NWLUA_TLIGHTUSERDATA	2
#define NWLUA_TNUMBER		    3
#define NWLUA_TSTRING		    4
#define NWLUA_TTABLE		    5
#define NWLUA_TFUNCTION		6
#define NWLUA_TUSERDATA		7
#define NWLUA_TTHREAD		    8
#define NWLUA_NUMTYPES		9
#define NWLUA_TUPVAL			NWLUA_NUMTYPES

namespace NWLUA
{
	class NW_API lua_engine;

	struct NW_API lua_engine_info;
	struct NW_API lua_func_info;
	struct NW_API lua_table_info;

	using lua_state = lua_State;
	using lua_native_func = v1s(*)(lua_State*);
	using lua_up_values = dstack<ptr>;
}
namespace NWLUA
{
	enum lua_types : v1s {
		LT_NON = NWLUA_TNONE,
		LT_NIL = NWLUA_TNIL,
		LT_BOL = NWLUA_TBOOLEAN,
		LT_LUD = NWLUA_TLIGHTUSERDATA,
		LT_NUM = NWLUA_TNUMBER,
		LT_STR = NWLUA_TSTRING,
		LT_TAB = NWLUA_TTABLE,
		LT_FNC = NWLUA_TFUNCTION,
		LT_USD = NWLUA_TUSERDATA,
		LT_THR = NWLUA_TTHREAD,
		LT_UPV = NWLUA_TUPVAL,
	};
}

#endif	// NWLUA_CORE_HPP
namespace NWLUA
{
	/// lua_engine class
	/// -- Handles lua state
	/// -- Performs current script of the engine state
	class NW_API lua_engine
	{
		using info = lua_engine_info;
		using cinfo = const info;
	public:
		lua_engine();
		lua_engine(lua_engine& copy) = delete;
		~lua_engine();
		// --getters
		inline lua_state* get_lua_state() { return m_lstate; }
		inline cinfo& get_info() const    { return m_info; }
		template <typename vtype> vtype get_lua_value_glb(cstr name);
		template <typename vtype> vtype get_lua_value_loc(cstr name);
		template <typename vtype> vtype get_lua_value(cstr loc, cstr name);
		// --setters
		template <typename vtype> bool set_lua_value_glb(const vtype& value, cstr name);
		template <typename vtype> bool set_lua_value_loc(const vtype& value, cstr name);
		template <typename vtype> bool set_lua_value(const vtype& tValue, cstr loc, cstr name);
		// --predicates
		inline v1b is_running() const     { return m_is_running; }
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
		v1b m_is_running;
		mem_arena m_MemAllocator;

		lua_state* m_lstate;
		info m_info;
	private:	// Implementation Methods
		inline bool lua_move_glb(cstr name);
		inline bool lua_move_loc(cstr name);
		inline bool lua_move(const char* strLoc, const char* name);

		inline bool lua_move_tglb(cstr name, lua_types ltype);
		inline bool lua_move_tloc(cstr name, lua_types ltype);
		inline bool lua_move_t(cstr loc, cstr name, lua_types ltype);
		
		inline void lua_clear();
		inline void lua_clear(v1s count);
		inline bool lua_push(lua_func_info& reg_info);
		inline bool lua_push(lua_table_info& r_reg_tab);
		inline void write_err(const char* strFormat, ...);
		inline void write_info();

		static v1s lua_write(lua_state* lstate, ptr block_ptr, size block_size, ptr writer);
		static cstr lua_read(lua_state* lstate, ptr reader, size* block_size);
		static ptr lua_alloc(ptr allocator, ptr block_ptr, size new_size, size old_size);
		static ptr lua_warn(ptr warner, cstr msg, v1s code);
	};
}

#endif	// NWLUA_ENGINE_H
#endif	// NW_API