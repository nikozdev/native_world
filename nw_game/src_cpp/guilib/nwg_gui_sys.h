#ifndef NWG_GUI_SYSTEM_H
#define NWG_GUI_SYSTEM_H

#include <nwg_pch.hpp>

struct ImGuiIO;
struct ImGuiStyle;

namespace NWG
{
	/// GuiSys Static Class
	class GuiSys
	{
	public:
		// --predicates
		static inline bool IsDrawing() { return s_bIsDrawing; }

		// --core_methods
		static void OnInit();
		static void OnQuit();
		static void Update();
		static void BeginDraw();
		static void BeginWindow(const char* strName);
		static void EndWindow();
		static void EndDraw();

		// --on_event_methods
		void GuiSys::OnEvent(MouseEvent& rmEvt);
		void GuiSys::OnEvent(KeyboardEvent& rkEvt);
		void GuiSys::OnEvent(WindowEvent& rwEvt);
	private:
		static bool s_bIsDrawing;
	};
}

#endif // NW_GUI_SYSTEM_H