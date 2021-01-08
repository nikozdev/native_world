#ifndef NATIVE_WORLD_HPP
#define NATIVE_WORLD_HPP
/// For usage by CheerNik applications

// --==<engine_core>==--
#include <core/nw_core_engine.h>
#include <core/nw_core_state.h>
#include <core/nw_data_res.h>
// --==</engine_core>==--

// --==<systems>==--
#include <sys/nw_ev_sys.h>
#include <sys/nw_io_sys.h>
#include <sys/nw_time_sys.h>
#include <sys/nw_mem_sys.h>
#include <sys/nw_data_sys.h>
#include <sys/nw_gui_sys.h>
#include <sys/nw_rand_sys.h>
// --==</systems>==--

// --==<graphics_library>==--
#include <gl/control/nw_gapi.h>
#include <gl/control/nw_draw_engine.h>

#include <gl/gcontext/nw_window.h>
#include <gl/gcontext/nw_gcontext.h>

#include <gl/vision/nw_shader.h>
#include <gl/vision/nw_gmaterial.h>
#include <gl/vision/nw_gcamera.h>

#include <gl/gcontext/nw_framebuf.h>
#include <gl/render/nw_texture.h>
#include <gl/render/nw_drawable.h>
// --==</graphics_library>==--

// --==<entity_component_system>==--
#include <ecs/nw_scene.h>
#include <ecs/nw_entity.h>
#include <ecs/nw_entity_cmp.h>
// --==</entity_component_system>==--

#endif // CHEER_NIK_HPP