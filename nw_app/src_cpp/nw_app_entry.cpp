#include "nw_app_pch.hpp"

#define NW_APP_LAUNCH_TEST     1 << 0
#define NW_APP_LAUNCH_ENGINE   1 << 1
#define NW_APP_LAUNCH_CONSOLE  1 << 2
#define NW_APP_LAUNCH          NW_APP_LAUNCH_ENGINE

#include "nw_app.hpp"

int main(int nof_arguments, char* arguments[])
{
	try {
#if (NW_APP_LAUNCH & NW_APP_LAUNCH_ENGINE)
		NW::core_engine game = NW::core_engine("native_world");
		game.run();
		//game.new_state<NW::gui_core_state>();
		game.new_state<NW::gfx_core_state>();
		if (game.get_run_thread()->joinable()) { game.get_run_thread()->join(); }
#endif
#if (NW_APP_LAUNCH & NW_APP_LAUNCH_CONSOLE)
#endif
#if (NW_APP_LAUNCH & NW_APP_LAUNCH_TEST)
#	if (NW_FALSE)
		// build a structure
		char* buf = NW::mem_sys::get().new_arr<char>(1 << 12);
		if constexpr (NW_TRUE) {
			NW::mem_layt root("root", NW::mem_layt::elems_t({
				NW::mem_layt("0", NW::type_info::get_type<NW::v1f>(), NW_NULL),
				NW::mem_layt("1", NW::type_info::get_type<NW::v1f>(), NW_NULL),
				NW::mem_layt("2", NW::type_info::get_type<NW::v1f>(), NW_NULL),
				NW::mem_layt("3", NW::type_info::get_type<NW::v1f>(), NW_NULL),
				NW::mem_layt("branch_0", NW::mem_layt::elems_t({
					NW::mem_layt("0", NW::type_info::get_type<NW::v1u>(), NW_NULL ),
					NW::mem_layt("1", NW::type_info::get_type<NW::v1u>(), NW_NULL ),
					NW::mem_layt("2", NW::type_info::get_type<NW::v1u>(), NW_NULL ),
					NW::mem_layt("branch_0_0", NW::mem_layt::elems_t({
						NW::mem_layt("1", NW::type_info::get_type<NW::v4f>(), NW_NULL),
						NW::mem_layt("2", NW::type_info::get_type<NW::v4f>(), NW_NULL),
						NW::mem_layt("3", NW::type_info::get_type<NW::v4f>(), NW_NULL),
						NW::mem_layt("4", NW::type_info::get_type<NW::v4f>(), NW_NULL),
						NW::mem_layt("branch_0_0_0", NW::mem_layt::elems_t({
							NW::mem_layt("1", NW::type_info::get_type<NW::v4f>(), NW_NULL),
							NW::mem_layt("2", NW::type_info::get_type<NW::v4f>(), NW_NULL),
							NW::mem_layt("3", NW::type_info::get_type<NW::v4f>(), NW_NULL),
							NW::mem_layt("4", NW::type_info::get_type<NW::v4f>(), NW_NULL),
						}) ),
					}) ),
				}) )
			}) );
			root.add_node("branch_1");
			root["branch_1"].add_node("branch_1_0");
			root["branch_1"]["branch_1_0"].add_node("branch_1_0_0", NW::mem_layt::elems_t({
					NW::mem_layt("color", NW::type_info::get_type<NW::v4u08>()),
					NW::mem_layt("direction", NW::type_info::get_type<NW::v3f>()),
				}) );
			root["branch_1"].add_node<NW::dstr>("name");
			root["branch_1"].add_node("name__", NW::type_info::get_type<NW::dstr>());
			NW_CHECK(root.remake(buf), "failed to build the structure!", return -1);
			if constexpr (NW_FALSE) {
				NW::stm_io_str stm;
				stm << root;
				std::cout << stm.str();
				NW::mem_layt elem;
				stm >> elem;
			}
			if constexpr (NW_TRUE) {
				std::cout << root.get_tree_str();
			}
		}
		NW::mem_sys::get().del_arr<char>(buf, 1 << 12);
#	elif (NW_TRUE)
		for (NW::v1u itr = 0u; itr < 1000u; itr++) {
			NW::mem_ref<NW::mem_layt> layt;
			layt.make_ref<NW::mem_layt>();
			NW::byte_t* buf = NW::mem_sys::get().new_arr<NW::byte_t>(1000u);
			layt.make_ref<NW::mem_layt>();
			NW::mem_sys::get().del_arr<NW::byte_t>(buf, 1000u);
			layt.make_ref<NW::mem_layt>();
		}
#	elif (NW_FALSE)
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