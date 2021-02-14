#include <nw_pch.hpp>

#define NWG_LAUNCH_TEST		1 << 0
#define NWG_LAUNCH_ENGINE	1 << 1
#define NWG_LAUNCH_NWC		1 << 2
#define NWG_LAUNCH			NWG_LAUNCH_ENGINE

#include <core/nw_engine_states.h>
#include <nwl_test.h>

int main(int nArgs, char* strArgs[])
{
	try {
#if (NWG_LAUNCH & NWG_LAUNCH_ENGINE)
		NW::CoreEngine* pGameEngine = &NW::CoreEngine::Get();

		pGameEngine->Run();
		if (pGameEngine->IsRunning()) { pGameEngine->GetRunThread().join(); }
#endif
#if (NWG_LAUNCH & NWG_LAUNCH_NWC)
		CmdEngine* pCmd = &CmdEngine::Get();
		
		pCmd->Run();
		pCmd->GetRunThread().join();
#endif
	}
	catch (NWL::AException exc) {
		NWL_ERR(exc.GetStr());
	}
	catch (std::exception exc) {
		NWL_ERR(exc.what());
	}
	catch (...) {
		NWL_ERR("somethig went wrong");
	}

	return 0;
}