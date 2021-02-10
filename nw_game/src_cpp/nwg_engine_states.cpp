#include <nwg_pch.hpp>
#include "nwg_engine_states.h"

#include <nwg_gui_of.h>

#include <gfx/gfx_camera_lad.h>
#include <gfx/gfx_component.h>

using namespace NWL;
#pragma warning(disable : 4312)

namespace NWG
{
	GamerState::GamerState() :
		AEngineState("gamer_state"),
		m_rCore(CoreEngine::Get()) { }
	GamerState::~GamerState() { }

	// --==<core_methods>==--
	bool GamerState::Init() {
		return true;
	}
	void GamerState::OnQuit() {
	}
	void GamerState::Update() {
	}

	void GamerState::OnEvent(MouseEvent& rmEvt) { }
	void GamerState::OnEvent(KeyboardEvent& rkEvt) {
		switch (rkEvt.evType) {
		case ET_KEY_RELEASE:
			switch (rkEvt.unKeyCode) {
			case NW_KEY_N_78:
				for (UInt32 ei = 0; ei < 100; ei++) {
					m_eIds.push_back(EntSys::AddEnt());
					CmpSys::AddCmp<Gfx2dCmp>(m_eIds.back());
					CmpSys::AddCmp<TFormCmp>(m_eIds.back());
				}
				break;
			case NW_KEY_R_82:
				for (UInt32 ei = 0; ei < 100; ei++) {
					if (!m_eIds.empty()) {
						EntSys::RmvEnt(m_eIds.back());
						m_eIds.pop_back();
					}
				}
				default: break;
			case NWL::KC_K1:
				break;
			case NWL::KC_K2:
				break;
			}
			break;
		default: break;
		}
	}
	void GamerState::OnEvent(WindowEvent& rwEvt) {}
	// --==</core_methods>==--
}
namespace NWG
{
	GuiState::GuiState() :
		AEngineState("gui_state"),
		m_rCore(CoreEngine::Get()),
		m_bDockspace(true), m_bFullScreenPersist(true),
		m_pGuiContext(nullptr), m_pGuiIO(nullptr), m_pGuiStyle(nullptr) { }
	GuiState::~GuiState() { }

	// --==<core_methods>==--
	bool GuiState::Init() {
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();

			m_pGuiContext = ImGui::GetCurrentContext();
			m_pGuiIO = &ImGui::GetIO();
			m_pGuiStyle = &ImGui::GetStyle();
			
			m_pGuiIO->ConfigFlags |=
				ImGuiConfigFlags_NavEnableKeyboard |
				ImGuiConfigFlags_ViewportsEnable |
				ImGuiConfigFlags_DockingEnable;
			m_pGuiIO->ConfigDockingWithShift = true;
			ImGui::StyleColorsDark();
			if (m_pGuiIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				m_pGuiStyle->WindowRounding = 0.0f;
				m_pGuiStyle->Colors[ImGuiCol_WindowBg].w = 1.0f;
			}
			ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow*>(NW::CoreEngine::Get().GetWindow()->GetNative()), true);
			ImGui_ImplOpenGL3_Init("#version 430");
		}

		GuiOfCoreEngine::Get().bIsEnabled = true;
		GuiOfGraphEngine::Get().bIsEnabled = true;
		GuiOfDataSys::Get().bIsEnabled = true;

		return true;
	}
	void GuiState::OnQuit() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void GuiState::Update() {
		BeginDraw();
		{
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("view")) {
					ImGui::Checkbox("core_engine",			&GuiOfCoreEngine::Get().bIsEnabled);
					ImGui::Checkbox("graphichs_engine",		&GuiOfGraphEngine::Get().bIsEnabled);
					ImGui::Checkbox("console_engine",		&GuiOfCmdEngine::Get().bIsEnabled);
					ImGui::Checkbox("data_system",			&GuiOfDataSys::Get().bIsEnabled);
					ImGui::Checkbox("code_editor",			&GuiOfCodeEditor::Get().bIsEnabled);
					ImGui::Checkbox("sprite_editor",		&GuiOfSpriteEditor::Get().bIsEnabled);
					ImGui::Checkbox("gmaterial_editor",		&GuiOfGfxMaterialEditor::Get().bIsEnabled);

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			GuiOfCoreEngine::Get().OnDraw();
			GuiOfGraphEngine::Get().OnDraw();
			GuiOfCmdEngine::Get().OnDraw();
			GuiOfDataSys::Get().OnDraw();
			GuiOfCodeEditor::Get().OnDraw();
			GuiOfSpriteEditor::Get().OnDraw();
			GuiOfGfxMaterialEditor::Get().OnDraw();
		}
		EndDraw();
	}

	void GuiState::OnEvent(MouseEvent& rmEvt) { }
	void GuiState::OnEvent(KeyboardEvent& rkEvt) { }
	void GuiState::OnEvent(WindowEvent& rwEvt) { }
	// --==</core_methods>==--
	
	// --==<implementation_methods>==--
	inline void GuiState::BeginDraw() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Note: Switch this to true to enable dockspace
		bool bFullScreen = m_bFullScreenPersist;

		static ImGuiDockNodeFlags DockspaceFlags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		if (bFullScreen) {
			ImGuiViewport* Viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(Viewport->Pos);
			ImGui::SetNextWindowSize(Viewport->Size);
			ImGui::SetNextWindowViewport(Viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &m_bDockspace, WindowFlags);
		ImGui::PopStyleVar();

		if (bFullScreen) ImGui::PopStyleVar(2);

		// dock space
		float nMinSizeW = m_pGuiStyle->WindowMinSize.x;
		m_pGuiStyle->WindowMinSize.x = 100.0f;
		m_pGuiStyle->Alpha = 1.0f;
		if (m_pGuiIO->ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID nDockspaceId = ImGui::GetID("DockSpace");
			ImGui::DockSpace(nDockspaceId, ImVec2(0.0f, 0.0f), DockspaceFlags);
		}
		m_pGuiStyle->WindowMinSize.x = nMinSizeW;
	}
	inline void GuiState::EndDraw() {
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		ImGui::EndFrame();
		if (m_pGuiIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}
	// --==</implementation_methods>==--
}