#include <nwg_pch.hpp>

#include <nwg_creator_state.h>

int main(int nArgs, char* strArgs[])
{
	try {
	#if false
		NW::CoreEngine* pGameEngine = &NW::CoreEngine::Get();
		if (!pGameEngine->Init()) { return -1; }
		pGameEngine->AddState(NW::MemSys::NewT<NWG::CreatorState>());
		pGameEngine->Run();
	#endif
	#if true
		CMD::CmdEngine* pCmdEngine = &CMD::CmdEngine::Get();
		pCmdEngine->SetWndSize(64, 32);
		pCmdEngine->SetPxSize(8, 16);
		if (!pCmdEngine->Init()) { return -1; }
		
		pCmdEngine->Run();
		
		CMD::CWidget cwRoot("root");
		cwRoot.SetEnabled(true);
		cwRoot.SetCoord(10, 10);
		cwRoot.OnDraw();
		
		while (pCmdEngine->IsRunning()) {
			if (pCmdEngine->GetKeyPressed(CMD::KC_A)) {
				cwRoot.OnActivate(!cwRoot.IsActive());
			}
			else if (pCmdEngine->GetKeyPressed(CMD::KC_H)) {
				cwRoot.OnHovered(pCmdEngine->GetMouseMoveX(), pCmdEngine->GetMouseMoveY());
			}
			else if (pCmdEngine->GetKeyPressed(CMD::KC_E)) {
				cwRoot.SetEnabled(!cwRoot.IsEnabled());
			}
			if (pCmdEngine->GetKeyHeld(CMD::KC_C)) {
				cwRoot.SetCoord(pCmdEngine->GetMouseMoveX(), pCmdEngine->GetMouseMoveY());
			}
		}
		pCmdEngine->GetRunThread().join();
	#endif
	#if false
		GLIB::GEngine* pGEngine = &GLIB::GEngine::Get();
		if (!pGEngine->Init(GLIB::WAPI_GLFW, GLIB::GAPI_OPENGL)) { return -1; }
		while (pGEngine->IsRunning()) { pGEngine->Update(); }
		pGEngine->Run();
		pGEngine->GetRunThread().join();
	#endif
	} catch (std::exception Ex){
		NW_ERR(Ex.what());
	}

	return 0;
}