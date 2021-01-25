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
#include <glfw/glfw3.h>
#endif	// NWG_LAUNCH
#if (NWG_LAUNCH & NWG_LAUNCH_CMD)
#endif	// NWG_LAUNCH
#if (NWG_LAUNCH & NWG_LAUNCH_ENGINE)
#include <nwg_mem_sys.h>
#include <sys/nw_ent_sys.h>
#endif	// NWG_LAUNCH
#if (NWG_LAUNCH & NWG_LAUNCH_TEST)
#include <nwg_mem_sys.h>
#endif	// NWG_LAUNCH

int main(int nArgs, char* strArgs[])
{
	try {
#if (NWG_LAUNCH & NWG_LAUNCH_ENGINE)
		NW::CoreEngine* pGameEngine = &NW::CoreEngine::Get();
		NWG::CreatorState crtState;
		pGameEngine->AddState(&crtState);
		pGameEngine->Run(1 << 24);
#endif
#if (NWG_LAUNCH & NWG_LAUNCH_CMD)
		CMD::CmdEngine* pCmdEngine = &CMD::CmdEngine::Get();
		pCmdEngine->SetWndSize(96, 32);
		pCmdEngine->SetPxSize(8, 16);
		if (!pCmdEngine->Init(1 << 24)) { return -1; }
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
		pCmdEngine->GetRunThread().join();
#endif
#if (NWG_LAUNCH & NWG_LAUNCH_GLIB)
		GLIB::GEngine* pGEngine = &GLIB::GEngine::Get();

		if (glfwInit() != GLFW_TRUE) { return -1; }
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		GLFWwindow* pWindow = glfwCreateWindow(1200, 800, "graphics_library", nullptr, nullptr);
		glfwMakeContextCurrent(pWindow);
		glfwSetWindowCloseCallback(pWindow,
			[](GLFWwindow* pWindow)->void {glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE); GLIB::GEngine::Get().Quit(); });
		
 		if (!pGEngine->Init(1 << 24)) { return -1; }
		while (pGEngine->IsRunning()) {
			if (_kbhit()) { if (getchar() == KC_P) { pGEngine->Quit(); } }
			pGEngine->Update();
			glfwSwapBuffers(pWindow);
			glfwPollEvents();
		}
#endif
#if (NWG_LAUNCH & NWG_LAUNCH_TEST)
#if false
		Size szMem = (1 << 26);
		{
			NW::MemSys::OnInit(szMem);
			NW::EntSys::OnInit(szMem >> 8);
			NW::CmpSys::OnInit(szMem >> 4);
			for (UInt32 ei = 0; ei < 1 << 8; ei++) {
				NW::EntId eId = NW::EntSys::AddEnt();
				NW::CmpSys::AddCmp<NW::TransformCmp>(eId);
				auto pCmp = NW::CmpSys::GetCmp<NW::TransformCmp>(eId);
				pCmp->xyzCrd = { eId, eId, eId };
			}
			for (UInt32 ei = 0; ei < 1 << 8; ei++) {
				auto pCmp = NW::CmpSys::GetCmp<NW::TransformCmp>(ei);
				NW::CmpSys::RmvCmp<NW::TransformCmp>(ei);
				NW::EntSys::RmvEnt(ei);
			}
			NW::CmpSys::OnQuit();
			NW::EntSys::OnQuit();
			NW::MemSys::OnQuit();
		}
#endif
		NW::TimeSys::Update();
		NW::LogSys::WriteStr("The test takes {flt} milliseconds\n", NW::TimeSys::GetRealDelta());
#endif
	}
	catch (std::exception Exc) {
		NWL_ERR(Exc.what());
	}

	return 0;
}