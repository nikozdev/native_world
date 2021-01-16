#ifndef NATIVE_WORLD_HPP
#define NATIVE_WORLD_HPP
/// For usage by CheerNik applications

// --==<core_engine>==--
#include <core/nw_core_engine.h>
#include <core/nw_core_state.h>
#include <core/nw_data_res.h>
// --==</core_engine>==--

// --==<lua_engine>==--
#include <lua/nw_lua_engine.h>
// --==</lua_engine>==--

// --==<graph_engine>==--
#include <glib/core/nw_gengine.h>
#include <glib/gcontext/nw_framebuf.h>
#include <glib/gcontext/nw_gcontext.h>
#include <glib/gcontext/nw_window.h>
#include <glib/nw_drawable.h>
#include <glib/vision/nw_shader.h>
#include <glib/vision/nw_gmaterial.h>
#include <glib/vision/nw_gcamera.h>
#include <glib/vision/nw_gcamera_lad.h>
// --==</graph_engine>==--

// --==<systems>==--
#include <sys/nw_ev_sys.h>
#include <sys/nw_io_sys.h>
#include <sys/nw_log_sys.h>
#include <sys/nw_time_sys.h>
#include <sys/nw_mem_sys.h>
#include <sys/nw_data_sys.h>
#include <sys/nw_gui_sys.h>
// --==</systems>==--

// --==<entity_component_system>==--
#include <ecs/nw_scene.h>
#include <ecs/nw_entity.h>
#include <ecs/nw_entity_cmp.h>
#include <ecs/nw_script_cmp.h>
#include <ecs/nw_transform_cmp.h>
#include <ecs/nw_graphics_cmp.h>
#include <ecs/nw_physics_cmp.h>
#include <ecs/nw_collider_cmp.h>
// --==</entity_component_system>==--

#endif // CHEER_NIK_HPP