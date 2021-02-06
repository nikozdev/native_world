#include <nwg_pch.hpp>
#include "nwg_engine_states.h"

#include <nwg_gui_of.h>
#include <nwg_gcamera_lad.h>

#include <glib_component.h>

using namespace NWL;
#pragma warning(disable : 4312)

namespace NWG
{
	GraphState::GraphState() :
		AEngineState("graphics_state"),
		m_rGraph(GraphEngine::Get()),
		m_gCamera(GCamera()),
		m_pShdBuf() {}
	GraphState::~GraphState() { }

	// --==<core_methods>==--
	bool GraphState::Init() {
		ShaderBuf::Create(m_pShdBuf);
		m_pShdBuf->SetData(1 << 12, nullptr);

		{	// shaders
			Shader* pShader = nullptr;
			
			pShader = Shader::Create("shd_3d_default");
			if (!pShader->LoadF("D:/dev/native_world/nw_glib/src_glsl/shd_3d_default.glsl")) { return false; }

			pShader = Shader::Create("shd_3d_batch");
			if (!pShader->LoadF("D:/dev/native_world/nw_glib/src_glsl/shd_3d_batch.glsl")) { return false; }
			m_pShdBuf->Remake(pShader->GetShdLayout());
		}
		{	// textures
			Texture* pTex = nullptr;
			pTex = Texture::Create("tex_nw_bat", TXT_2D);
			pTex->LoadF(R"(D:\dev\native_world\nw_glib\data\ico\nw_bat.png)");
		}
		{	// materials
			GMaterial* pgMtl = GMaterial::Create("gmt_3d_default");
			pgMtl->SetShader(TDataRes<Shader>::GetDataRes("shd_3d_default"));
			pgMtl = GMaterial::Create("gmt_3d_batch");
			pgMtl->SetShader(TDataRes<Shader>::GetDataRes("shd_3d_batch"));
			pgMtl->SetTexture(TDataRes<Texture>::GetDataRes("tex_nw_bat"), "unf_tex[0]");
		}
		{	// framebuffers
		}

		return true;
	}
	void GraphState::OnQuit() {
		m_pShdBuf.Reset();
	}
	void GraphState::Update()
	{
		GCameraLad::Get().whBounds.x = GraphEngine::Get().GetFrameBuf()->GetWidth();
		GCameraLad::Get().whBounds.y = GraphEngine::Get().GetFrameBuf()->GetHeight();
		GCameraLad::Get().UpdateCamera(&m_gCamera);
		
		DrawScene();
	}

	void GraphState::OnEvent(MouseEvent& rmEvt)
	{
		GCameraLad::Get().OnEvent(rmEvt, &m_gCamera);
	}
	void GraphState::OnEvent(KeyboardEvent& rkEvt)
	{
		GCameraLad::Get().OnEvent(rkEvt, &m_gCamera);
	}
	void GraphState::OnEvent(WindowEvent& rwEvt)
	{
		GCameraLad::Get().OnEvent(rwEvt, &m_gCamera);
	}
	// --==</core_methods>==--

	// --==<implementation_methods>==--
	inline void GraphState::DrawScene() {
		{
			FrameBuf* pfmBuf = GraphEngine::Get().GetFrameBuf();
			pfmBuf->Bind();
			pfmBuf->Clear();
			m_pShdBuf->Bind();

			m_rGraph.SetModes(true, PM_BLEND);
			//m_rGraph.SetModes(true, PM_DEPTH_TEST);
			m_rGraph.SetBlendFunc(BC_SRC_ALPHA, BC_ONE_MINUS_SRC_ALPHA);

			Mat4f mat4Proj = m_gCamera.GetProjMatrix();
			Mat4f mat4View = m_gCamera.GetViewMatrix();
			m_pShdBuf->SetSubData(sizeof(Mat4f), &(mat4Proj[0][0]), 0 * sizeof(Mat4f));
			m_pShdBuf->SetSubData(sizeof(Mat4f), &(mat4View[0][0]), 1 * sizeof(Mat4f));

			auto& rg2dCmps = CmpSys::GetCmps<Graphics2dCmp>();
			for (auto& itCmp : rg2dCmps) {
				if (TransformCmp* tfCmp = CmpSys::GetCmp<TransformCmp>(itCmp->GetEntId())) {
					auto vtxData = MakeVtxRect(tfCmp->GetMatrix());
					BufferData vbData{ &vtxData[0], vtxData.size() * sizeof(Float32), 0 };
					itCmp->m_drw.UploadVtxData(&vbData);
				}
				m_rGraph.OnDraw(itCmp->m_drw);
			}
			m_pShdBuf->Unbind();
			pfmBuf->Unbind();
		}
	}
	// --==</implementation_methods>==--
}
namespace NWG
{
	GamerState::GamerState() :
		AEngineState("game_state"),
		m_rCore(CoreEngine::Get()) { }
	GamerState::~GamerState() { }

	// --==<core_methods>==--
	bool GamerState::Init() {
		EntSys::OnInit();
		CmpSys::OnInit();
		return true;
	}
	void GamerState::OnQuit() {
		EntSys::OnQuit();
		CmpSys::OnQuit();
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
					CmpSys::AddCmp<Graphics2dCmp>(m_eIds.back());
					CmpSys::AddCmp<TransformCmp>(m_eIds.back());
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
			case NW_KEY_1_49:
				break;
			case NW_KEY_2_50:
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
			ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(NW::CoreEngine::Get().GetWindow()->GetNative()), true);
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
					ImGui::Checkbox("gmaterial_editor",		&GuiOfGMaterialEditor::Get().bIsEnabled);

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
			GuiOfGMaterialEditor::Get().OnDraw();
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