#include <nwg_pch.hpp>
#include "nwg_creator_state.h"

#include <nwg_gui_of.h>
#include <guilib/nwg_gui_sys.h>

#pragma warning(disable : 4312)

namespace NWG
{
	using namespace NW;

	CreatorState::CreatorState()
	{
	}
	CreatorState::~CreatorState(){
		if (true) {	// materials
		}
		if (true) {	// textures
			delete (ADataRes::GetDataRes<ATexture2d>("spt_nw_hero"));
		}
		if (true) {	// shaders
		}
		GuiSys::OnQuit();
	}

	// --==<core_methods>==--
	bool CreatorState::Init() {
		GuiSys::OnInit();

		if (true) {	// shaders
		}
		if (true) {	// textures
		}
		if (true) {	// materials
		}

		GuiOfCoreEngine::Get().bIsEnabled = true;
		GuiOfGEngine::Get().bIsEnabled = true;
		GuiOfDataSys::Get().bIsEnabled = true;
		GuiOfTimeSys::Get().bIsEnabled = true;
		GuiOfMemSys::Get().bIsEnabled = true;

		return true;
	}
	void CreatorState::Update() {
		GuiSys::BeginDraw();
		GuiSys::Update();

		//GCameraLad::Get().UpdateCamera(&GEngine::Get().GetLayer()->Camera);

		if (true) {
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("view")) {
					ImGui::Checkbox("core_engine", &GuiOfCoreEngine::Get().bIsEnabled);
					ImGui::Checkbox("graphichs_engine", &GuiOfGEngine::Get().bIsEnabled);
					ImGui::Checkbox("console_engine", &GuiOfCmdEngine::Get().bIsEnabled);
					ImGui::Checkbox("data_system", &GuiOfDataSys::Get().bIsEnabled);
					ImGui::Checkbox("memory_mystem", &GuiOfMemSys::Get().bIsEnabled);
					ImGui::Checkbox("time_system", &GuiOfTimeSys::Get().bIsEnabled);
					ImGui::Checkbox("code_Editor", &GuiOfCodeEditor::Get().bIsEnabled);
					ImGui::Checkbox("sprite_editor", &GuiOfSpriteEditor::Get().bIsEnabled);
					ImGui::Checkbox("gmaterial_editor", &GuiOfGMaterialEditor::Get().bIsEnabled);

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			GuiOfCoreEngine::Get().OnDraw();
			GuiOfGEngine::Get().OnDraw();
			GuiOfCmdEngine::Get().OnDraw();
			GuiOfDataSys::Get().OnDraw();
			GuiOfMemSys::Get().OnDraw();
			GuiOfTimeSys::Get().OnDraw();
			GuiOfCodeEditor::Get().OnDraw();
			GuiOfSpriteEditor::Get().OnDraw();
			GuiOfGMaterialEditor::Get().OnDraw();
		}
		GuiSys::EndDraw();
	}
	void CreatorState::OnEnable() { }
	void CreatorState::OnDisable() { }
	// --==</core_methods>==--


	// --==<--on_event_methods>==--
	void CreatorState::OnEvent(MouseEvent& rmEvt)
	{
		//GCameraLad::Get().OnEvent(rmEvt, &GEngine::Get().GetLayer()->Camera);
	}
	void CreatorState::OnEvent(KeyboardEvent& rkEvt)
	{
		//GCameraLad::Get().OnEvent(rkEvt, &GEngine::Get().GetLayer()->Camera);
	}
	void CreatorState::OnEvent(WindowEvent& rwEvt)
	{
		//GCameraLad::Get().OnEvent(rwEvt, &GEngine::Get().GetLayer()->Camera);
	}
	// --==</--on_event_methods>==--
}