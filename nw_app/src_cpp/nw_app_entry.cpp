#include "nw_app_pch.hpp"

#define NW_APP_LAUNCH_TEST     1 << 0
#define NW_APP_LAUNCH_ENGINE   1 << 1
#define NW_APP_LAUNCH_CONSOLE  1 << 2
#define NW_APP_LAUNCH          NW_APP_LAUNCH_ENGINE

#include "nw_app.hpp"

#include "std/nw_std_graph.h"

#if (defined NW_APP_LAUNCH)
int main(int nof_arguments, char* arguments[])
{
	try {
#	if (NW_APP_LAUNCH & NW_APP_LAUNCH_ENGINE)
		auto& game = NW::app_engine::get();
		game.run();
		game.add_state<NW::app_state_game>();
		// game.add_state<NW::app_state_gui>();
		if (game.get_run_thread()->joinable()) { game.get_run_thread()->join(); }
#	endif
#	if (NW_APP_LAUNCH & NW_APP_LAUNCH_CONSOLE)
#	endif
#	if (NW_APP_LAUNCH & NW_APP_LAUNCH_TEST)
		NW_CHECK(NW::mem_sys::get().init(1 << 23), "failed init!", return NW_FALSE);
#		if (NW_FALSE)
		if constexpr (NW_FALSE) {
			STARTUPINFO spInfo{ NW_NULL };
			PROCESS_INFORMATION pcInfo{ NW_NULL };
			if (!CreateProcess(
				R"(F:\dev\lua_jit\lua_jit.exe)",
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
			)) { throw(NW::a_error("failed to create a process", ::GetLastError())); }
			WaitForSingleObject(pcInfo.hProcess, INFINITE);
			CloseHandle(pcInfo.hProcess);
			CloseHandle(pcInfo.hThread);
		}
		if constexpr (NW_FALSE) {
			NW::byte_t* buf = NW::mem_sys::get().new_arr<NW::byte_t>(1 << 8);
			for (NW::a_iter_array iter_beg(buf), iter_end(buf + (1 << 8)); iter_beg != iter_end; iter_beg++) {
				iter_beg = NW::get_rand<NW::byte_t>(-128, 127);
				std::cout << iter_beg.get<NW::byte_t>();
			}
			std::cout << NW_STR_EOL;
			for (int itr = 0; itr < 1 << 8; itr++) { std::cout << buf[itr]; }
			std::cout << NW_STR_EOL;
			NW::mem_sys::get().del_arr<NW::byte_t>(buf, 1 << 8);
		}
		if constexpr (NW_FALSE) {
			NW::byte_t* buf = NW::mem_sys::get().new_arr<NW::byte_t>(1 << 8);
			for (NW::t_iter_array<NW::byte_t> iter_beg(buf), iter_end(buf + (1 << 8)); iter_beg != iter_end; iter_beg++) {
				iter_beg = NW::get_rand<NW::byte_t>(-128, 127);
				std::cout << iter_beg;
			}
			std::cout << NW_STR_EOL;
			for (int itr = 0; itr < 1 << 8; itr++) { std::cout << buf[itr];}
			std::cout << NW_STR_EOL;
			NW::mem_sys::get().del_arr<NW::byte_t>(buf, 1 << 8);
		}
		if constexpr (NW_FALSE) {
			NW::time_state timer;
			timer.update();
			for (auto itr = 0u; itr < 1000u; itr++) {
				if (itr % 100u == 0u) { std::cout << itr; }
				NW::t_array<NW::v1u> arr;
				for (auto ie = 0u; ie < 100u; ie++) { arr.add_back(ie); }
				for (auto ie = 0u; ie < 100u; ie++) { arr.rmv_back(); }
			}
			timer.update();
			std::cout << " \nyour array worked " << timer.get_delta() << " seconds;" << NW_STR_EOL;
			for (auto itr = 0u; itr < 1000u; itr++) {
				if (itr % 100u == 0u) { std::cout << itr; }
				std::vector<NW::v1u> arr;
				for (auto ie = 0u; ie < 100u; ie++) { arr.push_back(ie); }
				for (auto ie = 0u; ie < 100u; ie++) { arr.pop_back(); }
			}
			timer.update();
			std::cout << " \nstd vector worked " << timer.get_delta() << " seconds;" << NW_STR_EOL;
		}
		if constexpr (NW_FALSE) {
			NW::mem_layt layt("root");
			layt.add_node<int>("inty").add_node<float>("floaty").add_node("layty");
			layt["layty"].add_node("layty_child");
			NW_CHECK(layt.remake(), "failed remake", return -1);
			std::cout << layt << NW_STR_EOL;
			std::cout << layt.get_tree_str() << NW_STR_EOL;
		}
#		endif
		NW_CHECK(NW::mem_sys::get().quit(), "failed quit!", return NW_FALSE);
#	endif
	}
	catch (NW::a_error& exc) { NW_ERROR(exc.get_str(), return -1); }
	catch (std::exception& exc) { NW_ERROR(exc.what(), return -1); }
	catch (...) { NW_ERROR("somethig went wrong", return -1); }

	return 0;
}
#endif	// NW_APP_LAUNCH