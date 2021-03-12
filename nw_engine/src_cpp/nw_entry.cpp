#include <nw_pch.hpp>

#define NW_LAUNCH_TEST		1 << 0
#define NW_LAUNCH_ENGINE	1 << 1
#define NW_LAUNCH_NW		1 << 2
#define NW_LAUNCH			NW_LAUNCH_ENGINE

#include <core/nw_engine.h>
#include <core/nw_engine_states.h>

#include <native_console.hpp>

int main(int nArgs, char* strArgs[])
{
	try {
#if (NW_LAUNCH & NW_LAUNCH_ENGINE)
		NW::core_engine game = NW::core_engine();
		NW::gfx_state gfx = NW::gfx_state(game);
		//NW::gui_state gui = NW::gui_state(game);
		game.add_state(gfx);
		//game.add_state(gui);
		game.run();
		if (game.get_run_thread()->joinable()) { game.get_run_thread()->join(); }
#endif
#if (NW_LAUNCH & NW_LAUNCH_NW)
#endif
#if (NW_LAUNCH & NW_LAUNCH_TEST)
		if (false) {
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
				throw(Codeerror("failed to create a process", GetLastError()));
			}
			WaitForSingleObject(pcInfo.hProcess, INFINITE);
			CloseHandle(pcInfo.hProcess);
			CloseHandle(pcInfo.hThread);
		}
#endif
	}
	catch (NW::error& exc) { std::cout << exc; }
	catch (std::exception& exc) { NW_ERR(exc.what()); }
	catch (...) { NW_ERR("somethig went wrong"); }

	return 0;
}