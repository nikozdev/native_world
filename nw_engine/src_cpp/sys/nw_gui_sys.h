#ifndef NW_GUI_SYSTEM_H
#define NW_GUI_SYSTEM_H

#include <nw_glib_decl.hpp>

#if (NW_GUI & NW_GUI_NATIVE)
#include <glib/nw_gui_widget.h>
#endif	// NW_GUI
#if (NW_GUI & NW_GUI_IMGUI)
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
struct ImGuiIO;
struct ImGuiStyle;
#endif	// NW_GUI

namespace NW
{
	/// GuiSys Static Class
	class NW_API GuiSys
	{
	public:
		// --getters
		static inline GuiIO& GetGuiIO() { return s_GuiIO; }
		static inline GuiStyle& GetGuiStyle() { return s_GuiStyle; }
		static inline GuiWindow* GetWindow() { return s_pCurrWindow; }
		// --setters
		static void SetWindow(GuiWindow* pWindow);
		// -- Predicates
		static inline bool IsDrawing() { return s_bIsDrawing; }

		// -- Core methods
		static void OnInit();
		static void OnQuit();
		static void Update();
		// -- OnEvent methods
		static void OnEvent(MouseEvent& rmEvt);
		static void OnEvent(KeyboardEvent& rkEvt);
		static void OnEvent(WindowEvent& rwEvt);

		static void BeginDraw();
		static void BeginWindow(const char* strName);
		static void OnDraw(AGuiWidget* pAGuiWidget);
		static void EndWindow();
		static void EndDraw();

	private:	// Implementation Attribs
		static GuiIO s_GuiIO;
		static GuiStyle s_GuiStyle;
		
		static bool s_bIsDrawing;

		static GuiWindow* s_pCurrWindow;
	};
}

#endif // NW_GUI_SYSTEM_H