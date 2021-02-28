#include <nw_pch.hpp>
#include <core/nw_engine_states.h>

#include <core/nw_engine.h>
#include <core/nw_gui_of.h>

#include <data/data_mesh.h>

namespace NW
{
	AEngineState::AEngineState(CoreEngine& rEngine) :
		m_pEngine(&rEngine) { }
	AEngineState::~AEngineState() { }
}
namespace NW
{
	GamerState::GamerState(CoreEngine& rEngine) :
		AEngineState(rEngine) { }
	GamerState::~GamerState() { }

	// --==<core_methods>==--
	bool GamerState::Init() { return true; }
	void GamerState::Quit() { }
	void GamerState::Update() { }

	void GamerState::OnEvent(CursorEvent& rEvt) { }
	void GamerState::OnEvent(KeyboardEvent& rEvt) { }
	void GamerState::OnEvent(WindowEvent& rEvt) { }
	// --==</core_methods>==--
}
namespace NW
{
	static RefKeeper<ADrawCmp> s_drb;
	GfxState::GfxState(CoreEngine& rEngine) :
		AEngineState(rEngine),
		m_pGfx(m_pEngine->GetGfx()) { }
	GfxState::~GfxState() { }
	// --==<core_methods>==--
	bool GfxState::Init() {
		m_pGfx = m_pEngine->GetGfx();
		
		Vtx2f2f vtxData[] = {
			{ { -0.5f,	-0.5f },	{ -0.0f,	-0.0f } },
			{ { -0.5f,	+0.5f },	{ -0.0f,	+1.0f } },
			{ { +0.5f,	+0.5f },	{ +1.0f,	+1.0f } },
			{ { +0.5f,	-0.5f },	{ +1.0f,	-0.0f } },
		};
		UInt32 idxData[] = {
			0, 1, 2,
			2, 3, 0,
		};

		m_pGfx->NewRes<ADrawCmp, DrawIdxCmp>(s_drb,
			sizeof(idxData) / sizeof(UInt32), &idxData[0],
			sizeof(vtxData) / sizeof(Vtx2f2f), &vtxData[0]
			);

		RefKeeper<GfxMaterial> gMtl;
		m_pGfx->NewRes<GfxMaterial>(gMtl, "gmt_2d_default");
		m_pGfx->NewRes<Texture, Texture2d>(gMtl->GetTexture(), "tex_nw_logo");
		if (!gMtl->GetTexture()->LoadF(R"(D:\dev\native_world\data\image\nw_logo.bmp)")) { return false; }
		m_pGfx->NewRes<ShaderProg>(gMtl->GetShaderProg(), "shp_2d_default");
		if (!gMtl->GetShaderProg()->LoadF(R"(D:\dev\native_world\data\shader\shp_2d_default.shd)")) { return false; }
		s_drb->AddResource<GfxMaterial>(gMtl);

		GfxCameraLad::Get().SetCursor(m_pEngine->GetCursor());
		GfxCameraLad::Get().SetKeyboard(m_pEngine->GetKeyboard());

		return true;
	}
	void GfxState::Quit()
	{
		s_drb.Reset();

		GfxCameraLad::Get().SetCursor(nullptr);
		GfxCameraLad::Get().SetKeyboard(nullptr);
	}
	void GfxState::Update()
	{
		GfxCameraLad::Get().UpdateCamera();
		DrawScene();
	}

	void GfxState::OnEvent(CursorEvent& rEvt) { GfxCameraLad::Get().OnEvent(rEvt); }
	void GfxState::OnEvent(KeyboardEvent& rEvt) { GfxCameraLad::Get().OnEvent(rEvt); }
	void GfxState::OnEvent(WindowEvent& rEvt) {
		GfxCameraLad::Get().OnEvent(rEvt);
		switch (rEvt.evType) {
		case EVT_WINDOW_RESIZE: {
			V4i rectViewport = m_pGfx->GetConfigs().General.rectViewport;
			rectViewport[2] = rEvt.nX;
			rectViewport[3] = rEvt.nY;
			m_pGfx->SetViewport(rectViewport[0], rectViewport[1], rectViewport[2], rectViewport[3]);
			break;
		}
		case EVT_WINDOW_MOVE: { break; }
		default: break;
		}
	}
	// --==</core_methods>==--

