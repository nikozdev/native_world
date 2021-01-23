#ifndef NATIVE_WORLD_HPP
#define NATIVE_WORLD_HPP
/// For usage by CheerNik applications

// --==<global>==--
#include <core/nw_core_engine.h>
#include <core/nw_core_state.h>
#include <core/nw_window.h>
#include <glib/core/nw_gengine.h>
#include <glib/core/nw_glayer.h>
#include <glib/core/nw_gapi.h>
#include <lua/nw_lua_engine.h>
#include <sys/nw_io_sys.h>
#include <sys/nw_log_sys.h>
#include <sys/nw_time_sys.h>
#include <sys/nw_data_sys.h>
#include <sys/nw_gui_sys.h>
// --==</core_engine>==--

// --==<graphics_library>==--
#include <glib/gcontext/nw_framebuf.h>
#include <glib/gcontext/nw_gcontext.h>
#include <glib/nw_drawable.h>
#include <glib/vision/nw_shader.h>
#include <glib/vision/nw_gmaterial.h>
#include <glib/vision/nw_gcamera.h>
#include <glib/vision/nw_gcamera_lad.h>
// --==</graphics_library>==--

// --==<entity_component_system>==--
// --==</entity_component_system>==--

#endif // CHEER_NIK_HPP