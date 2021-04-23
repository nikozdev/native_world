#include "nw_app_pch.hpp"

#define NW_APP_LAUNCH_TEST     1 << 0
#define NW_APP_LAUNCH_ENGINE   1 << 1
#define NW_APP_LAUNCH_CONSOLE  1 << 2
#define NW_APP_LAUNCH          NW_APP_LAUNCH_ENGINE

#include "nw_app.hpp"

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
#		if (NW_FALSE)
#		elif (NW_FALSE)
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
#		endif
#	endif
	}
	catch (NW::a_error& exc) { NW_ERROR(exc.get_str(), return -1); }
	catch (std::exception& exc) { NW_ERROR(exc.what(), return -1); }
	catch (...) { NW_ERROR("somethig went wrong", return -1); }

	return 0;
}
#endif	// NW_APP_LAUNCH