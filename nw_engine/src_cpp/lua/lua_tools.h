#ifndef LUA_TOOLS_H
#define LUA_TOOLS_H
#include <lua_core.hpp>
namespace NW
{
	/// lua_engine_info struct
	/// Description:
	/// -- Contains strings and numbers with the information about lua stack:
	/// ==| size
	/// ==| types
	/// ==| last level of search
	struct NW_API lua_engine_info : public a_info
	{
	public:
		using stack_types_info = carray<lua_types, 20>;
	public:
		lua_engine_info() { memset(&m_stack_types[0], LT_NON, sizeof(si8)); }
		// --getters
		inline lua_types get_stack_type(ui8 unIndex) { return m_stack_types[unIndex]; }
		inline lua_types get_last_type() { return m_nlevel == 0 ? LT_NIL : m_stack_types[m_nlevel - 1]; }
		inline ui8 get_level() { return m_nlevel; }
		// --setters
		void add_type(si32 ltype);
		void rmv_type(ui8 count);
		void reset();
		// --operators
		virtual out_stream& operator<<(out_stream& stm) const override;
	private:
		ui8 m_nlevel = 0;
		stack_types_info m_stack_types;
	};
	/// lua_registrable_function_info struct
	/// description:
	/// --this is a handler of lua for a function
	/// --it contains the function adress pointer or lua code (one of that)
	/// --also, it contains upvalues adresses, points to the owner table
	struct NW_API lua_func_info
	{
	public:
		cstring name;
		cstring loc;
		lua_native_func native_func;
		lua_up_values up_vals;
	public:
		lua_func_info(cstring lua_name,
			lua_native_func native_function) :
			name(lua_name), loc(""),
			native_func(native_function) { }
		lua_func_info(cstring lua_name, cstring lua_location,
			lua_native_func native_function) :
			name(lua_name), loc(lua_location),
			native_func(native_function) { }
		lua_func_info(cstring lua_name, cstring lua_location,
			lua_native_func native_function, lua_up_values& up_values) :
			name(lua_name), loc(lua_location),
			native_func(native_function), up_vals(up_values) { }
	};
	/// lua_registrable_table_info struct
	/// description:
	/// --this is an information about a table in lua
	/// --contains:
	/// ==| the name;
	/// ==| over table - where it is allocated;
	/// ==| metatable - which methods are inherited;
	/// ==| default methods
	/// ==| registrated methods
	struct NW_API lua_table_info
	{
	public:
		cstring name;
		cstring loc;
		darray<lua_func_info*> methods;
		lua_table_info* meta_table;
	public:
		lua_table_info(cstring lua_location, cstring lua_name, lua_table_info* lua_table) :
			name(lua_name), loc(lua_location),
			meta_table(lua_table) { }
	};
	/// lua_reader_state struct
	struct NW_API lua_reader_state
	{
		const dstring* chunk = nullptr;
		bit is_read = false;
	};
}
#endif	// LUA_CORE_H
/// Destroy function must to:
/// -> Get the object via "lua_touserdata(LuaNative, -1)"
/// -> Call the destructor of the object
/// Index function must to:
/// -> Get the object via "lua_touserdata(LuaNative, -2)"
/// -> Get the index value via "lua_tostring(LuaNative, -1)"
/// -> Switch between index values and behave depending on it
/// -> If there is no such index - get values from the script stack
/// -> "lua_getuservalue(-1)"
/// if it is - pushvalue(2), gettable(-2)
/// else "getglobal(RegTableName)", push index string
/// -> rawget(-2)