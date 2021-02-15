#include <nw_pch.hpp>
#include <core/nw_engine_states.h>

#include <core/nw_engine.h>

#include <gfx/gfx_buffer.h>
#include <gfx/gfx_framebuf.h>
#include <gfx/gfx_shader.h>
#include <gfx/gfx_material.h>
#include <gfx/gfx_texture.h>
#include <gfx/gfx_camera_lad.h>
#include <gfx/gfx_component.h>

namespace NW
{
	GamerState::GamerState() :
		AEngineState("gamer_state"),
		m_rEngine(CoreEngine::Get()) { }
	GamerState::~GamerState() { }

	// --==<core_methods>==--
	bool GamerState::Init()
	{
		if (!DataSys::LoadFImageBmp("..\\data\\image\\img_home_0.bmp", m_imgInfo)) { return false; }

		return true;
	}
	void GamerState::Quit()
	{
		if (m_imgInfo.pClrData != nullptr) { DelTArr<UByte>(m_imgInfo.pClrData, m_imgInfo.GetDataSize()); }
	}
	void GamerState::Update()
	{
		V2f whWndSize = { static_cast<Float32>(m_rEngine.GetWindow()->GetSizeW()), static_cast<Float32>(m_rEngine.GetWindow()->GetSizeH()) };
		glViewport( 0, 0, whWndSize.x, whWndSize.y);
		glPointSize(8.0f);
		glBegin(GL_POINTS);
		for (Int32 iy = -m_imgInfo.nHeight / 2; iy < m_imgInfo.nHeight / 2; iy++) {
			for (Int32 ix = -m_imgInfo.nWidth / 2; ix < m_imgInfo.nWidth / 2; ix++) {
				Size szCoord = NWL_XY_TO_X((ix + m_imgInfo.nWidth / 2) * 3, (iy + m_imgInfo.nHeight / 2) * 3, m_imgInfo.nWidth);
				glVertex2f(static_cast<Float32>(ix * 16) / static_cast<Float32>(whWndSize.x),
					static_cast<Float32>(iy * 16) / static_cast<Float32>(whWndSize.y));
				glColor3ub(m_imgInfo.pClrData[szCoord + 0], m_imgInfo.pClrData[szCoord + 1], m_imgInfo.pClrData[szCoord + 2]);
			}
		}
		glEnd();
		V2i xyCrsCrdI = { m_rEngine.GetCursor().GetMoveX(), m_rEngine.GetCursor().GetMoveY() };
		xyCrsCrdI.x = xyCrsCrdI.x - (whWndSize.x / 2.0f);
		xyCrsCrdI.y = -(xyCrsCrdI.y + whWndSize.y / 2.0f) + whWndSize.y;
		V2f xyCrsCrdF = xyCrsCrdI;
		xyCrsCrdF.x /= (whWndSize.x / 2.0f);
		xyCrsCrdF.y /= (whWndSize.y / 2.0f);
		glPointSize(32.0f);
		glBegin(GL_POINTS);
		if (m_rEngine.GetCursor().GetHeld(CRS_0)) {
			m_imgInfo.SetPixel(((xyCrsCrdF.x + whWndSize.x) * 100.0f) / m_imgInfo.nWidth * 100, ((xyCrsCrdF.y + whWndSize.y) * 100.0f) / m_imgInfo.nHeight * 100, 150, 150, 150);
		}
		glVertex2f(xyCrsCrdF.x, xyCrsCrdF.y);
		glColor3ub(255, 255, 255);
		glEnd();
	}

	void GamerState::OnEvent(CursorEvent& rmEvt) { }
	void GamerState::OnEvent(KeyboardEvent& rkEvt) {
		switch (rkEvt.evType) {
		case ET_KEYBOARD_RELEASE:
			switch (rkEvt.keyCode) {
			case KC_N:
				break;
			case KC_R:
				default: break;
			case KC_1:
				break;
			case KC_2:
				break;
			}
			break;
		default: break;
		}
	}
	void GamerState::OnEvent(WindowEvent& rwEvt) {}
	// --==</core_methods>==--
}
#if (false)
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
#if(NW_WAPI & NW_WAPI_WIN)
			ImGui_ImplWin32_Init(m_rEngine.GetWindow()->GetNative());
