#include <nwg_pch.hpp>

#define NWG_LAUNCH_TEST		1 << 0
#define NWG_LAUNCH_ENGINE	1 << 1
#define NWG_LAUNCH_CMD		1 << 2
#define NWG_LAUNCH_GLIB		1 << 3
#define NWG_LAUNCH			NWG_LAUNCH_ENGINE

#include <nwg_engine_states.h>
#include <nwlib/nwl_test.h>

int main(int nArgs, char* strArgs[])
{
	try {
#if (NWG_LAUNCH & NWG_LAUNCH_ENGINE)
		NW::CoreEngine* pGameEngine = &NW::CoreEngine::Get();
		GLIB::GraphEngine* pGraphEngine = &GLIB::GraphEngine::Get();
		//CMD::CmdEngine* pCmdEngine = &CMD::CmdEngine::Get();

		NWG::GraphState graphState;
		NWG::GuiState guiState;
		NWG::GamerState gameState;
		//NWG::CmdMenuState cmenuState;

		pGameEngine->AddState(gameState);

		pGraphEngine->AddState(graphState);
		pGraphEngine->AddState(guiState);
		
		//pCmdEngine->AddState(cmenuState);
		
		//pCmdEngine->Run();
		if (!pGameEngine->Init()) { return -1; }
		while (pGameEngine->IsRunning()) { pGameEngine->Update(); }
		pGameEngine->Quit();
		//pCmdEngine->GetRunThread().join();
#endif
#if (NWG_LAUNCH & NWG_LAUNCH_CMD)
		CmdEngine* pCmd = &CmdEngine::Get();
		CMD::CmdMenuState cmState;
		//CMD::CmdWritterState cwState;

		pCmd->AddState(cmState);
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