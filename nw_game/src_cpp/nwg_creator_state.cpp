#include <nwg_pch.hpp>
#include "nwg_creator_state.h"

#include <nwg_gui_of.h>

#pragma warning(disable : 4312)

namespace NWG
{
	using namespace NW;

	CreatorState::CreatorState() { }
	CreatorState::~CreatorState(){
		if (true) {	// materials
		}
		if (true) {	// textures
			delete (ADataRes::GetDataRes<ATexture2d>("spt_nw_hero"));
		}
		if (true) {	// shaders
		}
	}

	// --==<core_methods>==--
	bool CreatorState::Init() {
		if (true) {	// shaders
			GEngine::Get().GetLayer()->SetShader(ADataRes::GetDataRes<AShader>("shd_3d_batch"));
		}
		if (true) {	// textures
			ATexture2d::Create("spt_nw_hero")->LoadF("data/tex/spt_nw_hero.png");
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
		GCameraLad::Get().UpdateCamera(&GEngine::Get().GetLayer()->Camera);

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
#if false
					ImGui::Checkbox("scene_editor", &GuiOfSceneEditor::Get().bIsEnabled);
					ImGui::Checkbox("entity_editor", &GuiOfEntityEditor::Get().bIsEnabled);
#endif
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
#if false
			GuiOfSceneEditor::Get().OnDraw();
			GuiOfEntityEditor::Get().OnDraw();
#endif
			GuiOfSpriteEditor::Get().OnDraw();
			GuiOfGMaterialEditor::Get().OnDraw();
		}
	}
	void CreatorState::OnEnable() { }
	void CreatorState::OnDisable() { }
	// --==</core_methods>==--


	// --==<--on_event_methods>==--
	void CreatorState::OnEvent(MouseEvent& rmEvt)
	{
		GCameraLad::Get().OnEvent(rmEvt, &GEngine::Get().GetLayer()->Camera);
	}
	void CreatorState::OnEvent(KeyboardEvent& rkEvt)
	{
		GCameraLad::Get().OnEvent(rkEvt, &GEngine::Get().GetLayer()->Camera);
	}
	void CreatorState::OnEvent(WindowEvent& rwEvt)
	{
		GCameraLad::Get().OnEvent(rwEvt, &GEngine::Get().GetLayer()->Camera);
	}
	// --==</--on_event_methods>==--
}