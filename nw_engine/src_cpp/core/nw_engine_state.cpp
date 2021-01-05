#include <nw_pch.hpp>
#include "nw_engine_state.h"

#include <core/nw_engine.h>
#include <ecs/nw_scene.h>
#include <lua/nw_lua_vm.h>
#include <lua/nw_lua_core.h>

#include <gui/nw_gui_core.h>
#include <gui/nw_gui_widget.h>

#include <gl/control/nw_drawer.h>
#include <gl/control/nw_gcamera_lad.h>

#include <sys/nw_gui_sys.h>
#include <sys/nw_log_sys.h>

namespace NW
{
	EngineState::EngineState() :
		m_bIsEnabled(false) { }
	EngineState::~EngineState() {}

	// ========<Core Methods>========
	bool EngineState::Init()
	{
//		if (!LuaVM::Get().RunScriptF(R"(D:\dev\CheerNik\NW_Engine\src_lua\estates\est_default.lua)")) {
//			return false;
//		}
//		if (!LuaVM::Get().CallFunctionGlb("init")) {
//			return false;
//		}
		return true;
	}
	void EngineState::Update()
	{

		//if (!LuaVM::Get().CallFunctionGlb("update")) {
		//	Engine::Get().Quit();
		//	return;
		//}

		GCameraLad::Get().UpdateCamera();
		Scene::Get().Update();
	}
	void EngineState::OnEnable()
	{
		if (m_bIsEnabled) { return; }
		m_bIsEnabled = true;
		Scene::Get().SetGCamera(GCameraLad::Get().GetGCamera());
		//if (!LuaVM::Get().CallFunctionGlb("on_enable")) {
		//	Engine::Get().Quit();
		//	return;
		//}
	}
	void EngineState::OnDisable()
	{
		if (!m_bIsEnabled) { return; }
		m_bIsEnabled = false;
		Scene::Get().SetGCamera(nullptr);
		//if (!LuaVM::Get().CallFunctionGlb("on_disable")) {
		//	Engine::Get().Quit();
		//	return;
		//}
	}
	// ========</Core Methods>========

	// ========<OnEvent Methods>========
	void EngineState::OnEvent(MouseEvent& rmEvt)
	{
		GCameraLad::Get().OnEvent(rmEvt);
	}
	void EngineState::OnEvent(KeyboardEvent& rkEvt)
	{
		GCameraLad::Get().OnEvent(rkEvt);
	}
	void EngineState::OnEvent(WindowEvent& rwEvt)
	{
		GCameraLad::Get().OnEvent(rwEvt);
	}
	// ========</OnEvent Methods>========
}