#include <nw_pch.hpp>
#include "nw_gui_sys.h"

#include <gui/nw_gui_of.h>

#include <ecs/nw_scene.h>

#include <gl/vision/nw_gcamera.h>
#include <gl/vision/nw_gmaterial.h>

#include <core/nw_graph_engine.h>
#include <gl/gcontext/nw_gcontext.h>
#include <gl/gcontext/nw_framebuf.h>
#include <gl/control/nw_gapi.h>
#include <gl/gcontext/nw_window.h>
#include <gl/render/nw_drawable.h>
#include <gl/vision/nw_gcamera.h>
#include <gl/vision/nw_gmaterial.h>

#include <core/nw_core_engine.h>
#include <sys/nw_ev_sys.h>
#include <sys/nw_time_sys.h>
#include <sys/nw_io_sys.h>
#include <sys/nw_rand_sys.h>
#include <sys/nw_data_sys.h>
#include <sys/nw_mem_sys.h>
#include <sys/nw_log_sys.h>

#if (NW_GUI & NW_GUI_IMGUI)
#include <imgui.h>
#include <imgui_internal.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>
#include <glfw/glfw3.h>
#define GUI_DEFAULT_TREE_FLAGS ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
#endif	// NW_GUI


// --==<Static fields>==--
namespace NW
{
	struct NW_API DrawToolsGui
	{

	};
	static DrawToolsGui s_rtGui;

	bool GuiSys::s_bIsDrawing{ false };
	
	GuiWindow* GuiSys::s_pCurrWindow = nullptr;
	GuiIO GuiSys::s_GuiIO = GuiIO();
	GuiStyle GuiSys::s_GuiStyle = GuiStyle();

	static ImGuiContext *s_pGuiContext = nullptr;
	static ImGuiIO *s_pGuiIO = nullptr;
	static ImGuiStyle *s_pGuiStyle = nullptr;
}
// --==</Static fields>==--

namespace NW
{
	// --setters

	void GuiSys::SetWindow(GuiWindow* pWindow) {
		s_pCurrWindow = pWindow;
	}

