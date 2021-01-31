#include <nwg_pch.hpp>
#include "nwg_core_states.h"

#include <nwg_gui_of.h>
#include <nwg_gcamera_lad.h>

#pragma warning(disable : 4312)

#include <glad/glad.h>

namespace NWG
{
	DrawerState::DrawerState() :
		m_GCamera(GCamera()),
		m_pFmBuf(GEngine::Get().GetMemory()),
		m_pVtxBuf(GEngine::Get().GetMemory()),
		m_pIdxBuf(GEngine::Get().GetMemory()),
		m_pShdBuf(GEngine::Get().GetMemory()),
		m_pVtxArr(GEngine::Get().GetMemory()) { }
	DrawerState::~DrawerState() { }

	// --==<core_methods>==--
	bool DrawerState::Init() {

		{
			FrameBufInfo fbInfo;
			fbInfo.xywhViewport = { 0, 0, 800, 1200 };
			fbInfo.unSamples = 1;
			FrameBuf::Create("fmb_draw", fbInfo, m_pFmBuf);
			
			TextureInfo texInfo;
			texInfo.bGenMipmap = true; texInfo.bGenSubImage = true;
			texInfo.unSamples = 1;

			{
				RefKeeper<Texture> rTex(GEngine::Get().GetMemory());
				Texture::Create("fba_draw_color0", TXT_2D, rTex);
				texInfo.pxFormat = PXF_INT8;
				texInfo.texFormat = TXF_RGB; texInfo.texInterFormat = TXFI_RGB;
				rTex->SetInfo(texInfo);
				m_pFmBuf->AttachTexture(rTex);
			}
			{
				RefKeeper<Texture> rTex(GEngine::Get().GetMemory());
				Texture::Create("fba_draw_depth_stencil", TXT_2D, rTex);
				texInfo.pxFormat = PXF_UINT24_8;
				texInfo.texFormat = TXF_DEPTH_STENCIL; texInfo.texInterFormat = TXFI_DEPTH24_STENCIL8;
				rTex->SetInfo(texInfo);
				m_pFmBuf->AttachTexture(rTex);
			}
		}

		VertexBuf::Create(m_pVtxBuf);
		m_pVtxBuf->SetData(1 << 12, nullptr);
		IndexBuf::Create(m_pIdxBuf);
		m_pIdxBuf->SetData(1 << 12, nullptr);
		ShaderBuf::Create(m_pShdBuf);
		m_pShdBuf->SetData(1 << 12, nullptr);
		VertexArr::Create(m_pVtxArr);
		m_pVtxArr->AddVtxBuffer(*m_pVtxBuf);
		m_pVtxArr->SetIdxBuffer(m_pIdxBuf.GetRef());

		m_pVtxData = LinearAllocator(CoreEngine::Get().GetMemory().Alloc(m_pVtxBuf->GetDataSize()), m_pVtxBuf->GetDataSize());
		m_pIdxData = LinearAllocator(CoreEngine::Get().GetMemory().Alloc(m_pIdxBuf->GetDataSize()), m_pIdxBuf->GetDataSize());
		m_pShdData = LinearAllocator(CoreEngine::Get().GetMemory().Alloc(m_pShdBuf->GetDataSize()), m_pShdBuf->GetDataSize());

		{	// shaders
			Shader* pShader = nullptr;
			pShader = Shader::Create("shd_3d_batch");
			if (!pShader->LoadF("D:/dev/native_world/nw_glib/src_glsl/shd_3d_batch.glsl")) { return false; }
			m_pVtxArr->Remake(pShader->GetVtxLayout());
		}
		{	// textures
			Texture* pTex = nullptr;
			pTex = Texture::Create("tex_white_solid", TXT_2D);
			pTex->LoadF("");
		}
		{	// materials
			GMaterial* pGMtl = nullptr;
			pGMtl= CoreEngine::Get().NewT<GMaterial>("gmt_3d_batch");
			pGMtl->SetShader(ADataRes::GetDataRes<Shader>("shd_3d_batch"));
		}

		return true;
	}
	void DrawerState::OnQuit() {
		m_pFmBuf.Reset();
		m_pVtxBuf.Reset();
		m_pIdxBuf.Reset();
		m_pShdBuf.Reset();
		m_pVtxArr.Reset();

		CoreEngine::Get().GetMemory().Dealloc(m_pVtxData.GetDataBeg(), m_pVtxData.GetDataSize());
		CoreEngine::Get().GetMemory().Dealloc(m_pIdxData.GetDataBeg(), m_pIdxData.GetDataSize());
		CoreEngine::Get().GetMemory().Dealloc(m_pShdData.GetDataBeg(), m_pShdData.GetDataSize());
	}
	void DrawerState::Update()
	{
		GCameraLad::Get().UpdateCamera(&m_GCamera);
		
		GMaterial* pGMtl = ADataRes::GetDataRes<GMaterial>("gmt_3d_batch");

		float vtxData[] = {
			-0.5f,	-0.5f,	0.0f,		0.0f,	0.0f,	1.0f,	1.0f,		0.0f,	0.0f,		0.0f,
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f,
			-0.5f,	0.5f,	0.0f,		0.0f,	1.0f,	0.0f,	1.0f,		0.0f,	1.0f,		0.0f,
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f,
			0.5f,	0.5f,	0.0f,		0.0f,	1.0f,	1.0f,	1.0f,		1.0f,	1.0f,		0.0f,
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f,
			0.5f,	-0.5f,	0.0f,		1.0f,	0.0f,	0.0f,	1.0f,		1.0f,	0.0f,		0.0f,
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
		{
			m_pFmBuf->Bind();
			m_pFmBuf->Clear();

			GEngine::Get().SetModes(true, PM_BLEND);
			GEngine::Get().SetBlendFunc(BC_SRC_ALPHA, BC_ONE_MINUS_SRC_ALPHA);
			
			m_pVtxArr->SetDrawPrimitive(PT_TRIANGLES);

			m_pShdBuf->SetLayout(pGMtl->GetShader()->GetShdLayout());

			m_pVtxBuf->SetSubData(m_pVtxData.GetAllocSize(), m_pVtxData.GetDataBeg(), 0);
			m_pIdxBuf->SetSubData(m_pIdxData.GetAllocSize(), m_pIdxData.GetDataBeg(), 0);
			m_pShdBuf->SetSubData(m_pShdData.GetAllocSize(), m_pShdData.GetDataBeg(), 0);

			m_pShdBuf->Bind();
			GEngine::Get().OnDraw(*m_pVtxArr, *pGMtl);
			m_pShdBuf->Unbind();

			m_pShdData.Clear();
			m_pVtxData.Clear();
			m_pIdxData.Clear();

			m_pFmBuf->Unbind();
		}	
	}

	void DrawerState::OnEvent(MouseEvent& rmEvt)
	{
		GCameraLad::Get().OnEvent(rmEvt, &m_GCamera);
	}
	void DrawerState::OnEvent(KeyboardEvent& rkEvt)
	{
		GCameraLad::Get().OnEvent(rkEvt, &m_GCamera);
	}
	void DrawerState::OnEvent(WindowEvent& rwEvt)
	{
		rwEvt.nX = m_pFmBuf->GetWidth();
		rwEvt.nY = m_pFmBuf->GetHeight();
		GCameraLad::Get().OnEvent(rwEvt, &m_GCamera);
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
			GuiOfCodeEditor::Get().OnDraw();
			GuiOfSpriteEditor::Get().OnDraw();
			GuiOfGMaterialEditor::Get().OnDraw();

			ImGui::Begin("framebufs");
			{
				FrameBuf* pFmBuf = ADataRes::GetDataRes<FrameBuf>("fmb_draw");
				
				if (IOSys::GetMousePressed(NW_MS_BTN_RIGHT)) {
					UByte clrData[1 << 2]{ 0 };
					memset(clrData, 'z', sizeof(clrData));
					auto [xWindow, yWindow] = ImGui::GetWindowPos();
					auto hWindow = pFmBuf->GetWidth();
					auto wWindow = pFmBuf->GetHeight();
					auto [xCursor, yCursor] = ImGui::GetMousePos();
					V2i relCursor = { xCursor - xWindow, (hWindow + yWindow) - yCursor };
					std::cout << "mouse x = " << relCursor.x << "; mouse y = " << relCursor.y << std::endl;
					pFmBuf->ReadPixels(&clrData[0], 0, relCursor.x, relCursor.y);
					std::cout << clrData;
				}

				FrameBufInfo fbInfo = pFmBuf->GetInfo();
				auto [xWindow, yWindow] = ImGui::GetWindowPos();
				auto [wWindow, hWindow] = ImGui::GetContentRegionAvail();
				V4i xywhViewport = { xWindow, yWindow, xWindow + wWindow, yWindow + hWindow };
				if (fbInfo.xywhViewport != xywhViewport) { pFmBuf->SetViewport(xywhViewport); pFmBuf->Remake(); }
				ImGui::Image(reinterpret_cast<Ptr>(pFmBuf->GetAttachment(0)->GetRenderId()),
					{ static_cast<Float32>(pFmBuf->GetWidth()), static_cast<Float32>(pFmBuf->GetHeight()) }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
			}
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

	void GuiState::OnEvent(MouseEvent& rmEvt)
	{
	}
	void GuiState::OnEvent(KeyboardEvent& rkEvt)
	{
	}
	void GuiState::OnEvent(WindowEvent& rwEvt)
	{
	}
	// --==</core_methods>==--
}