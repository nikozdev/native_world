#ifndef NATIVE_WORLD_HPP
#define NATIVE_WORLD_HPP
/// For usage by applications

// --==<global>==--
#include <core/nw_core_engine.h>
#include <core/nw_core_state.h>
#include <core/nw_window.h>
#include <sys/nw_io_sys.h>
#include <sys/nw_log_sys.h>
#include <sys/nw_time_sys.h>
#include <sys/nw_data_sys.h>
// --==</global>==--

// --==</graphics_library>==--
#include <glib/nw_gapi.h>
#include <glib/nw_gtools.h>
#include <glib/nw_gbuffer.h>
#include <glib/nw_framebuf.h>
#include <glib/nw_shader.h>
#include <glib/nw_texture.h>
#include <glib/nw_gmaterial.h>
#include <glib/nw_gcamera.h>
#include <glib/nw_gcamera_lad.h>
// --==</graphics_library>==--

// --==<lua_library>==--
#include <lua/nw_lua_engine.h>
// --==</lua_library>==--

#endif // NATIVE_WORLD_HPP