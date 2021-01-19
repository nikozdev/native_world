#include <nwg_pch.hpp>

#define NWG_LAUNCH_TEST		1 << 0
#define NWG_LAUNCH_ENGINE	1 << 1
#define NWG_LAUNCH_GLIB		1 << 2
#define NWG_LAUNCH_CMD		1 << 3
#define NWG_LAUNCH			NWG_LAUNCH_ENGINE

#if (NWG_LAUNCH & NWG_LAUNCH_ENGINE)
#include <nwg_creator_state.h>
#endif	// NWG_LAUNCH
#if (NWG_LAUNCH & NWG_LAUNCH_GLIB)
#include <glib_texture.h>
#include <glib_shader.h>
#include <glib_material.h>
#endif	// NWG_LAUNCH
#if (NWG_LAUNCH & NWG_LAUNCH_CMD)
#endif	// NWG_LAUNCH
#if (NWG_LAUNCH & NWG_LAUNCH_TEST)
#include <nwg_mem_sys.h>
#endif	// NWG_LAUNCH

using namespace NWG;

int main(int nArgs, char* strArgs[])
{
	try {
	#if (NWG_LAUNCH & NWG_LAUNCH_ENGINE)
		NW::CoreEngine* pGameEngine = &NW::CoreEngine::Get();
		if (!pGameEngine->Init()) { return -1; }
		NWG::CreatorState crtState;
		pGameEngine->AddState(&crtState);
		pGameEngine->Run();
		// pGameEngine->GetRunThread()->join();
	#endif
	#if (NWG_LAUNCH & NWG_LAUNCH_CMD)
		CMD::CmdEngine* pCmdEngine = &CMD::CmdEngine::Get();
		pCmdEngine->SetWndSize(96, 32);
		pCmdEngine->SetPxSize(8, 16);
		if (!pCmdEngine->Init()) { return -1; }
		pCmdEngine->Run();
		
		CMD::CMenu cmMenu("root");
		cmMenu["menu0"].yCrd += 10;
		cmMenu["menu1"].yCrd += 20;
		cmMenu["menu2"].yCrd += 30;
		cmMenu.OnDraw();

		while (pCmdEngine->IsRunning()) {
			auto xyMsCrd = pCmdEngine->GetMouseMoveXY();
			if (pCmdEngine->GetKeyPressed(CMD::KC_LEFT)) {
				cmMenu.OnAction(CMD::CMA_MOVE_LT);
			}
			else if (pCmdEngine->GetKeyPressed(CMD::KC_RIGHT)) {
				cmMenu.OnAction(CMD::CMA_MOVE_RT);
			}
			else if (pCmdEngine->GetKeyPressed(CMD::KC_DOWN)) {
				cmMenu.OnAction(CMD::CMA_MOVE_DN);
			}
			else if (pCmdEngine->GetKeyPressed(CMD::KC_UP)) {
				cmMenu.OnAction(CMD::CMA_MOVE_UP);
			}
			
			if (pCmdEngine->GetKeyHeld(CMD::KC_R)) {
				if (pCmdEngine->GetMouseHeld(CMD::MSB_LEFT)) {
					pCmdEngine->DrawRectXY(
						pCmdEngine->GetMouseHeldX(CMD::MSB_LEFT), pCmdEngine->GetMouseHeldY(CMD::MSB_LEFT),
						pCmdEngine->GetMouseMoveX(), pCmdEngine->GetMouseMoveY(),
						CMD::CCN_BG_4 | CMD::CCN_FG_8);
					cmMenu.OnDraw();
				}
				else if (pCmdEngine->GetMouseHeld(CMD::MSB_RIGHT)) {
					pCmdEngine->DrawRectXY(
						pCmdEngine->GetMouseHeldX(CMD::MSB_RIGHT), pCmdEngine->GetMouseHeldY(CMD::MSB_RIGHT),
						pCmdEngine->GetMouseMoveX(), pCmdEngine->GetMouseMoveY(),
						CMD::CCN_BG_2 | CMD::CCN_FG_4);
					cmMenu.OnDraw();
				}
				pCmdEngine->DrawStrXY(pCmdEngine->GetWndWidth() - 16, 2, CMD::CCN_BG_1 | CMD::CCN_FG_16, &(std::string("move_x: ") + std::to_string(pCmdEngine->GetMouseMoveX()))[0]);
				pCmdEngine->DrawStrXY(pCmdEngine->GetWndWidth() - 16, 3, CMD::CCN_BG_1 | CMD::CCN_FG_16, &(std::string("move_y: ") + std::to_string(pCmdEngine->GetMouseMoveY()))[0]);
				pCmdEngine->DrawStrXY(pCmdEngine->GetWndWidth() - 16, 5, CMD::CCN_BG_1 | CMD::CCN_FG_16, &(std::string("held_x_l: ") + std::to_string(pCmdEngine->GetMouseHeldX(CMD::MSB_LEFT)))[0]);
				pCmdEngine->DrawStrXY(pCmdEngine->GetWndWidth() - 16, 6, CMD::CCN_BG_1 | CMD::CCN_FG_16, &(std::string("held_y_l: ") + std::to_string(pCmdEngine->GetMouseHeldY(CMD::MSB_LEFT)))[0]);
				pCmdEngine->DrawStrXY(pCmdEngine->GetWndWidth() - 16, 7, CMD::CCN_BG_1 | CMD::CCN_FG_16, &(std::string("held_x_r: ") + std::to_string(pCmdEngine->GetMouseHeldX(CMD::MSB_RIGHT)))[0]);
				pCmdEngine->DrawStrXY(pCmdEngine->GetWndWidth() - 16, 8, CMD::CCN_BG_1 | CMD::CCN_FG_16, &(std::string("held_y_r: ") + std::to_string(pCmdEngine->GetMouseHeldY(CMD::MSB_RIGHT)))[0]);
			}
			else if (pCmdEngine->GetKeyHeld(CMD::KC_L)) {
				if (pCmdEngine->GetMouseHeld(CMD::MSB_LEFT)) {
					pCmdEngine->DrawLineXY(
						pCmdEngine->GetMouseHeldX(CMD::MSB_LEFT), pCmdEngine->GetMouseHeldY(CMD::MSB_LEFT),
						pCmdEngine->GetMouseMoveX(), pCmdEngine->GetMouseMoveY(),
						CMD::CCN_BG_4 | CMD::CCN_FG_8);
					cmMenu.OnDraw();
				}
				else if (pCmdEngine->GetMouseHeld(CMD::MSB_RIGHT)) {
					pCmdEngine->DrawLineXY(
						pCmdEngine->GetMouseHeldX(CMD::MSB_RIGHT), pCmdEngine->GetMouseHeldY(CMD::MSB_RIGHT),
						pCmdEngine->GetMouseMoveX(), pCmdEngine->GetMouseMoveY(),
						CMD::CCN_BG_2 | CMD::CCN_FG_4);
					cmMenu.OnDraw();
				}
				pCmdEngine->DrawStrXY(pCmdEngine->GetWndWidth() - 16, 2, CMD::CCN_BG_1 | CMD::CCN_FG_16, &(std::string("move_x: ") + std::to_string(pCmdEngine->GetMouseMoveX()))[0]);
				pCmdEngine->DrawStrXY(pCmdEngine->GetWndWidth() - 16, 3, CMD::CCN_BG_1 | CMD::CCN_FG_16, &(std::string("move_y: ") + std::to_string(pCmdEngine->GetMouseMoveY()))[0]);
				pCmdEngine->DrawStrXY(pCmdEngine->GetWndWidth() - 16, 5, CMD::CCN_BG_1 | CMD::CCN_FG_16, &(std::string("held_x_l: ") + std::to_string(pCmdEngine->GetMouseHeldX(CMD::MSB_LEFT)))[0]);
				pCmdEngine->DrawStrXY(pCmdEngine->GetWndWidth() - 16, 6, CMD::CCN_BG_1 | CMD::CCN_FG_16, &(std::string("held_y_l: ") + std::to_string(pCmdEngine->GetMouseHeldY(CMD::MSB_LEFT)))[0]);
				pCmdEngine->DrawStrXY(pCmdEngine->GetWndWidth() - 16, 7, CMD::CCN_BG_1 | CMD::CCN_FG_16, &(std::string("held_x_r: ") + std::to_string(pCmdEngine->GetMouseHeldX(CMD::MSB_RIGHT)))[0]);
				pCmdEngine->DrawStrXY(pCmdEngine->GetWndWidth() - 16, 8, CMD::CCN_BG_1 | CMD::CCN_FG_16, &(std::string("held_y_r: ") + std::to_string(pCmdEngine->GetMouseHeldY(CMD::MSB_RIGHT)))[0]);
			}
		}
		pCmdEngine->GetRunThread()->join();
	#endif
	#if (NWG_LAUNCH & NWG_LAUNCH_GLIB)
	#endif
	#if (NWG_LAUNCH & NWG_LAUNCH_TEST)
		double nExeTime = 0.0f;
		std::chrono::steady_clock::time_point tpBeg;
		std::chrono::steady_clock::time_point tpEnd;
		std::chrono::duration<double> durExeTime(tpBeg - tpEnd);
		{
			tpBeg = std::chrono::steady_clock::now();
			MemSys::OnInit(1 << 20);
			for (UInt32 i = 0; i < 1'000'000; i++) {
				NW::GCamera* pMemInfo = nullptr;
				pMemInfo = MemSys::NewTArr<NW::GCamera>(64);
				MemSys::DelTArr<NW::GCamera>(pMemInfo, 64);
			}
			MemSys::OnQuit();
			tpEnd = std::chrono::steady_clock::now();
			durExeTime = std::chrono::duration(tpBeg - tpEnd);
			nExeTime = durExeTime.count();
			std::cout << "arena allocation takes " << nExeTime << " seconds" << std::endl;
		}
		{
			tpBeg = std::chrono::steady_clock::now();
			NW::MemSys::OnInit(1 << 20);
			for (UInt32 i = 0; i < 1'000'000; i++) {
				NW::GCamera* pMemInfo = nullptr;
				pMemInfo = NW::MemSys::NewTArr<NW::GCamera>(64);
				NW::MemSys::DelTArr<NW::GCamera>(pMemInfo, 64);
			}
			NW::MemSys::OnQuit();
			tpEnd = std::chrono::steady_clock::now();
			durExeTime = std::chrono::duration(tpBeg - tpEnd);
			nExeTime = durExeTime.count();
			std::cout << "heap allocation takes " << nExeTime << " seconds" << std::endl;
		}
#endif
	}
	catch (std::exception Exc) {
		NW_ERR(Exc.what());
	}

	return 0;
}