	// --==<core_methods>==--
	void GuiSys::OnInit()
	{
		AGraphicsApi* pGApi = GraphEngine::GetGApi();
		if (pGApi == nullptr) { return; }
		AWindow* pWindow = CoreEngine::Get().GetWindow();
		if (pWindow == nullptr) {
			return;
		}
		AGContext* pGContext = pWindow->GetGContext();
		if (pGContext == nullptr) {
			NW_ERR("GraphicsContext was not found");
			return;
		}
		GuiOfDataSys::Get().bIsEnabled = true;
		GuiOfTimeSys::Get().bIsEnabled = true;
		GuiOfSceneEditor::Get().bIsEnabled = true;
		GuiOfEntityEditor::Get().bIsEnabled = true;

	#if (NW_GUI & NW_GUI_NATIVE)
	#endif	// NW_GUI
	#if (NW_GUI & NW_GUI_COUT)
	#endif // NW_GUI
	#if (NW_GUI & NW_GUI_IMGUI)

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

		ImGui_ImplGlfw_InitForOpenGL(EvSys::s_pNativeWindow, true);
		ImGui_ImplOpenGL3_Init("#version 430");
	#endif	// NW_GUI
	}
	void GuiSys::OnQuit()
	{
	#if (NW_GUI & NW_GUI_NATIVE)
	#endif // NW_GUI
	#if (NW_GUI & NW_GUI_COUT)
	#endif // NW_GUI
	#if (NW_GUI & NW_GUI_IMGUI)
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	#endif	// NW_GUI
	}
	void GuiSys::Update()
	{
	#if (NW_GUI & NW_GUI_NATIVE)
	#endif // NW_GUI
	#if (NW_GUI & NW_GUI_COUT)
	#endif // NW_GUI
	#if (NW_GUI & NW_GUI_IMGUI)
		//s_pCurrWindow->whMaxSize.x = ImGui::GetWindowContentRegionMax().x;
		//s_pCurrWindow->whMaxSize.y = ImGui::GetWindowContentRegionMax().y;
		//s_pCurrWindow->Content.whSize.x = ImGui::GetContentRegionAvail().x;
		//s_pCurrWindow->Content.whSize.y = ImGui::GetContentRegionAvail().y;
		//s_pCurrWindow->Content.whMaxSize.x = ImGui::GetContentRegionMax().x;
		//s_pCurrWindow->Content.whMaxSize.y = ImGui::GetContentRegionMax().y;

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Files")) {
				if (ImGui::MenuItem("New Scene...", "Ctrl+N")) {
					String strFPath = DataSys::FDialog_save("all_files(*.*)\0*.*\0cn_scene(*.scn)\0*.scn\0");
					if (!strFPath.empty()) {}
				}
				if (ImGui::MenuItem("Save Scene As...", "Ctrl+S")) {
					String strFPath = DataSys::FDialog_save("all_files(*.*)\0*.*\0cn_scene(*.scn)\0*.scn\0");
					if (!strFPath.empty()) { Scene::Get().SaveF(&strFPath[0]); }
				}
				if (ImGui::MenuItem("Load Scene...", "Ctrl+S")) {
					String strFPath = DataSys::FDialog_load("all_files(*.*)\0*.*\0cn_scene(*.scn)\0*.scn\0");
					if (!strFPath.empty()) { Scene::Get().LoadF(&strFPath[0]); }
				}
				ImGui::EndMenu();
			}
			else if (ImGui::BeginMenu("View")) {
				ImGui::Checkbox("GuiOf_Engine",				&GuiOfEngine::Get().bIsEnabled);
				ImGui::Checkbox("GuiOf_Data_System",		&GuiOfDataSys::Get().bIsEnabled);
				ImGui::Checkbox("GuiOf_Memory_System",		&GuiOfMemSys::Get().bIsEnabled);
				ImGui::Checkbox("GuiOf_Time_System",		&GuiOfTimeSys::Get().bIsEnabled);
				//ImGui::Checkbox("GuiOf_Log_System",		&GuiOfLogSys::Get().bIsEnabled);
				//ImGui::Checkbox("GuiOf_Random_System",	&GuiOfRandSys::Get().bIsEnabled);
				ImGui::Checkbox("GuiOf_Render",				&GuiOfRender::Get().bIsEnabled);
				ImGui::Checkbox("GuiOf_Console",			&GuiOfConsole::Get().bIsEnabled);

				ImGui::Checkbox("Code_Editor",				&GuiOfCodeEditor::Get().bIsEnabled);
				ImGui::Checkbox("Scene_Editor",				&GuiOfSceneEditor::Get().bIsEnabled);
				ImGui::Checkbox("Entity_Editor",			&GuiOfEntityEditor::Get().bIsEnabled);
				ImGui::Checkbox("Sprite_Editor",			&GuiOfSpriteEditor::Get().bIsEnabled);

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	#endif // NW_GUI
		GuiOfEngine::Get().OnDraw();
		GuiOfDataSys::Get().OnDraw();
		GuiOfMemSys::Get().OnDraw();
		GuiOfTimeSys::Get().OnDraw();
		//GuiOfLogSys::Get().OnDraw();
		//GuiOfRandSys::Get().OnDraw();
		GuiOfRender::Get().OnDraw();
		GuiOfConsole::Get().OnDraw();

		GuiOfCodeEditor::Get().OnDraw();
		GuiOfSceneEditor::Get().OnDraw();
		GuiOfEntityEditor::Get().OnDraw();
		GuiOfSpriteEditor::Get().OnDraw();
	}

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
		case ET_KEY_TYPE:
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
			//s_pCurrWindow->xyCrd.x = rwEvt.nX;
			//s_pCurrWindow->xyCrd.y = rwEvt.nY;
			//s_pCurrWindow->Content.xyCrd = s_pCurrWindow->xyCrd;
			break;
		case ET_WINDOW_RESIZE:
			//s_pCurrWindow->whSize.x = rwEvt.nX;
			//s_pCurrWindow->whSize.y = rwEvt.nY;
			//s_pCurrWindow->Content.whSize = s_pCurrWindow->whSize;
			break;
		case ET_WINDOW_FOCUS:
			//s_pCurrWindow->bIsFocused = rwEvt.bIsFocused;
			break;
		case ET_WINDOW_CLOSE:
			//s_pCurrWindow->bIsClosed = true;
			break;
		}
	}
	// --==<Drawing>==--
	void GuiSys::BeginDraw()
	{
		bool bCanDraw = !s_bIsDrawing; if (!bCanDraw) { NW_ERR("Can not draw for now"); return; } s_bIsDrawing = true;
	#if (NW_GUI & NW_GUI_NATIVE)
	#endif	// NW_GUI
	#if (NW_GUI & NW_GUI_COUT)
	#endif	// NW_GUI
	#if (NW_GUI & NW_GUI_IMGUI)
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
	#endif	// NW_GUI
	}
	void GuiSys::EndDraw()
	{
		bool bCanDraw = s_bIsDrawing;
		if (!bCanDraw) { NW_ERR("Can not draw for now"); return; }
		s_bIsDrawing = false;
	#if (NW_GUI & NW_GUI_NATIVE)
	#endif	// NW_GUI
	#if (NW_GUI & NW_GUI_COUT)
	#endif	// NW_GUI
	#if (NW_GUI & NW_GUI_IMGUI)
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		ImGui::EndFrame();

		if (s_pGuiIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(EvSys::s_pNativeWindow);
		}
	#endif	// NW_GUI
	}

	void GuiSys::OnDraw(AGuiWidget* pAGuiWidget)
	{
	}
	void GuiSys::OnDrawStr(const char* str, V2f whSize, V2f xyCoord, float zRotation)
	{
		#if (NW_GUI & NW_GUI_NATIVE)
		#endif // NW_GUI
		#if (NW_GUI & NW_GUI_COUT)
		#endif // NW_GUI
		#if (NW_GUI & NW_GUI_IMGUI)
		#endif	// NW_GUI
	}
	inline void GuiSys::OnDrawCh(Char cCharacter, V2f whSize, V2f xyCoord, float zRotation)
	{
	}
	// --==</Drawing>==--

	// --==<Widgets>==--
	 void GuiSys::OnDrawTextF(const V2i& xyCoord, const V2i& whSize, const char* sFormat, ...)
	 {
	#if (NW_GUI & NW_GUI_NATIVE)
	#endif	// NW_GUI
	#if (NW_GUI & NW_GUI_COUT)
	#endif	// NW_GUI
	#if (NW_GUI & NW_GUI_IMGUI)
		 va_list args;
		 va_start(args, sFormat);
		 //ImGui::Text(sFormat, args);
		 ImGui::TextV(sFormat, args);
		 va_end(args);
	#endif	// NW_GUI
	 }
	 void GuiSys::OnDrawImage(void* pImgId, const V2i& xyCoord, const V2i& whSize)
	 {
		 ImGui::Image(pImgId, ImVec2{ (float)whSize.x, (float)whSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	 }
	 bool GuiSys::OnDrawSliderI8(Int8* pFirst, UInt16 unCount, Int32 nMin, Int32 nMax, const V2i& xyCoord, const V2i& whSize, const char* sLabel)
	 {
		return ImGui::SliderScalarN(sLabel, ImGuiDataType_S8, pFirst, unCount, &nMin, &nMax);
	 }
	 bool GuiSys::OnDrawSliderI32(Int32* pFirst, UInt16 unCount, Int32 nMin, Int32 nMax, const V2i& xyCoord, const V2i& whSize, const char* sLabel)
	 {
		 return ImGui::SliderScalarN(sLabel, ImGuiDataType_S32, pFirst, unCount, &nMin, &nMax);
	 }
	 bool GuiSys::OnDrawSliderF(float* pFirst, UInt16 unCount, float nMin, float nMax, const V2i& xyCoord, const V2i& whSize, const char* sLabel)
	 {
		 return ImGui::SliderScalarN(sLabel, ImGuiDataType_Float, pFirst, unCount, &nMin, &nMax);
	 }
	 bool GuiSys::OnDrawInputI8(Int8* pFirst, UInt16 unCount, const V2ui& xyCoord, const V2ui& whSize, const char* sLabel)
	 {
		 return ImGui::InputScalarN(sLabel, ImGuiDataType_S8, pFirst, unCount);
	 }
	 bool GuiSys::OnDrawInputI32(Int32* pFirst, UInt16 unCount, const V2ui& xyCoord, const V2ui& whSize, const char* sLabel)
	 {
		 return ImGui::InputScalarN(sLabel, ImGuiDataType_S32, pFirst, unCount);
	 }
	 bool GuiSys::OnDrawInputF(float* pFirst, UInt16 unCount, const V2ui& xyCoord, const V2ui& whSize, const char* sLabel)
	 {
		 return ImGui::InputScalarN(sLabel, ImGuiDataType_Float, pFirst, unCount);
	 }
	 bool GuiSys::OnDrawButton(const V2ui& xyCoord, const V2ui& whSize, const char* sLabel)
	 {
		 return ImGui::ButtonEx(sLabel, ImVec2{ (float)whSize.x, (float)whSize.y }, ImGuiButtonFlags_None);
	 }
	// --==</Widgets>==--
	// --==</core_methods>==--
}