#endif
#if (NW_GAPI & NW_GAPI_OGL)
			ImGui_ImplOpenGL3_Init("#version 430");
#endif
#if (NW_GAPI & NW_GAPI_DX)
			auto pGfx = static_cast<GfxEngineDx*>(m_rEngine.GetGfx());
			ImGui_ImplDX11_Init(pGfx->GetDevice(), pGfx->GetContext());
#endif
		}

		GuiOfCoreEngine::Get().bIsEnabled = true;
		GuiOfGraphEngine::Get().bIsEnabled = true;
		GuiOfDataSys::Get().bIsEnabled = true;

		return true;
	}
	void GuiState::Quit() {
#if (NW_GAPI & NW_GAPI_OGL)
		ImGui_ImplOpenGL3_Shutdown();
#endif
#if (NW_GAPI & NW_GAPI_DX)
		ImGui_ImplDX11_Shutdown();
#endif
#if (NW_WAPI & NW_WAPI_WIN)
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

	void GuiState::OnEvent(CursorEvent& rmEvt) { }
	void GuiState::OnEvent(KeyboardEvent& rkEvt) { }
	void GuiState::OnEvent(WindowEvent& rwEvt) { }
	// --==</core_methods>==--
	
	// --==<implementation_methods>==--
	inline void GuiState::BeginDraw() {
#if (NW_GAPI & NW_GAPI_OGL)
		ImGui_ImplOpenGL3_NewFrame();
#endif
#if (NW_GAPI & NW_GAPI_DX)
		ImGui_ImplDX11_NewFrame();
#endif
#if (NW_WAPI & NW_WAPI_WIN)
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
#if (NW_GAPI & NW_GAPI_OGL)
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
#if (NW_GAPI & NW_GAPI_DX)
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
		m_rEngine(CoreEngine::Get()),
		m_pShdBuf() { }
	GfxState::~GfxState() { }

	// --==<core_methods>==--
	bool GfxState::Init() {
		m_pShdBuf.MakeRef<ShaderBuf>();
		m_pShdBuf->SetData(1 << 12, nullptr);

		{	// shaders
			Shader* pShader = nullptr;
			pShader = DataSys::GetDR<Shader>(DataSys::NewDR<Shader>("shd_3d_default"));
			if (!pShader->LoadF("D:/dev/native_world/nw_engine/src_glsl/shd_3d_default.glsl")) { return false; }

			pShader = DataSys::GetDR<Shader>(DataSys::NewDR<Shader>("shd_3d_batch"));
			if (!pShader->LoadF("D:/dev/native_world/nw_engine/src_glsl/shd_3d_batch.glsl")) { return false; }
			m_pShdBuf->Remake(pShader->GetShdLayout());
		}
		{	// textures
			Texture* pTex = nullptr;
			//pTex = DataSys::GetDR<Texture>(DataSys::NewDR<Texture>("tex_nw_bat", TextureTypes::TXT_2D));
			pTex->LoadF(R"(D:\dev\native_world\nw_engine\data\image\nw_bat.png)");
		}
		{	// materials
			GfxMaterial* pgMtl = nullptr;
			pgMtl = DataSys::GetDR<GfxMaterial>(DataSys::NewDR<GfxMaterial>("gmt_3d_default"));
			pgMtl->SetShader(DataSys::GetDR<Shader>("shd_3d_default"));
			pgMtl = DataSys::GetDR<GfxMaterial>(DataSys::NewDR<GfxMaterial>("gmt_3d_batch"));
			pgMtl->SetShader(DataSys::GetDR<Shader>("shd_3d_batch"));
			pgMtl->SetTexture(DataSys::GetDR<Texture>("tex_nw_bat"), "unf_tex[0]");
		}
		{	// framebuffers
		}

		GfxCameraLad::Get().SetKeyboard(&m_rEngine.GetKeyboard());
		GfxCameraLad::Get().SetCursor(&m_rEngine.GetCursor());

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
#endif