	// --==<implementation_methods>==--
	inline void GfxState::DrawScene() {
		m_pGfx->BeginDraw();
		m_pGfx->OnDraw(s_drb);
		m_pGfx->EndDraw();
	}
	// --==</implementation_methods>==--
}
namespace NW
{
	GuiState::GuiState(CoreEngine& rEngine) :
		AEngineState(rEngine),
		m_bDockspace(true), m_bFullScreenPersist(true),
		m_pGuiContext(nullptr), m_pGuiIO(nullptr), m_pGuiStyle(nullptr) { }
	GuiState::~GuiState() { }
	// --==<core_methods>==--
	bool GuiState::Init()
	{
		IMGUI_CHECKVERSION();
		GUI::CreateContext();

		m_pGuiContext = GUI::GetCurrentContext();
		m_pGuiIO = &GUI::GetIO();
		m_pGuiStyle = &GUI::GetStyle();

		m_pGuiIO->ConfigFlags |=
			ImGuiConfigFlags_NavEnableKeyboard |
			ImGuiConfigFlags_ViewportsEnable |
			ImGuiConfigFlags_DockingEnable;
		m_pGuiIO->ConfigDockingWithShift = true;
		GUI::StyleColorsDark();
		if (m_pGuiIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			m_pGuiStyle->WindowRounding = 0.0f;
			m_pGuiStyle->Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
#if(defined NW_PLATFORM_WINDOWS)
		GUI::Win32Init(m_pEngine->GetWindow()->GetNative());
#endif
#if (NWG_GAPI & NWG_GAPI_OGL)
		GUI::OglInit("#version 130");
#endif
#if (NWG_GAPI & NWG_GAPI_DX)
		GUI::Dx11Init(m_rEngine.GetGfx()->GetDevice(), m_rEngine.GetGfx()->GetContext());
#endif
		GuiOfCoreEngine::Get().bIsEnabled = true;
		GuiOfGfxEngine::Get().bIsEnabled = true;
		GuiOfDataSys::Get().bIsEnabled = true;

		return true;
	}
	void GuiState::Quit()
	{
#if (NWG_GAPI & NWG_GAPI_OGL)
		GUI::OglShutdown();
#endif
#if (NWG_GAPI & NWG_GAPI_DX)
		GUI::Dx11Shutdown();
#endif
#if(defined NW_PLATFORM_WINDOWS)
		GUI::Win32Shutdown();
#endif
		GUI::DestroyContext();
	}
	void GuiState::Update() {
		BeginDraw();
		if (GUI::BeginMenuBar()) {
			if (GUI::BeginMenu("view")) {
				GUI::Checkbox("core_engine", &GuiOfCoreEngine::Get().bIsEnabled);
				GUI::Checkbox("graphichs_engine", &GuiOfGfxEngine::Get().bIsEnabled);
				GUI::Checkbox("console_engine", &GuiOfCmdEngine::Get().bIsEnabled);
				GUI::Checkbox("data_system", &GuiOfDataSys::Get().bIsEnabled);
				GUI::Checkbox("shader_editor", &GuiOfShaderEditor::Get().bIsEnabled);
				GUI::Checkbox("sprite_editor", &GuiOfSpriteEditor::Get().bIsEnabled);
				GUI::Checkbox("graphics_material_editor", &GuiOfGfxMaterialEditor::Get().bIsEnabled);

				GUI::EndMenu();
		}
			GUI::EndMenuBar();
	}

		GuiOfCoreEngine::Get().OnDraw();
		GuiOfGfxEngine::Get().OnDraw();
		GuiOfCmdEngine::Get().OnDraw();
		GuiOfDataSys::Get().OnDraw();
		GuiOfShaderEditor::Get().OnDraw();
		GuiOfSpriteEditor::Get().OnDraw();
		GuiOfGfxMaterialEditor::Get().OnDraw();
		EndDraw();
}

	void GuiState::OnEvent(CursorEvent& rmEvt) { }
	void GuiState::OnEvent(KeyboardEvent& rkEvt) { }
	void GuiState::OnEvent(WindowEvent& rwEvt) { }
	// --==</core_methods>==--

	// --==<implementation_methods>==--
	inline void GuiState::BeginDraw() {
#if (NWG_GAPI & NWG_GAPI_OGL)
		GUI::OglNewFrame();
#endif
#if (NWG_GAPI & NWG_GAPI_DX)
		GUI::Dx11NewFrame();
#endif
#if (defined NW_PLATFORM_WINDOWS)
		GUI::Win32NewFrame();
#endif
		GUI::NewFrame();
		// note: switch this to true to enable dockspace
		bool bFullScreen = m_bFullScreenPersist;
		static ImGuiDockNodeFlags DockspaceFlags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		if (bFullScreen) {
			ImGuiViewport* Viewport = GUI::GetMainViewport();
			GUI::SetNextWindowPos(Viewport->Pos);
			GUI::SetNextWindowSize(Viewport->Size);
			GUI::SetNextWindowViewport(Viewport->ID);
			GUI::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			GUI::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		GUI::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		GUI::Begin("DockSpace", &m_bDockspace, WindowFlags);
		GUI::PopStyleVar();

		if (bFullScreen) { GUI::PopStyleVar(2); }

		// dock space
		float nMinSizeW = m_pGuiStyle->WindowMinSize.x;
		m_pGuiStyle->WindowMinSize.x = 100.0f;
		m_pGuiStyle->Alpha = 1.0f;
		if (m_pGuiIO->ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID nDockspaceId = GUI::GetID("DockSpace");
			GUI::DockSpace(nDockspaceId, ImVec2(0.0f, 0.0f), DockspaceFlags);
		}
		m_pGuiStyle->WindowMinSize.x = nMinSizeW;
	}
	inline void GuiState::EndDraw() {
		GUI::End();
		GUI::Render();
#if (NWG_GAPI & NWG_GAPI_OGL)
		GUI::OglRenderDrawData(GUI::GetDrawData());
#endif
#if (NWG_GAPI & NWG_GAPI_DX)
		GUI::Dx11RenderDrawData(GUI::GetDrawData());
#endif
		GUI::EndFrame();
		if (m_pGuiIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
#if (defined NW_PLATFORM_WINDOWS)
			GUI::UpdatePlatformWindows();
			GUI::RenderPlatformWindowsDefault();
#endif
		}
	}
	// --==</implementation_methods>==--
}