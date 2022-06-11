#ifndef NW_GUI_API_H
#define NW_GUI_API_H
#include "nw_gui_core.hpp"
#if (defined NW_API)
#include "lib/nw_gui_lib.h"
#include "lib/nw_gui_internal.h"
namespace NW
{
	// data
	extern NW_API ImGuiIO* s_imlib_io;
	extern NW_API ImGuiPlatformIO* s_imsys_io;
	extern NW_API ImGuiStyle* s_imstyle;
	// setters
	NW_API v1nil gui_set_mouse_enabled(v1bit enable);
	NW_API v1nil gui_set_docks_enabled(v1bit enable);
	NW_API v1nil gui_set_viewp_enabled(v1bit enable);
	// predicates
	NW_API v1bit gui_is_mouse_enabled();
	NW_API v1bit gui_is_docks_enabled();
	NW_API v1bit gui_is_viewp_enabled();
	// core_functions
	NW_API v1bit gui_init(window_handle wndh, device_handle dvch, context_handle ctxh);
	NW_API v1bit gui_quit();
	NW_API v1nil gui_begin_frame();
	NW_API v1nil gui_end_frame();
}
#endif	// NW_API
#endif	// NW_GUI_API_H
