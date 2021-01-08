#include <nw_pch.hpp>
#include "nw_core_state.h"

#include <core/nw_core_engine.h>
#include <ecs/nw_scene.h>
#include <lua/nw_lua_vm.h>

#include <gui/nw_gui_core.h>
#include <gui/nw_gui_widget.h>

#include <gl/control/nw_draw_engine.h>
#include <gl/control/nw_gcamera_lad.h>

#include <sys/nw_gui_sys.h>
#include <sys/nw_log_sys.h>

namespace NW
{
	CoreState::CoreState() :
		m_bIsEnabled(false) { }
	CoreState::~CoreState() {}

	// --==<core_methods>==--
	bool CoreState::Init()
	{
		return true;
	}
	void CoreState::Update()
	{
		GCameraLad::Get().UpdateCamera();
		Scene::Get().Update();
	}
	void CoreState::OnEnable()
	{
		if (m_bIsEnabled) { return; }
		m_bIsEnabled = true;
		Scene::Get().SetGCamera(GCameraLad::Get().GetGCamera());
	}
	void CoreState::OnDisable()
	{
		if (!m_bIsEnabled) { return; }
		m_bIsEnabled = false;
		Scene::Get().SetGCamera(nullptr);
	}
	// --==</core_methods>==--

	// --==<--on_event_methods>==--
	void CoreState::OnEvent(MouseEvent& rmEvt)
	{
		GCameraLad::Get().OnEvent(rmEvt);
	}
	void CoreState::OnEvent(KeyboardEvent& rkEvt)
	{
		GCameraLad::Get().OnEvent(rkEvt);
	}
	void CoreState::OnEvent(WindowEvent& rwEvt)
	{
		GCameraLad::Get().OnEvent(rwEvt);
	}
	// --==</--on_event_methods>==--
}