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
			delete ADataRes::GetDataRes<ATexture2d>("spt_nw_hero");
		}
		if (true) {	// shaders
		}
	}

	// --==<core_methods>==--
	bool GamerState::Init() {
		if (true) {	// shaders
		}
		if (true) {	// textures
			ATexture2d* pTex = ATexture2d::Create("spt_nw_hero");
			pTex->LoadF("data/tex/spt_nw_hero.png");
		}
		if (true) {	// materials
		}

		return true;
	}
	void GamerState::Update() { }
	void GamerState::OnEnable() { }
	void GamerState::OnDisable() { }
	// --==</core_methods>==--


	// --==<--on_event_methods>==--
	void GamerState::OnEvent(MouseEvent& rmEvt)
	{
	}
	void GamerState::OnEvent(KeyboardEvent& rkEvt)
	{
	}
	void GamerState::OnEvent(WindowEvent& rwEvt)
	{
	}
	// --==</--on_event_methods>==--
}