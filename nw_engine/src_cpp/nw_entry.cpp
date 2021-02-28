#include <nw_pch.hpp>

#define NW_LAUNCH_TEST		1 << 0
#define NW_LAUNCH_ENGINE	1 << 1
#define NW_LAUNCH_NWC		1 << 2
#define NW_LAUNCH			NW_LAUNCH_ENGINE

#include <core/nw_engine.h>
#include <core/nw_engine_states.h>

#include <native_console.hpp>
#include <nwl_test.h>

int main(int nArgs, char* strArgs[])
{
	try {
#if (NW_LAUNCH & NW_LAUNCH_ENGINE)
		NW::CoreEngine Game = NW::CoreEngine();
		Game.AddState<NW::GfxState>();
		Game.Run();
		if (Game.GetRunThread()->joinable()) { Game.GetRunThread()->join(); }
#endif
#if (NW_LAUNCH & NW_LAUNCH_NWC)
		NWC::CmdEngine Cmd = NWC::CmdEngine();
		pCmd->Run();
		if (pCmd->GetRunThread().joinable()) { pCmd->GetRunThread().join(); }
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
				throw(CodeException("failed to create a process", GetLastError()));
			}
			WaitForSingleObject(pcInfo.hProcess, INFINITE);
			CloseHandle(pcInfo.hProcess);
			CloseHandle(pcInfo.hThread);
		}
#endif
	}
	catch (NWL::Exception& exc) { std::cout << exc; }
	catch (std::exception& exc) { NWL_ERR(exc.what()); }
	catch (...) { NWL_ERR("somethig went wrong"); }

	return 0;
}