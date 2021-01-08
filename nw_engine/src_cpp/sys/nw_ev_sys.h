#ifndef NW_EVENT_SYSTEM_H
#define NW_EVENT_SYSTEM_H
#include <lib/nw_event.h>

#include <nw_decl.hpp>
#include <gl_decl.hpp>

namespace NW
{
	/// EventSystem static class
	/// -- Has static callback function for the window
	/// -- Keeps tracking of new events, Updates Time and InOut systems
	class NW_API EvSys
	{
	public:
		static AppWindow* s_pAppWindow;
		static NativeWindow s_pNativeWindow;
		static bool s_bWindowFocused;
	public:
		// --getters

		// --core_methods
		static bool OnInit();
		static void Update();

	#if (NW_WINDOW & NW_WINDOW_CONSOLE)
		// --input_callbacks: mouse
		static void cb_mouse_coord(double dX, double dY);
		static void cb_mouse_scroll(double dX, double dY);
		static void cb_mouse_button(UInt32 unButton, UInt32 unAction);
		// --input_callbacks: keyboard
		static void cb_keyboard(UInt32 unKey, UInt32 unScanCode, UInt32 unAction);
		static void cb_keyboard_char(UInt32 unChar);
		// --window_callbacks
		static void cb_menu(Int32 nCmdId);
		static void cb_window_size(Int32 nW, Int32 nH);
		static void cb_window_focus(Int32 nFocus);
	#endif // NW_WINDOW
	#if (NW_WINDOW & NW_WINDOW_GLFW)
		// --input_callbacks: mouse
		static void cb_mouse_coord(GLFWwindow* pWindow, double nCoordX, double nCoordY);
		static void cb_mouse_scroll(GLFWwindow* pWindow, double nScrollDeltaY, double nScrollY);
		static void cb_mouse_button(GLFWwindow* pWindow, Int32 nButton, Int32 nAction, Int32 nMode);
		// --input_callbacks: keyboard
		static void cb_keyboard(GLFWwindow* pWindow, Int32 nKey, Int32 nScanCode, Int32 nAction, Int32 nMode);
		static void cb_keyboard_char(GLFWwindow* pWindow, UInt32 unChar);
		// --input_callbacks: window
		static void cb_window_close(GLFWwindow* pWindow);
		static void cb_window_size(GLFWwindow* pWindow, Int32 nWidth, Int32 nHeight);
		static void cb_window_focus(GLFWwindow* pWindow, Int32 nFocus);
		// --other_callbacks: window
		static void cb_error(Int32 nErrCode, const char* strErrMsg);
	#endif // NW_WINDOW
	};
}
#endif	// NW_EVENT_SYSTEM_H

/*
* Development started: 18.10.2020
* By MichailRis tutorial
* 31.10.2020
* Improved with the tutorials of Cherno
*/