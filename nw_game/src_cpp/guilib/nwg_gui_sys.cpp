#include <nwg_pch.hpp>
#include "nwg_gui_sys.h"

#include <core/nw_core_engine.h>
#include <core/nw_window.h>

#include <glfw/glfw3.h>

namespace NWG
{
	struct DrawToolsGui
	{
	};
	static DrawToolsGui s_rtGui;

	bool GuiSys::s_bIsDrawing{ false };
	
	static ImGuiContext *s_pGuiContext = nullptr;
	static ImGuiIO *s_pGuiIO = nullptr;
	static ImGuiStyle *s_pGuiStyle = nullptr;
}

namespace NWG
{
	// --==<core_methods>==--
	void GuiSys::OnInit()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		s_pGuiContext = ImGui::GetCurrentContext();
		s_pGuiIO = &ImGui::GetIO();
		s_pGuiStyle = &ImGui::GetStyle();

		s_pGuiIO->ConfigFlags |=
			ImGuiConfigFlags_NavEnableKeyboard |
			ImGuiConfigFlags_ViewportsEnable |
			ImGuiConfigFlags_DockingEnable;

		s_pGuiIO->ConfigDockingWithShift = true;

		ImGui::StyleColorsDark();

		if (s_pGuiIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			s_pGuiStyle->WindowRounding = 0.0f;
			s_pGuiStyle->Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(NW::CoreEngine::Get().GetWindow()->GetNative()), true);
		ImGui_ImplOpenGL3_Init("#version 430");
	}
	void GuiSys::OnQuit()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void GuiSys::Update()
	{
	}
	// --==</core_methods>==--

	// --==<Drawing>==--
	void GuiSys::BeginDraw()
	{
		bool bCanDraw = !s_bIsDrawing; if (!bCanDraw) { NWL_ERR("Can not draw for now"); return; } s_bIsDrawing = true;

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
		ImGuiStyle& rStyle = ImGui::GetStyle();
		float nMinSizeW = rStyle.WindowMinSize.x;
		rStyle.WindowMinSize.x = 100.0f;
		rStyle.Alpha = 1.0f;
		if (s_pGuiIO->ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID nDockspaceId = ImGui::GetID("DockSpace");
			ImGui::DockSpace(nDockspaceId, ImVec2(0.0f, 0.0f), DockspaceFlags);
		}
		rStyle.WindowMinSize.x = nMinSizeW;
	}
	void GuiSys::EndDraw()
	{
		bool bCanDraw = s_bIsDrawing;
		if (!bCanDraw) { NWL_ERR("Can not draw for now"); return; }
		s_bIsDrawing = false;
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		ImGui::EndFrame();

		if (s_pGuiIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(static_cast<GLFWwindow*>(NW::CoreEngine::Get().GetWindow()->GetNative()));
		}
	}
	// --==</Drawing>==--
	void GuiSys::OnEvent(MouseEvent& rmEvt)
	{
		switch (rmEvt.EvtType) {
		case ET_MOUSE_PRESS:
			break;
		case ET_MOUSE_RELEASE:
			break;
		case ET_MOUSE_MOVE:
			break;
		case ET_MOUSE_SCROLL:
			break;
		}
	}
	void GuiSys::OnEvent(KeyboardEvent& rkEvt)
	{
		switch (rkEvt.EvtType) {
		case ET_KEY_CHAR:
			break;
		case ET_KEY_RELEASE:
			break;
		case ET_KEY_PRESS:
			break;
		}
	}
	void GuiSys::OnEvent(WindowEvent& rwEvt)
	{
		switch (rwEvt.EvtType) {
		case ET_WINDOW_MOVE:
			break;
		case ET_WINDOW_RESIZE:
			break;
		case ET_WINDOW_FOCUS:
			break;
		case ET_WINDOW_CLOSE:
			break;
		}
	}
}