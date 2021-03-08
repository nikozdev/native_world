#include <nw_pch.hpp>
#include <core/nw_engine_states.h>

#include <core/nw_engine.h>
#include <core/nw_gui_of.h>

namespace NW
{
	a_engine_state::a_engine_state(core_engine& engine) :
		m_core(&engine) { }
	a_engine_state::~a_engine_state() { }
}
namespace NW
{
	game_state::game_state(core_engine& engine) :
		a_engine_state(engine) { }
	game_state::~game_state() { }

	// --==<core_methods>==--
	bool game_state::init() { return true; }
	void game_state::quit() { }
	void game_state::update() { }

	void game_state::event_proc(cursor_event& evt) { }
	void game_state::event_proc(keyboard_event& evt) { }
	void game_state::event_proc(window_event& evt) { }
	// --==</core_methods>==--
}
namespace NW
{
	gfx_state::gfx_state(core_engine& rEngine) :
		a_engine_state(rEngine),
		m_gfx(m_core->get_graphics()),
		m_camera_lad() { }
	gfx_state::~gfx_state() { }
	// --==<core_methods>==--
	bool gfx_state::init() {
		m_gfx = m_core->get_graphics();
		
		gfx_mesh_data<vtx_3f2f3f, ui32> gmd;
		// back
		gmd.add_vertex(vtx_3f2f3f{ { -0.5f,	-0.5f,	-0.5f },	{ 0.00f,	0.00f } });
		gmd.add_vertex(vtx_3f2f3f{ { -0.5f,	+0.5f,	-0.5f },	{ 0.00f,	1.00f } });
		gmd.add_vertex(vtx_3f2f3f{ { +0.5f,	+0.5f,	-0.5f },	{ 1.00f,	1.00f } });
		gmd.add_vertex(vtx_3f2f3f{ { +0.5f,	-0.5f,	-0.5f },	{ 1.00f,	0.00f } });
		// bottom
		gmd.add_vertex(vtx_3f2f3f{ { +0.5f,	-0.5f,	+0.5f },	{ 1.00f,	1.00f } });
		gmd.add_vertex(vtx_3f2f3f{ { -0.5f,	-0.5f,	+0.5f },	{ 1.00f,	0.00f } });
		// left
		gmd.add_vertex(vtx_3f2f3f{ { -0.5f,	+0.5f,	+0.5f },	{ 1.00f,	1.00f } });
		gmd.add_vertex(vtx_3f2f3f{ { -0.5f,	+0.5f,	-0.5f },	{ 0.00f,	1.00f } });
		gmd.update_indices(GPT_TRIANGLES);
		// front
		gmd.add_vertex(vtx_3f2f3f{ { +0.5f,	+0.5f,	+0.5f },	{ 1.00f,	1.00f } });
		gmd.add_vertex(vtx_3f2f3f{ { +0.5f,	-0.5f,	+0.5f },	{ 1.00f,	0.00f } });
		gmd.add_vertex(vtx_3f2f3f{ { -0.5f,	-0.5f,	+0.5f },	{ 0.00f,	0.00f } });
		gmd.add_vertex(vtx_3f2f3f{ { -0.5f,	+0.5f,	+0.5f },	{ 0.00f,	1.00f } });
		// top
		gmd.add_vertex(vtx_3f2f3f{ { -0.5f,	+0.5f,	-0.5f },	{ 0.00f,	0.00f } });
		gmd.add_vertex(vtx_3f2f3f{ { +0.5f,	+0.5f,	-0.5f },	{ 0.00f,	1.00f } });
		// right
		gmd.add_vertex(vtx_3f2f3f{ { +0.5f,	-0.5f,	-0.5f },	{ 0.00f,	0.00f } });
		gmd.add_vertex(vtx_3f2f3f{ { +0.5f,	-0.5f,	+0.5f },	{ 1.00f,	0.00f } });
		gmd.update_indices(GPT_TRIANGLES);

		mem_ref<idx_drawable> drb;
		m_gfx->new_res<idx_drawable>(drb);

		m_gfx->new_res<idx_buf>(drb->get_buf());
		drb->get_buf()->remake<ui32>(gmd.get_idx_count(), gmd.get_idx_data());
		
		mem_ref<vtx_buf> vbuf;
		m_gfx->new_res<vtx_buf>(vbuf);
		vbuf->remake<vtx_3f2f3f>(gmd.get_vtx_count(), gmd.get_vtx_data());
		drb->add_res(vbuf);

		mem_ref<gfx_material> gmtl;
		m_gfx->new_res<gfx_material>(gmtl, "gmt_default_3d");
		if (!gmtl->load_file(R"(D:\dev\native_world\data\material\default_3d.gmt)")) { return false; }
		drb->add_res(gmtl);
		
		mem_ref<a_texture> tex;
		m_gfx->new_res<a_texture, texture2d>(tex, "txr_nw_logo");
		if (!tex->load_file(R"(D:\dev\native_world\data\image\nw_logo.bmp)")) { return false; }

		gmtl->get_shader(SHD_PIXEL)->get_texture(0).set_ref(tex);

		return true;
	}
	void gfx_state::quit()
	{
	}
	void gfx_state::update()
	{
		m_camera_lad.update(*m_core->get_keyboard() , *m_core->get_cursor(), *m_core->get_timer());
		draw_scene();
	}

