#include <nwg_pch.hpp>
#include "nwg_gamer_state.h"

namespace NWG
{
	using namespace NW;

	GamerState::GamerState() { }
	GamerState::~GamerState() {
		if (true) {	// materials
		}
		if (true) {	// textures
			MemSys::DelT<ATexture2d>(DataSys::GetDataRes<ATexture2d>("spt_nw_hero"));
		}
		if (true) {	// shaders
		}
	}

	// --==<core_methods>==--
	bool GamerState::Init() {
		if (true) {	// shaders
		}
		if (true) {	// textures
			ATexture2d::Create("spt_nw_hero")->LoadF("data/tex/spt_nw_hero.png");
		}
		if (true) {	// materials
		}

		Scene::Get();

		return true;
	}
	void GamerState::Update() {
		Scene::Get().Update();
	}
	void GamerState::OnEnable() { }
	void GamerState::OnDisable() { }
	// --==</core_methods>==--


	// --==<--on_event_methods>==--
	void GamerState::OnEvent(MouseEvent& rmEvt)
	{
		GCameraLad::Get().OnEvent(rmEvt);
	}
	void GamerState::OnEvent(KeyboardEvent& rkEvt)
	{
		GCameraLad::Get().OnEvent(rkEvt);
	}
	void GamerState::OnEvent(WindowEvent& rwEvt)
	{
		GCameraLad::Get().OnEvent(rwEvt);
	}
	// --==</--on_event_methods>==--
}