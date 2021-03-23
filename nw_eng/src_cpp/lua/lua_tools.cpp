#include <nw_pch.hpp>
#include "lua_tools.h"

namespace NW
{
	// --setters
	void lua_engine_info::add_type(v1si ltype) { if (m_nlevel > m_stack_types.size()) rmv_type(1); m_stack_types[m_nlevel++] = static_cast<lua_types>(ltype); }
	void lua_engine_info::rmv_type(v1ui count) { while (count > 0 && m_nlevel > 0) { count--; m_nlevel--; m_stack_types[m_nlevel] = LT_NON; } }
	void lua_engine_info::reset() { rmv_type(m_nlevel); }
	// --operators
	stm_out& lua_engine_info::operator<<(stm_out& stm) const {
		stm << "--==lua_engine_info==--" << std::endl <<
			"The top of the stack:" << std::endl;
		for (v1ui ti = 0; ti < m_stack_types.size(); ti++) {
			stm << static_cast<v1si>(ti + 1) << "th type: ";
			switch (m_stack_types[ti]) {
			case LT_NON: stm << ("none");				break;
			case LT_NIL: stm << ("nil");				break;
			case LT_LUD: stm << ("light user data");	break;
			case LT_BOL: stm << ("boolean");			break;
			case LT_NUM: stm << ("number");				break;
			case LT_STR: stm << ("string");				break;
			case LT_FNC: stm << ("function");			break;
			case LT_TAB: stm << ("table");				break;
			case LT_USD: stm << ("user data");			break;
			case LT_THR: stm << ("thread");				break;
			case LT_UPV: stm << ("up value");			break;
			}
			std::cout << std::endl;
		}
		stm << "the bottom of the stack" << std::endl;
		stm << "--==/lua_engine_info==--" << std::endl;
		return stm;
	}
	stm_in& lua_engine_info::operator>>(stm_in& stm) {
		return stm;
	}
}