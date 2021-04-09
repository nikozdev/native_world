#include "nw_app_pch.hpp"

#define NW_APP_LAUNCH_TEST     1 << 0
#define NW_APP_LAUNCH_ENGINE   1 << 1
#define NW_APP_LAUNCH_CONSOLE  1 << 2
#define NW_APP_LAUNCH          NW_APP_LAUNCH_TEST

#include "nw_app.hpp"

template<typename vtype, NW::csize mat_size_x, NW::csize mat_size_y>
static constexpr vtype get_deter(const NW::mat_t<vtype, mat_size_x, mat_size_y>& matrix) {
	using namespace NW;
	vtype result = static_cast<vtype>(0);
	if constexpr (mat_size_x == 1u && mat_size_y == 1u) {
		result = matrix[0][0];
	}
	else if constexpr (mat_size_x == 2u && mat_size_y == 2u) {
		result = (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
	}
	else if constexpr (mat_size_x > 2u && mat_size_y > 2u) {
		v1s sign = +1; // -1 power(itr)
		for (v1u itr = 0u; itr < mat_size_x; itr++) {
			result += sign * matrix[0u][itr] * get_deter(matrix.erase(0u, itr));
			sign = -sign;
		}
	}
	else if constexpr (mat_size_x < 1 || mat_size_y < 1) {
		static_assert(mat_size_x >= 1 && mat_size_y >= 1, "cannot find determinant!");
	}
	return result;
}

static inline NW::dstr make_str_tree(NW::mem_elem& node, NW::cv1u generation = 0u) {
	static NW::size total_size = 0u;
	NW::stm_io_str stm{ };
	NW::schar offset[32]{ };
	
	if (generation == 0u) { total_size = 0u; }

	for (NW::v1u itr = 0u; itr < generation * 4; itr++) { offset[itr] = ' '; }
	stm << &offset[4u] << "[" << node.get_name() << "]:" << "{" << NW_STR_EOL;
	stm << &offset[0u] << "name:" << node.get_name() << ";" << NW_STR_EOL;
	stm << &offset[0u] << "type:" << node.get_type_info() << ";" << NW_STR_EOL;
	stm << &offset[0u] << "size:" << node.get_size() << ";" << NW_STR_EOL;
	stm << &offset[0u] << "offset:" << node.get_offset() << ";" << NW_STR_EOL;
	stm << &offset[0u] << "count:" << node.get_count() << ";" << NW_STR_EOL;
	for (auto& inode : node) { stm << make_str_tree(inode, generation + 1u); }
	stm << &offset[4u] << "}" << "[" << node.get_name() << "]" << ";" << NW_STR_EOL;

	total_size += sizeof(node);
	if (generation == 0u) { stm << "total_size:" << total_size << NW_STR_EOL; }

	return stm.str();
}

int main(int nof_arguments, char* arguments[])
{
	try {
#if (NW_APP_LAUNCH & NW_APP_LAUNCH_ENGINE)
		NW::core_engine game = NW::core_engine("native_world");
		game.run();
		game.new_state<NW::gui_core_state>();
		// game.new_state<NW::gfx_core_state>();
		if (game.get_run_thread()->joinable()) { game.get_run_thread()->join(); }
#endif
#if (NW_APP_LAUNCH & NW_APP_LAUNCH_CONSOLE)
#endif
#if (NW_APP_LAUNCH & NW_APP_LAUNCH_TEST)
#	if (NW_TRUE)
		// build a structure
		char* buf = NW::mem_sys::get().new_arr<char>(1 << 12);
		if constexpr (NW_TRUE) {
			NW::type_info::get<NW::mem_elem>();
			NW::mem_elem root("root", {
				NW::mem_elem("0", NW::type_info::get_id<NW::v1f>(), NW_NULL),
				NW::mem_elem("1", NW::type_info::get_id<NW::v1f>(), NW_NULL),
				NW::mem_elem("2", NW::type_info::get_id<NW::v1f>(), NW_NULL),
				NW::mem_elem("3", NW::type_info::get_id<NW::v1f>(), NW_NULL),
				NW::mem_elem("branch_0", {
					NW::mem_elem("0", NW::type_info::get_id<NW::v1u>(), NW_NULL ),
					NW::mem_elem("1", NW::type_info::get_id<NW::v1u>(), NW_NULL ),
					NW::mem_elem("2", NW::type_info::get_id<NW::v1u>(), NW_NULL ),
					NW::mem_elem("branch_0_0", {
						NW::mem_elem("1", NW::type_info::get_id<NW::v4f>(), NW_NULL),
						NW::mem_elem("2", NW::type_info::get_id<NW::v4f>(), NW_NULL),
						NW::mem_elem("3", NW::type_info::get_id<NW::v4f>(), NW_NULL),
						NW::mem_elem("4", NW::type_info::get_id<NW::v4f>(), NW_NULL),
						NW::mem_elem("branch_0_0_0", {
							NW::mem_elem("1", NW::type_info::get_id<NW::v4f>(), NW_NULL),
							NW::mem_elem("2", NW::type_info::get_id<NW::v4f>(), NW_NULL),
							NW::mem_elem("3", NW::type_info::get_id<NW::v4f>(), NW_NULL),
							NW::mem_elem("4", NW::type_info::get_id<NW::v4f>(), NW_NULL),
						}),
					} ),
				} )
			} );
			NW_CHECK(root.remake(buf), "failed to build the structure!", return -1);
			std::cout << make_str_tree(root) << NW_STR_EOL;
		}
		NW::mem_sys::get().del_arr<char>(buf, 1 << 12);
#	endif
#	if (NW_FALSE)
		STARTUPINFO spInfo{ 0 };
		PROCESS_INFORMATION pcInfo{ 0 };
		if (!CreateProcess(
			LR"(F:\dev\lua_jit\lua_jit.exe)",
			//&(reinterpret_cast<TCHAR*>(R"(F:\dev\lua_jit\lua_jit.exe)"))[0],
			NULL,		// cmd arguments
			NULL,		// process is not inheritable
			NULL,		// not inherit the handle
			FALSE,		// handle inheritance to false
			0,			// no creation flags
			NULL,		// parrent's environment
			NULL,		// parent's directory
			&spInfo,
			&pcInfo
		)) {
			throw(NW::error("failed to create a process", ::GetLastError()));
		}
		WaitForSingleObject(pcInfo.hProcess, INFINITE);
		CloseHandle(pcInfo.hProcess);
		CloseHandle(pcInfo.hThread);
#	endif
#endif
	}
	catch (NW::a_error& exc) { NW_ERROR(exc.get_str(), return -1); }
	catch (std::exception& exc) { NW_ERROR(exc.what(), return -1); }
	catch (...) { NW_ERROR("somethig went wrong", return -1); }

	return 0;
}