	void gfx_state::event_proc(cursor_event& evt)
	{
	}
	void gfx_state::event_proc(keyboard_event& evt)
	{
		switch (evt.type) {
		case EVT_KEYBOARD_RELEASED:
			switch (evt.code) {
			//case KC_1: m_gfx->set_primitive(GPT_TRIANGLES); break;
			//case KC_2: m_gfx->set_primitive(GPT_TRIANGLE_STRIP); break;
			//case KC_3: m_gfx->set_primitive(GPT_TRIANGLE_FAN); break;
			//case KC_4: m_gfx->set_primitive(GPT_LINE_STRIP); break;
			//case KC_5: m_gfx->set_primitive(GPT_LINES); break;
			//case KC_6: m_gfx->set_primitive(GPT_LINE_LOOP); break;
			//case KC_7: m_gfx->set_primitive(GPT_POINTS); break;
			}
			break;
		default: break;
		}
	}
	void gfx_state::event_proc(window_event& evt) {
		switch (evt.type) {
		case EVT_WINDOW_RESIZE: {
			v4si viewport = m_gfx->get_configs().viewport;
			viewport[2] = evt.val_x;
			viewport[3] = evt.val_y;
			m_gfx->set_viewport(viewport[0], viewport[1], viewport[2], viewport[3]);
			m_camera_lad.aspect_ratio = static_cast<f32>(viewport[2] - viewport[0]) / static_cast<f32>(viewport[3] - viewport[1]);
			break;
		}
		case EVT_WINDOW_MOVE: { break; }
		default: break;
		}
	}
	// --==</core_methods>==--

	// --==<implementation_methods>==--
	inline void gfx_state::draw_scene() {
		static f32 angle = 0.0f;
		static f32 time_delta = 0.0f;
		static f32 time_curr = 0.0f;
		static buf_m4fm4fm4f unf_tform;
		auto& viewport = m_core->get_graphics()->get_configs().viewport;

		time_delta = m_core->get_timer()->get_delta();
		time_curr = m_core->get_timer()->get_curr();
		angle = sinf(time_curr);

		unf_tform.view = m_camera_lad.get_view_mat();
		unf_tform.proj = m_camera_lad.get_proj_mat();

		auto& shd = m_gfx->get_res<vtx_shader>(0);
		auto* vshd = shd.get_ref<vtx_shader>();
		auto& sbuf = vshd->get_buffer(0);
		sbuf->on_draw();
		sbuf->set_data(sizeof(buf_m4fm4fm4f), &unf_tform);

		m_gfx->get_configs().clear_color.g = angle;
		m_gfx->get_configs().clear_color.b = angle;

		auto& drb = m_gfx->get_res<idx_drawable>(0);
		drb->on_draw();
	}
	// --==</implementation_methods>==--
}
namespace NW
{
	gui_state::gui_state(core_engine& rEngine) :
		a_engine_state(rEngine),
		m_enable_dockspace(true), m_full_screen_persist(true),
		m_gui_context(nullptr), m_gui_io(nullptr), m_gui_style(nullptr) { }
	gui_state::~gui_state() { }
	// --==<core_methods>==--
	bit gui_state::init()
	{
		if (!NWG::imgui_init(m_core->get_window()->get_native(), m_core->get_graphics()->get_device(), m_core->get_graphics()->get_context())) { return false; }
		
		m_gui_of.resize(4);
		m_gui_of[0].make_ref<gui_of_core_engine>();
		m_gui_of[1].make_ref<gui_of_gfx_engine>();
		m_gui_of[2].make_ref<gui_of_cmd_engine>();
		m_gui_of[3].make_ref<gui_of_data_sys>();
		
		m_gui_of[0]->m_is_enabled = true;
		m_gui_of[1]->m_is_enabled = true;
		m_gui_of[3]->m_is_enabled = true;

		return true;
	}
	void gui_state::quit()
	{
		NWG::imgui_quit();
	}
	void gui_state::update() {
		imgui_begin_frame();

		if (GUI::BeginMenuBar()) {
			if (GUI::BeginMenu("view")) {
				for (auto& igui_of : m_gui_of) {
					GUI::Checkbox(igui_of->get_name(), &igui_of->m_is_enabled);
				}
				GUI::EndMenu();
			}
			GUI::EndMenuBar();
		}
		for (auto& igui_of : m_gui_of) {
			igui_of->on_draw();
		}

		imgui_end_frame();
	}

	void gui_state::event_proc(cursor_event& rmEvt) { }
	void gui_state::event_proc(keyboard_event& rkEvt) { }
	void gui_state::event_proc(window_event& rwEvt) { }
	// --==</core_methods>==--
}