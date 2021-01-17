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
			MemSys::DelT<ATexture2d>(DataSys::GetDataRes<ATexture2d>("spt_nw_hero"));
		}
		if (true) {	// shaders
		}
		if (true) {	// entitites
			auto& rEnts = DataSys::GetDataResources<AEntity>();
			while (!rEnts.empty()) { MemSys::DelT<AEntity>(rEnts.begin()->second); rEnts.erase(rEnts.begin()); }
		}
	}

	// --==<core_methods>==--
	bool CreatorState::Init() {
		if (true) {	// shaders
		}
		if (true) {	// textures
			ATexture2d::Create("spt_nw_hero")->LoadF("data/tex/spt_nw_hero.png");
		}
		if (true) {	// materials
		}

		GuiOfDataSys::Get().bIsEnabled = true;
		GuiOfTimeSys::Get().bIsEnabled = true;
		GuiOfGEngine::Get().bIsEnabled = true;
		GuiOfCoreEngine::Get().bIsEnabled = true;

		return true;
	}
	void CreatorState::Update() {
		GCameraLad::Get().UpdateCamera(GEngine::Get().GetLayer()->GetGCamera());

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
					ImGui::Checkbox("scene_editor", &GuiOfSceneEditor::Get().bIsEnabled);
					ImGui::Checkbox("entity_editor", &GuiOfEntityEditor::Get().bIsEnabled);
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
			GuiOfSceneEditor::Get().OnDraw();
			GuiOfEntityEditor::Get().OnDraw();
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
		GCameraLad::Get().OnEvent(rmEvt, GEngine::Get().GetLayer()->GetGCamera());
	}
	void CreatorState::OnEvent(KeyboardEvent& rkEvt)
	{
		GCameraLad::Get().OnEvent(rkEvt, GEngine::Get().GetLayer()->GetGCamera());
	}
	void CreatorState::OnEvent(WindowEvent& rwEvt)
	{
		GCameraLad::Get().OnEvent(rwEvt, GEngine::Get().GetLayer()->GetGCamera());
	}
	// --==</--on_event_methods>==--
}