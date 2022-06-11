#ifndef NW_GUI_WINDOW_API_H
#define NW_GUI_WINDOW_API_H
#include "nw_gui_core.hpp"
#if (defined NW_WAPI)
#	include "nw_gui_api.h"
namespace NW
{
	// core_functions
	NW_API v1bit gui_wapi_init(window_handle window);
	NW_API v1bit gui_wapi_quit();
	NW_API v1nil gui_wapi_update();
	NW_API v1bit gui_wapi_wnd_init();
	NW_API v1nil gui_wapi_wnd_quit();
	// app_wnd_core_functions
	NW_API v1nil gui_wapi_wnd_create(ImGuiViewport* viewport);
	NW_API v1nil gui_wapi_wnd_delete(ImGuiViewport* viewport);
	NW_API v1nil gui_wapi_wnd_update(ImGuiViewport* viewport);
	NW_API v1nil gui_wapi_wnd_show(ImGuiViewport* viewport);
	// wnd_graphics
	NW_API v1nil gui_wapi_wnd_render(ImGuiViewport* viewport, ptr_t draw_arg);
	NW_API v1nil gui_wapi_wnd_swap(ImGuiViewport* viewport, ptr_t draw_arg);
	// wnd_getters
	NW_API ImVec2 gui_wapi_wnd_get_coord(ImGuiViewport* viewport);
	NW_API ImVec2 gui_wapi_wnd_get_size(ImGuiViewport* viewport);
	// wnd_setters
	NW_API v1nil gui_wapi_wnd_set_coord(ImGuiViewport* viewport, ImVec2 coord);
	NW_API v1nil gui_wapi_wnd_set_size(ImGuiViewport* viewport, ImVec2 size);
	NW_API v1nil gui_wapi_wnd_set_title(ImGuiViewport* viewport, cstr_t title);
	NW_API v1nil gui_wapi_wnd_set_alpha(ImGuiViewport* viewport, v1f alpha);
	NW_API v1nil gui_wapi_wnd_set_focused(ImGuiViewport* viewport);
	// wnd_predicates
	NW_API v1bit gui_wapi_wnd_is_focused(ImGuiViewport* viewport);
	NW_API v1bit gui_wapi_wnd_is_minimized(ImGuiViewport* viewport);
#	if (NW_WAPI & NW_WAPI_WIN)
	NW_API LRESULT WINAPI gui_wapi_event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	// aux_functions
	NW_API LRESULT WINAPI gui_wapi_wnd_event_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
	NW_API BOOL WINAPI gui_wapi_update_monitors_enum(HMONITOR monitor, HDC device, LPRECT ImRect, LPARAM lparam);
	NW_API v1nil gui_wapi_get_viewport_flags_imstyle(ImGuiViewportFlags flags, DWORD* out_style, DWORD* out_ex_style);
	NW_API v1bit gui_wapi_update_mouse_coord();
	NW_API v1bit gui_wapi_update_mouse_cursor();
#	endif	// WAPI_WIN
}
#endif	// NW_WAPI
#endif	// NW_GUI_WINDOW_API_H