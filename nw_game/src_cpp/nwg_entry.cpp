#include <nwg_pch.hpp>

#define NWG_LAUNCH_TEST		1 << 0
#define NWG_LAUNCH_ENGINE	1 << 1
#define NWG_LAUNCH_NWC		1 << 2
#define NWG_LAUNCH			NWG_LAUNCH_ENGINE

#include <nwg_engine_states.h>
#include <nwl_test.h>

int main(int nArgs, char* strArgs[])
{
	try {
#if (NWG_LAUNCH & NWG_LAUNCH_ENGINE)
		NW::CoreEngine* pGameEngine = &NW::CoreEngine::Get();
		//NWC::CmdEngine* pCmdEngine = &NWC::CmdEngine::Get();

		NW::GfxState gfxState;
		NWG::GuiState guiState;
		NWG::GamerState gameState;
		//NWG::CmdMenuState cmenuState;

		pGameEngine->AddState(gameState);
		pGameEngine->AddState(gfxState);

		pGameEngine->Run();
		if (pGameEngine->IsRunning()) { pGameEngine->GetRunThread().join(); }
#endif
#if (NWG_LAUNCH & NWG_LAUNCH_NWC)
		CmdEngine* pCmd = &CmdEngine::Get();
		//NWC::CmdMenuState cmState;
		//NWC::CmdWritterState cwState;

		//pCmd->AddState(cmState);
		//pCmd->AddState(cwState);
		pCmd->Run();
		pCmd->GetRunThread().join();
#endif
	}
	catch (std::exception Exc) {
		NWL_ERR(Exc.what());
	}

	return 0;
}