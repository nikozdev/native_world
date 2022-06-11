#ifndef NW_GUI_GRAPHICS_API_H
#define NW_GUI_GRAPHICS_API_H
#include "nw_gui_core.hpp"
#if (defined NW_GAPI)
#   include "nw_gui_api.h"
namespace NW
{
    // core_functions
    NW_API v1bit gui_gapi_init(device_handle device, context_handle context);
    NW_API v1bit gui_gapi_quit();
    // app_wnd_core_functions
    NW_API v1bit gui_gapi_wnd_init();
    NW_API v1nil gui_gapi_wnd_quit();
    NW_API v1nil gui_gapi_update(ImDrawData* draw_data);
    NW_API v1nil gui_gapi_wnd_create(ImGuiViewport* viewport);
    NW_API v1nil gui_gapi_wnd_delete(ImGuiViewport* viewport);
    // wnd_graphics
    NW_API v1nil gui_gapi_wnd_render(ImGuiViewport* viewport, ptr_t draw_arg);
    NW_API v1nil gui_gapi_wnd_swap(ImGuiViewport* viewport, ptr_t draw_arg);
    // wnd_setters
    NW_API v1nil gui_gapi_wnd_set_size(ImGuiViewport* viewport, ImVec2 size);
#   if (NW_GAPI & NW_GAPI_OGL)
    NW_API v1nil gui_gapi_update_state(ImDrawData* draw_data, v1s fmbuf_size_x, v1s fmbuf_size_y, GLuint varr);
#   endif
#   if (NW_GAPI & NW_GAPI_D3D)
    NW_API v1nil gui_gapi_update_state(ImDrawData* draw_data, context_handle context);
    #endif
}
#endif	// NW_GAPI
#endif	// NW_GUI_GRAPHICS_API_H
