#include <nwg_pch.hpp>
#include "nwg_core_states.h"

#include <nwg_gui_of.h>

#pragma warning(disable : 4312)

namespace NWG
{
	DrawerState::DrawerState() :
		m_GCamera(GCamera()),
		m_pFmBuf(CoreEngine::Get().GetMemory()),
		m_pVtxBuf(CoreEngine::Get().GetMemory()),
		m_pIdxBuf(CoreEngine::Get().GetMemory()),
		m_pShdBuf(CoreEngine::Get().GetMemory()) { }
	DrawerState::~DrawerState() { }

	// --==<core_methods>==--
	bool DrawerState::Init() {
		NW::FrameBufInfo fbInfo;
		fbInfo.unHeight = 800;
		fbInfo.unWidth = 1200;
		fbInfo.unSamples = 1;
		NW::AFrameBuf::Create("fmb_draw", fbInfo, m_pFmBuf);
		
		NW::AVertexBuf::Create(1 << 12, nullptr, m_pVtxBuf);
		NW::AIndexBuf::Create(1 << 12, nullptr, m_pIdxBuf);
		NW::AShaderBuf::Create(1 << 12, nullptr, m_pShdBuf);

		m_pVtxData = NWL::LinearAllocator(CoreEngine::Get().GetMemory().Alloc(m_pVtxBuf->GetDataSize()), m_pVtxBuf->GetDataSize());
		m_pIdxData = NWL::LinearAllocator(CoreEngine::Get().GetMemory().Alloc(m_pIdxBuf->GetDataSize()), m_pIdxBuf->GetDataSize());
		m_pShdData = NWL::LinearAllocator(CoreEngine::Get().GetMemory().Alloc(m_pShdBuf->GetDataSize()), m_pShdBuf->GetDataSize());

		{	// shaders
			AShader* pShader = nullptr;
			pShader = AShader::Create("shd_3d_batch");
			if (!pShader->LoadF("D:/dev/native_world/nw_engine/src_glsl/shd_3d_batch.glsl")) { return false; }
		}
		{	// textures
			ATexture2d* pTex = nullptr;
			pTex = ATexture2d::Create("tex_white_solid");
			pTex->LoadF("");
		}
		{	// materials
			GMaterial* pGMtl = nullptr;
			pGMtl= CoreEngine::Get().NewT<GMaterial>("gmt_3d_batch");
			pGMtl->SetShader(NWL::ADataRes::GetDataRes<AShader>("shd_3d_batch"));
		}

		return true;
	}
	void DrawerState::OnQuit() {
		m_pFmBuf.Reset();
		m_pVtxBuf.Reset();
		m_pIdxBuf.Reset();
		m_pShdBuf.Reset();

		CoreEngine::Get().GetMemory().Dealloc(m_pVtxData.GetDataBeg(), m_pVtxData.GetDataSize());
		CoreEngine::Get().GetMemory().Dealloc(m_pIdxData.GetDataBeg(), m_pIdxData.GetDataSize());
		CoreEngine::Get().GetMemory().Dealloc(m_pShdData.GetDataBeg(), m_pShdData.GetDataSize());
	}
	void DrawerState::Update()
	{
		NW::AGApi* pGApi = NW::CoreEngine::Get().GetGApi();
		NW::GMaterial* pGMtl = NWL::ADataRes::GetDataRes<GMaterial>("gmt_3d_batch");

		float vtxData[] = {
			-0.5f,	-0.5f,	0.0f,		0.0f,	0.0f,	1.0f,	1.0f,		0.0f,	0.0f,		0.0f,
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f,
			-0.5f,	0.5f,	0.0f,		0.0f,	1.0f,	1.0f,	1.0f,		0.0f,	1.0f,		0.0f,
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f,
			0.5f,	0.5f,	0.0f,		1.0f,	1.0f,	1.0f,	1.0f,		1.0f,	1.0f,		0.0f,
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f,
			0.5f,	-0.5f,	0.0f,		1.0f,	0.0f,	1.0f,	1.0f,		1.0f,	0.0f,		0.0f,
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f
		};
		UInt32 idxData[] = {
			0, 1, 2,
			2, 3, 0
		};
		memcpy(m_pVtxData.Alloc(sizeof(vtxData)), vtxData, sizeof(vtxData));
		memcpy(m_pIdxData.Alloc(sizeof(idxData)), idxData, sizeof(idxData));
		memcpy(m_pShdData.Alloc(sizeof(Mat4f)), &m_GCamera.GetProjMatrix()[0], sizeof(Mat4f));
		memcpy(m_pShdData.Alloc(sizeof(Mat4f)), &m_GCamera.GetViewMatrix()[0], sizeof(Mat4f));

		m_pFmBuf->Bind();
		m_pFmBuf->Clear(FB_COLOR | FB_DEPTH | FB_STENCIL);
		NW::FrameBufInfo fbInfo = m_pFmBuf->GetInfo();
		pGApi->SetViewport(0, 0, static_cast<Int32>(fbInfo.unWidth), static_cast<Int32>(fbInfo.unHeight));

		pGApi->SetModes(true, PM_BLEND);
		pGApi->SetBlendFunc(BC_SRC_ALPHA, BC_ONE_MINUS_SRC_ALPHA);
		pGApi->SetModes(false, PM_DEPTH_TEST);
		pGApi->SetPrimitiveType(PT_TRIANGLES);

		pGMtl->Enable();
		m_pVtxBuf->SetLayout(pGMtl->GetShader()->GetVtxLayout());
		m_pShdBuf->SetLayout(pGMtl->GetShader()->GetShdLayout());
		
		m_pVtxBuf->SetSubData(m_pVtxData.GetAllocSize(), m_pVtxData.GetDataBeg(), 0);
		m_pIdxBuf->SetSubData(m_pIdxData.GetAllocSize(), m_pIdxData.GetDataBeg(), 0);
		m_pShdBuf->SetSubData(m_pShdData.GetAllocSize(), m_pShdData.GetDataBeg(), 0);

		m_pShdBuf->Bind();
		m_pVtxBuf->Bind();
		m_pIdxBuf->Bind();
		pGApi->DrawIndexed(m_pIdxBuf->GetDataSize() / sizeof(UInt32));
		m_pIdxBuf->Unbind();
		m_pVtxBuf->Unbind();
		m_pShdBuf->Unbind();

		pGMtl->Disable();

		m_pShdData.Clear();
		m_pVtxData.Clear();
		m_pIdxData.Clear();

		m_pFmBuf->Unbind();


		NW::GCameraLad::Get().UpdateCamera(&m_GCamera);
	}
	void DrawerState::OnEnable() { }
	void DrawerState::OnDisable() { }

