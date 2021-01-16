#include <nwg_pch.hpp>

#include <nwg_creator_state.h>

int main(int nArgs, char* strArgs[])
{
	try {
#if false
		//NW::CoreEngine* pGameEngine = &NW::CoreEngine::Get();
		//if (!pGameEngine->Init()) { return -1; }
		//pGameEngine->AddState(NW::MemSys::NewT<NWG::CreatorState>());
		//pGameEngine->Run();
#endif
#if false
		NWC::CmdEngine* pCmdEngine = &NWC::CmdEngine::Get();
		pCmdEngine->SetWndSize(40, 15);
		pCmdEngine->SetPxSize(8, 16);
		if (!pCmdEngine->Init()) { return -1; }
		pCmdEngine->Run();
		pCmdEngine->GetRunThread().join();
#endif
#if false
		//
#endif
	} catch (std::exception ext){
		NW_ERR(ext.what());
	}

	return 0;
}