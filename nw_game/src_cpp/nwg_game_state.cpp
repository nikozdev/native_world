#include <nwg_pch.hpp>
#include "nwg_game_state.h"

#include <glib/control/nw_gcamera_lad.h>

namespace NWG
{
	using namespace NW;

	GameState::GameState(){}
	GameState::~GameState(){
		if (true) {	// materials
		}
		if (true) {	// textures
			MemSys::DelT<ATexture2d>(DataSys::GetDataRes<ATexture2d>("spt_nw_hero"));
		}
		if (true) {	// shaders
		}
	}

	// --==<core_methods>==--
	bool GameState::Init() {
		if (true) {	// shaders
		}
		if (true) {	// textures
			ATexture2d::Create("spt_nw_hero")->LoadF("data_res/graphics/images/spt_nw_hero.png");
		}
		if (true) {	// materials
		}
		return true;
	}
	void GameState::Update() {
		//auto& rEnts = Scene::Get().GetEnts();
		
		GCameraLad::Get().UpdateCamera();
		Scene::Get().Update();
	}
	void GameState::OnEnable() { Scene::Get().SetGCamera(GCameraLad::Get().GetGCamera()); }
	void GameState::OnDisable() { Scene::Get().SetGCamera(nullptr); }
	// --==</core_methods>==--


	// --==<--on_event_methods>==--
	void GameState::OnEvent(MouseEvent& rmEvt)
	{
		GCameraLad::Get().OnEvent(rmEvt);
	}
	void GameState::OnEvent(KeyboardEvent& rkEvt)
	{
		GCameraLad::Get().OnEvent(rkEvt);
	}
	void GameState::OnEvent(WindowEvent& rwEvt)
	{
		GCameraLad::Get().OnEvent(rwEvt);
	}
	// --==</--on_event_methods>==--
}