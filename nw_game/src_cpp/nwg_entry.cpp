#include <nwg_pch.hpp>

#include <nwg_creator_state.h>

#include <glib_texture.h>
#include <glib_shader.h>
#include <glib_material.h>

#define NWG_LAUNCH_ENGINE	1 << 1
#define NWG_LAUNCH_GLIB		1 << 2
#define NWG_LAUNCH_CMD		1 << 3
#define NWG_LAUNCH			NWG_LAUNCH_GLIB

int main(int nArgs, char* strArgs[])
{
	try {
	#if (NWG_LAUNCH & NWG_LAUNCH_ENGINE)
		NW::CoreEngine* pGameEngine = &NW::CoreEngine::Get();
		if (!pGameEngine->Init()) { return -1; }
		pGameEngine->AddState(NW::MemSys::NewT<NWG::CreatorState>());
		pGameEngine->Run();
	#endif
	#if (NWG_LAUNCH & NWG_LAUNCH_CMD)
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
	#if (NWG_LAUNCH & NWG_LAUNCH_GLIB)
		GLIB::GEngine* pGEngine = &GLIB::GEngine::Get();
		if (!pGEngine->Init(GLIB::WAPI_GLFW, GLIB::GAPI_OPENGL)) { return -1; }
		GLIB::DrawObjectData DOData;
		std::vector<float> vtxData = {
			-0.5f,	-0.5f,	0.0f,		0.0f,	1.0f,	1.0f,	1.0f,		0.0f,	1.0f,		0.0f,			1.0f,		0.0f,			0.0f,			0.0f,
																												0.0f,		cosf(time(0)),	-sinf(time(0)),	0.0f,
																												0.0f,		sinf(time(0)),	cosf(time(0)),	0.0f,
																												0.0f,		0.0f,			0.0f,			1.0f,
			-0.5f,	0.5f,	0.0f,		0.0f,	1.0f,	1.0f,	1.0f,		0.0f,	0.0f,		0.0f,			1.0f,		0.0f,			0.0f,			0.0f,
																												0.0f,		cosf(time(0)),	-sinf(time(0)),	0.0f,
																												0.0f,		sinf(time(0)),	cosf(time(0)),	0.0f,
																												0.0f,		0.0f,			0.0f,			1.0f,
			0.5f,	0.5f,	0.0f,		0.0f,	1.0f,	1.0f,	1.0f,		1.0f,	0.0f,		0.0f,			1.0f,		0.0f,			0.0f,			0.0f,
																												0.0f,		cosf(time(0)),	-sinf(time(0)),	0.0f,
																												0.0f,		sinf(time(0)),	cosf(time(0)),	0.0f,
																												0.0f,		0.0f,			0.0f,			1.0f,
			0.5f,	-0.5f,	0.0f,		0.0f,	1.0f,	1.0f,	1.0f,		1.0f,	1.0f,		0.0f,			1.0f,		0.0f,			0.0f,			0.0f,
																												0.0f,		cosf(time(0)),	-sinf(time(0)),	0.0f,
																												0.0f,		sinf(time(0)),	cosf(time(0)),	0.0f,
																												0.0f,		0.0f,			0.0f,			1.0f
		};
		DOData.vtxData.resize(vtxData.size() * sizeof(float));
		memcpy(&DOData.vtxData[0], &vtxData[0], DOData.vtxData.size());
		DOData.idxData = {
			0,	1,	2,
			2,	3,	0
		};
		DOData.unDrawOrder = 0;
		DOData.pGMtl = pGEngine->GetGResource<GLIB::GMaterial>("gmt_3d_batch");
		
		while (pGEngine->IsRunning()) {
			GLIB::GLayer* pLayer = pGEngine->GetLayer("gel_3d_batch");
			pLayer->DConfig.Blending.bEnable = true;
			pLayer->DConfig.Blending.FactorSrc = GLIB::BC_SRC_ALPHA;
			pLayer->DConfig.Blending.FactorDest = GLIB::BC_ONE_MINUS_SRC_ALPHA;
			pLayer->DConfig.DepthTest.bEnable = false;
			pLayer->AddDrawData(DOData);
			pGEngine->Update();
		}
		//pGEngine->Run();
		//pGEngine->GetRunThread().join();
	#endif
	} catch (std::exception Ex){
		NW_ERR(Ex.what());
	}

	return 0;
}