	void DrawerState::OnEvent(MouseEvent& rmEvt)
	{
		NW::GCameraLad::Get().OnEvent(rmEvt, &m_GCamera);
	}
	void DrawerState::OnEvent(KeyboardEvent& rkEvt)
	{
		NW::GCameraLad::Get().OnEvent(rkEvt, &m_GCamera);
	}
	void DrawerState::OnEvent(WindowEvent& rwEvt)
	{
		NW::GCameraLad::Get().OnEvent(rwEvt, &m_GCamera);
	}
	// --==</core_methods>==--
}

namespace NWG
{
	GuiState::GuiState() { }
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

		{
			GuiOfCoreEngine::Get().bIsEnabled = true;
			GuiOfGEngine::Get().bIsEnabled = true;
			GuiOfDataSys::Get().bIsEnabled = true;
			GuiOfTimeSys::Get().bIsEnabled = true;
			GuiOfMemSys::Get().bIsEnabled = true;
		}

		return true;
	}
	void GuiState::OnQuit() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void GuiState::Update() {
		{	// begin drawing
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Note: Switch this to true to enable dockspace
			static bool bDockspace = true;
			static bool bFullScreenPersist = true;
			bool bFullScreen = bFullScreenPersist;

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
			ImGui::Begin("DockSpace", &bDockspace, WindowFlags);
			ImGui::PopStyleVar();

			if (bFullScreen) ImGui::PopStyleVar(2);

			// DockSpace
			float nMinSizeW = m_pGuiStyle->WindowMinSize.x;
			m_pGuiStyle->WindowMinSize.x = 100.0f;
			m_pGuiStyle->Alpha = 1.0f;
			if (m_pGuiIO->ConfigFlags & ImGuiConfigFlags_DockingEnable) {
				ImGuiID nDockspaceId = ImGui::GetID("DockSpace");
				ImGui::DockSpace(nDockspaceId, ImVec2(0.0f, 0.0f), DockspaceFlags);
			}
			m_pGuiStyle->WindowMinSize.x = nMinSizeW;
		}

		{	// drawing
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("view")) {
					ImGui::Checkbox("core_engine", &GuiOfCoreEngine::Get().bIsEnabled);
					ImGui::Checkbox("graphichs_engine", &GuiOfGEngine::Get().bIsEnabled);
					ImGui::Checkbox("console_engine", &GuiOfCmdEngine::Get().bIsEnabled);
					ImGui::Checkbox("data_system", &GuiOfDataSys::Get().bIsEnabled);
					ImGui::Checkbox("memory_mystem", &GuiOfMemSys::Get().bIsEnabled);
					ImGui::Checkbox("time_system", &GuiOfTimeSys::Get().bIsEnabled);
					ImGui::Checkbox("code_editor", &GuiOfCodeEditor::Get().bIsEnabled);
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

			ImGui::Begin("framebufs");
			AFrameBuf* pFmBuf = NWL::ADataRes::GetDataRes<AFrameBuf>("fmb_draw");
			NW::FrameBufInfo fbInfo = pFmBuf->GetInfo();
			NWL::V2f whSize = { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y };
			if (fbInfo.unWidth != static_cast<UInt16>(whSize.x) || fbInfo.unHeight != static_cast<UInt16>(whSize.y)) {
				pFmBuf->SetSizeWH(whSize.x, whSize.y);
			}
			ImGui::Image(reinterpret_cast<Ptr>(pFmBuf->GetAttachment(FBAT_COLOR)->GetId()), { whSize.x, whSize.y });
			static NWL::V3f rgbClear;
			if (ImGui::ColorEdit3("clear_color", &rgbClear[0])) { CoreEngine::Get().GetGApi()->SetClearColor(rgbClear.r, rgbClear.g, rgbClear.b, 1.0f); }
			ImGui::End();
		}

		{	// end drawing
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			ImGui::EndFrame();

			if (m_pGuiIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(static_cast<GLFWwindow*>(NW::CoreEngine::Get().GetWindow()->GetNative()));
			}
		}
	}
	void GuiState::OnEnable() { }
	void GuiState::OnDisable() { }

	void GuiState::OnEvent(MouseEvent& rmEvt)
	{
		rmEvt.bIsHandled = true;
	}
	void GuiState::OnEvent(KeyboardEvent& rkEvt)
	{
		rkEvt.bIsHandled = true;
	}
	void GuiState::OnEvent(WindowEvent& rwEvt)
	{
		rwEvt.bIsHandled = true;
	}
	// --==</core_methods>==--
}

