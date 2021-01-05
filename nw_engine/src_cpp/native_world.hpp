#ifndef NATIVE_WORLD_HPP
#define NATIVE_WORLD_HPP
/// For usage by CheerNik applications

// ========<Application>========
#include <core/nw_engine.h>
#include <core/nw_engine_state.h>
#include <core/nw_data_res.h>
// ========</Application>========

// ========<Systems>========
#include <sys/nw_ev_sys.h>
#include <sys/nw_io_sys.h>
#include <sys/nw_time_sys.h>
#include <sys/nw_mem_sys.h>
#include <sys/nw_data_sys.h>
#include <sys/nw_gui_sys.h>
#include <sys/nw_rand_sys.h>
// ========</Systems>========

// ========<Graphics Library>========
#include <gl/gcontext/nw_window.h>
#include <gl/gcontext/nw_gcontext.h>

#include <gl/control/nw_drawer.h>
#include <gl/control/nw_gapi.h>

#include <gl/vision/nw_shader.h>
#include <gl/vision/nw_gmaterial.h>
#include <gl/vision/nw_gcamera.h>

#include <gl/gcontext/nw_framebuf.h>
#include <gl/render/nw_texture.h>
#include <gl/render/nw_drawable.h>
// ========</Graphics Library>========

// ========<Scenes&ECS>========
#include <ecs/nw_scene.h>
#include <ecs/nw_entity.h>
#include <ecs/nw_entity_cmp.h>
// ========</Scenes&ECS>========

#endif // CHEER_NIK_HPP