#include <nwg_pch.hpp>
#include "nwg_game_state.h"

#include <glib/control/nw_gcamera_lad.h>

namespace NWG
{
	using namespace NW;

	GameState::GameState(){
		if (true) {
		}
		if (true) {
		}
		if (true) {
		}
	}
	GameState::~GameState(){
		if (true) {
		}
		if (true) {
		}
		if (true) {
		}
	}

	// --==<core_methods>==--
	bool GameState::Init() { return true; }
	void GameState::Update() { GCameraLad::Get().UpdateCamera(); Scene::Get().Update(); }
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