namespace NWG
{
	CreatorState::CreatorState() :
		m_guiState(GuiState()),
		m_drwState(DrawerState())
	{
		CoreEngine::Get().AddState(m_guiState);
		CoreEngine::Get().AddState(m_drwState);
	}
	CreatorState::~CreatorState() { }

	// --==<core_methods>==--
	bool CreatorState::Init()
	{		
		return true;
	}
	void CreatorState::OnQuit() { }
	void CreatorState::Update()
	{
		m_drwState.Update();
		m_guiState.Update();
	}
	void CreatorState::OnEnable()
	{
		m_drwState.OnEnable();
		m_guiState.OnEnable();
	}
	void CreatorState::OnDisable() {
		m_drwState.OnDisable();
		m_guiState.OnDisable();
	}

	void CreatorState::OnEvent(MouseEvent& rmEvt)
	{
		//m_guiState.OnEvent(rmEvt);
		if (rmEvt.bIsHandled) { return; }
		m_drwState.OnEvent(rmEvt);
	}
	void CreatorState::OnEvent(KeyboardEvent& rkEvt)
	{
		//m_guiState.OnEvent(rkEvt);
		if (rkEvt.bIsHandled) { return; }
		m_drwState.OnEvent(rkEvt);
	}
	void CreatorState::OnEvent(WindowEvent& rwEvt)
	{
		//m_guiState.OnEvent(rwEvt);
		if (rwEvt.bIsHandled) { return; }
		m_drwState.OnEvent(rwEvt);
	}
	// --==</core_methods>==--
}