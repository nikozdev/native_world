#include <nwg_pch.hpp>

#define NWG_LAUNCH_TEST		1 << 0
#define NWG_LAUNCH_ENGINE	1 << 1
#define NWG_LAUNCH_CMD		1 << 2
#define NWG_LAUNCH_GLIB		1 << 3
#define NWG_LAUNCH			NWG_LAUNCH_ENGINE

#if (NWG_LAUNCH & NWG_LAUNCH_ENGINE)
#include <nwg_core_states.h>
#endif	// NWG_LAUNCH
#if (NWG_LAUNCH & NWG_LAUNCH_TEST)
#include <nwlib/nwl_test.h>
#endif	// NWG_LAUNCH

int main(int nArgs, char* strArgs[])
{
	try {
#if (NWG_LAUNCH & NWG_LAUNCH_ENGINE)
		NW::CoreEngine* pGameEngine = &NW::CoreEngine::Get();
		NWG::GuiState guiState;
		NWG::DrawerState drwState;

		pGameEngine->AddState(drwState);
		pGameEngine->AddState(guiState);
		if (!pGameEngine->Init()) { return -1; }
		while (pGameEngine->IsRunning()) { pGameEngine->Update(); }
		pGameEngine->Quit();
		//pGameEngine->Run();
#endif
#if (NWG_LAUNCH & NWG_LAUNCH_CMD)
		TimeSys::Update();
		CMD::CmdEngine* pCmd = &CMD::CmdEngine::Get();
		pCmd->SetPxSize(16, 16);
		pCmd->SetWndSize(100, 35);
		pCmd->Run();
		while (pCmd->IsRunning()) {
			pCmd->SwapBuffers();
			CMD::CmdFrameBuf* pfb = pCmd->GetFrameBuf();
			pfb->Clear();
			if (pCmd->GetMouseHeld(CMD::MSB_LEFT)) {
				pfb->DrawRectXY(
					pCmd->GetMouseHeldX(CMD::MSB_LEFT), pCmd->GetMouseHeldY(CMD::MSB_LEFT),
					pCmd->GetMouseMoveX(), pCmd->GetMouseMoveY(),
					CMD::CCN_BG_1 | CMD::CCN_FG_16);
			}
			else if (pCmd->GetMouseHeld(CMD::MSB_RIGHT)) {
				pfb->DrawRectXY(
					pCmd->GetMouseHeldX(CMD::MSB_RIGHT), pCmd->GetMouseHeldY(CMD::MSB_RIGHT),
					pCmd->GetMouseMoveX(), pCmd->GetMouseMoveY(),
					CMD::CCN_BG_8 | CMD::CCN_FG_8);
			}
			if (pCmd->GetKeyPressed(CMD::KC_ESCAPE)) { pCmd->Quit(); }
		}
		pCmd->GetRunThread().join();
		TimeSys::Update();
		NW::LogSys::WriteStr("The test takes {flt} milliseconds\n", TimeSys::GetDeltaMs());
#endif
	}
	catch (std::exception Exc) {
		NWL_ERR(Exc.what());
	}

	return 0;
}