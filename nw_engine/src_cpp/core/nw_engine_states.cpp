#include <nw_pch.hpp>
#include <core/nw_engine_states.h>

#include <core/nw_engine.h>

namespace NW
{
	GamerState::GamerState() :
		AEngineState("gamer_state"),
		m_rEngine(CoreEngine::Get()) { }
	GamerState::~GamerState() { }

	// --==<core_methods>==--
	bool GamerState::Init()
	{
#if false
		Vtx3f2f3f vtxData[] = {
			{ { -0.5f,	-0.5f,	0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
			{ { -0.5f,	0.5f,	0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f } },
			{ { 0.5f,	0.5f,	0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f } },
			{ { 0.5f,	-0.5f,	0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
		};
#else
		Vtx2f2f vtxData[] = {
			{ { -0.5f,	-0.5f }, { 0.0f, 0.0f } },
			{ { -0.5f,	0.5f }, { 0.0f, 1.0f } },
			{ { 0.5f,	0.5f }, { 1.0f, 1.0f } },
			{ { 0.5f,	-0.5f }, { 1.0f, 0.0f } },
		};
#endif
		UInt32 idxData[] = {
			0, 1, 2,
			2, 3, 0,
		};

		m_rEngine.GetGfx()->CreateRes<Drawable, IdxDrawable>(m_pDrb,
			sizeof(idxData) / sizeof(UInt32), &idxData[0],
			sizeof(vtxData) / sizeof(Vtx2f2f), &vtxData[0]);

		RefKeeper<GfxMaterial> gMtl;
		m_rEngine.GetGfx()->CreateRes<GfxMaterial>(gMtl, "gmt_2d_default");
		m_rEngine.GetGfx()->CreateRes<Texture, Texture2d>(gMtl->GetTexture(), "tex_solid_white");
		m_rEngine.GetGfx()->CreateRes<ShaderProg>(gMtl->GetShaderProg(), "shp_2d_default");
		if (!gMtl->GetShaderProg()->LoadF(R"(D:\dev\native_world\data\shader\shp_2d_default.shd)")) { return false; }
		m_pDrb->AddResource(gMtl);

		return true;
	}
	void GamerState::Quit()
	{
		m_pDrb.Reset();
	}
	void GamerState::Update()
	{
		GfxCameraLad::Get().UpdateCamera();

		m_rEngine.GetGfx()->GetConfigs().General.unSwapInterval = 0;
		m_rEngine.GetGfx()->BeginDraw();
		m_rEngine.GetGfx()->OnDraw(m_pDrb);
		m_rEngine.GetGfx()->EndDraw();
	}

	void GamerState::OnEvent(CursorEvent& rEvt) {
		GfxCameraLad::Get().OnEvent(rEvt);
	}
	void GamerState::OnEvent(KeyboardEvent& rEvt) {
		GfxCameraLad::Get().OnEvent(rEvt);
	}
	void GamerState::OnEvent(WindowEvent& rEvt) {
		GfxCameraLad::Get().OnEvent(rEvt);
		switch (rEvt.evType) {
		case EVT_WINDOW_RESIZE: {
			V4i rectViewport = m_rEngine.GetGfx()->GetConfigs().General.rectViewport;
			rectViewport[2] = rEvt.nX;
			rectViewport[3] = rEvt.nY;
			m_rEngine.GetGfx()->SetViewport(rectViewport[0], rectViewport[1], rectViewport[2], rectViewport[3]);
			break;
		}
		case EVT_WINDOW_MOVE: { break; }
		default: break;
		}
	}
	// --==</core_methods>==--
}
namespace NW
{
	GuiState::GuiState() :
		AEngineState("gui_state"),
		m_rEngine(CoreEngine::Get()),
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
#if(defined NW_PLATFORM_WINDOWS)
			ImGui_ImplWin32_Init(m_rEngine.GetWindow()->GetNative());
#endif
#if (NWG_GAPI & NWG_GAPI_OGL)
			ImGui_ImplOpenGL3_Init("#version 430");
#endif
#if (NWG_GAPI & NWG_GAPI_DX)
			ImGui_ImplDX11_Init(m_rEngine.GetGfx()->GetDevice(), m_rEngine.GetGfx()->GetContext());
#endif
		}

		GuiOfCoreEngine::Get().bIsEnabled = true;
		GuiOfGfxEngine::Get().bIsEnabled = true;
		GuiOfDataSys::Get().bIsEnabled = true;

		return true;
	}
	void GuiState::Quit() {
#if (NWG_GAPI & NWG_GAPI_OGL)
		ImGui_ImplOpenGL3_Shutdown();
#endif
#if (NWG_GAPI & NWG_GAPI_DX)
		ImGui_ImplDX11_Shutdown();
#endif
#if(defined NW_PLATFORM_WINDOWS)
		ImGui_ImplWin32_Shutdown();
#endif
		ImGui::DestroyContext();
	}
	void GuiState::Update() {
		BeginDraw();
		{
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("view")) {
					ImGui::Checkbox("core_engine",			&GuiOfCoreEngine::Get().bIsEnabled);
					ImGui::Checkbox("graphichs_engine",		&GuiOfGfxEngine::Get().bIsEnabled);
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
			GuiOfGfxEngine::Get().OnDraw();
			GuiOfCmdEngine::Get().OnDraw();
			GuiOfDataSys::Get().OnDraw();
			GuiOfCodeEditor::Get().OnDraw();
			GuiOfSpriteEditor::Get().OnDraw();
			GuiOfGfxMaterialEditor::Get().OnDraw();
		}
		EndDraw();
	}

	void GuiState::OnEvent(CursorEvent& rmEvt) { }
	void GuiState::OnEvent(KeyboardEvent& rkEvt) { }
	void GuiState::OnEvent(WindowEvent& rwEvt) { }
	// --==</core_methods>==--
	
	// --==<implementation_methods>==--
	inline void GuiState::BeginDraw() {
#if (NWG_GAPI & NWG_GAPI_OGL)
		ImGui_ImplOpenGL3_NewFrame();
#endif
#if (NWG_GAPI & NWG_GAPI_DX)
		ImGui_ImplDX11_NewFrame();
#endif
#if (defined NW_PLATFORM_WINDOWS)
		ImGui_ImplWin32_NewFrame();
#endif
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
#if (NWG_GAPI & NWG_GAPI_OGL)
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
#if (NWG_GAPI & NWG_GAPI_DX)
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
		ImGui::EndFrame();
		if (m_pGuiIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
#if (defined NW_PLATFORM_WINDOWS)
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
#endif
		}
	}
	// --==</implementation_methods>==--
}
namespace NW
{
	GfxState::GfxState() :
		AEngineState("graphics_state"),
		m_rEngine(CoreEngine::Get()) { }
	GfxState::~GfxState() { }

	// --==<core_methods>==--
	bool GfxState::Init() {

		return true;
	}
	void GfxState::Quit() {
	}
	void GfxState::Update()
	{
		GfxCameraLad::Get().UpdateCamera();
		
		DrawScene();
	}

	void GfxState::OnEvent(CursorEvent& rmEvt)
	{
		GfxCameraLad::Get().OnEvent(rmEvt);
	}
	void GfxState::OnEvent(KeyboardEvent& rkEvt)
	{
		GfxCameraLad::Get().OnEvent(rkEvt);
	}
	void GfxState::OnEvent(WindowEvent& rwEvt)
	{
		GfxCameraLad::Get().OnEvent(rwEvt);
	}
	// --==</core_methods>==--

	// --==<implementation_methods>==--
	inline void GfxState::DrawScene() {
	}
	// --==</implementation_methods>==--
}