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
	vtx_3f vtx_cube_3f[] = {
		{ { -1.0f,  1.0f, -1.0f } },
		{ { -1.0f, -1.0f, -1.0f } },
		{ {  1.0f, -1.0f, -1.0f } },
		{ {  1.0f, -1.0f, -1.0f } },
		{ {  1.0f,  1.0f, -1.0f } },
		{ { -1.0f,  1.0f, -1.0f } },

		{ { -1.0f, -1.0f,  1.0f } },
		{ { -1.0f, -1.0f, -1.0f } },
		{ { -1.0f,  1.0f, -1.0f } },
		{ { -1.0f,  1.0f, -1.0f } },
		{ { -1.0f,  1.0f,  1.0f } },
		{ { -1.0f, -1.0f,  1.0f } },

		{  { 1.0f, -1.0f, -1.0f } },
		{  { 1.0f, -1.0f,  1.0f } },
		{  { 1.0f,  1.0f,  1.0f } },
		{  { 1.0f,  1.0f,  1.0f } },
		{  { 1.0f,  1.0f, -1.0f } },
		{  { 1.0f, -1.0f, -1.0f } },

		{ { -1.0f, -1.0f,  1.0f } },
		{ { -1.0f,  1.0f,  1.0f } },
		{ {  1.0f,  1.0f,  1.0f } },
		{ {  1.0f,  1.0f,  1.0f } },
		{ {  1.0f, -1.0f,  1.0f } },
		{ { -1.0f, -1.0f,  1.0f } },

		{ { -1.0f,  1.0f, -1.0f } },
		{ {  1.0f,  1.0f, -1.0f } },
		{ {  1.0f,  1.0f,  1.0f } },
		{ {  1.0f,  1.0f,  1.0f } },
		{ { -1.0f,  1.0f,  1.0f } },
		{ { -1.0f,  1.0f, -1.0f } },

		{ { -1.0f, -1.0f, -1.0f } },
		{ { -1.0f, -1.0f,  1.0f } },
		{ {  1.0f, -1.0f, -1.0f } },
		{ {  1.0f, -1.0f, -1.0f } },
		{ { -1.0f, -1.0f,  1.0f } },
		{ {  1.0f, -1.0f,  1.0f } },
	};
	vtx_3f2f3f vtx_cube_3f2f3f[] = {
		// back
		{ { -1.0f,	+1.0f,	-1.0f },	{ 0.00f,	1.00f } },
		{ { -1.0f,	-1.0f,	-1.0f },	{ 0.00f,	0.00f } },
		{ { +1.0f,	+1.0f,	-1.0f },	{ 1.00f,	1.00f } },
		{ { +1.0f,	-1.0f,	-1.0f },	{ 1.00f,	0.00f } },
		{ { +1.0f,	+1.0f,	-1.0f },	{ 1.00f,	1.00f } },
		{ { -1.0f,	+1.0f,	-1.0f },	{ 0.00f,	1.00f } },
		// left
		{ { -1.0f,	-1.0f,	+1.0f },	{ 0.00f,	1.00f } },
		{ { -1.0f,	-1.0f,	-1.0f },	{ 0.00f,	0.00f } },
		{ { -1.0f,	+1.0f,	-1.0f },	{ 1.00f,	0.00f } },
		{ { -1.0f,	+1.0f,	-1.0f },	{ 1.00f,	0.00f } },
		{ { -1.0f,	+1.0f,  +1.0f },	{ 1.00f,	1.00f } },
		{ { -1.0f,	-1.0f,  +1.0f },	{ 0.00f,	1.00f } },
		// right
		{ { +1.0f,	-1.0f,	-1.0f },	{ 0.00f,	0.00f } },
		{ { +1.0f,	-1.0f,	+1.0f },	{ 0.00f,	1.00f } },
		{ { +1.0f,	+1.0f,	+1.0f },	{ 1.00f,	1.00f } },
		{ { +1.0f,	+1.0f,	+1.0f },	{ 1.00f,	1.00f } },
		{ { +1.0f,	+1.0f,	-1.0f },	{ 1.00f,	0.00f } },
		{ { +1.0f,	-1.0f,	-1.0f },	{ 0.00f,	0.00f } },
		// front
		{ { -1.0f,	-1.0f,	+1.0f },	{ 0.00f,	0.00f } },
		{ { -1.0f,	+1.0f,	+1.0f },	{ 0.00f,	1.00f } },
		{ { +1.0f,	+1.0f,	+1.0f },	{ 1.00f,	1.00f } },
		{ { +1.0f,	+1.0f,	+1.0f },	{ 1.00f,	1.00f } },
		{ { +1.0f,	-1.0f,	+1.0f },	{ 1.00f,	0.00f } },
		{ { -1.0f,	-1.0f,	+1.0f },	{ 0.00f,	0.00f } },
		// top
		{ { -1.0f,	+1.0f,	-1.0f },	{ 0.00f,	0.00f } },
		{ { +1.0f,	+1.0f,	-1.0f },	{ 1.00f,	0.00f } },
		{ { +1.0f,	+1.0f,	+1.0f },	{ 1.00f,	1.00f } },
		{ { +1.0f,	+1.0f,	+1.0f },	{ 1.00f,	1.00f } },
		{ { -1.0f,	+1.0f,	+1.0f },	{ 0.00f,	1.00f } },
		{ { -1.0f,	+1.0f,	-1.0f },	{ 0.00f,	0.00f } },
		// bottom
		{ { -1.0f,	-1.0f,	-1.0f },	{ 0.00f,	0.00f } },
		{ { -1.0f,	-1.0f,	+1.0f },	{ 0.00f,	1.00f } },
		{ { +1.0f,	-1.0f,	-1.0f },	{ 1.00f,	0.00f } },
		{ { +1.0f,	-1.0f,	-1.0f },	{ 1.00f,	0.00f } },
		{ { -1.0f,	-1.0f,	+1.0f },	{ 0.00f,	1.00f } },
		{ { +1.0f,	-1.0f,	+1.0f },	{ 1.00f,	1.00f } },
	};
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
		
		//if (!init_skybox()) { return false; }
		if (!init_scene()) { return false; }
		
		return true;
	}
	void gfx_state::quit()
	{
	}
	void gfx_state::update()
	{
		m_camera_lad.update(*m_core->get_keyboard() , *m_core->get_cursor(), *m_core->get_timer());
		//draw_skybox();
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
			case KC_1: m_gfx->set_primitive(GPT_TRIANGLES); break;
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
	inline bool gfx_state::init_scene() {
		mem_ref<drawable_vtx> drb;
		m_gfx->new_rsc<drawable_vtx>(drb);

		mem_ref<buf_vtx> vbuf;
		m_gfx->new_rsc<buf_vtx>(vbuf);
		vbuf->remake<vtx_3f2f3f>(std::size(vtx_cube_3f2f3f), &vtx_cube_3f2f3f[0]);
		drb->get_bufs().push_back(vbuf);

		dstring str_buf = "default";

		mem_ref<a_shader> vshd;
		m_gfx->new_rsc<a_shader, shader_vtx>(vshd, "shd_default_3d_vtx");
		if (!data_sys::load_file(R"(F:\projects\native_world\nw_gfx\src_glsl\default_3d_vtx.glsl)", str_buf)) { return false; }
		if (!vshd->remake(&str_buf[0])) { return false; }

		mem_ref<a_shader> pshd;
		m_gfx->new_rsc<a_shader, shader_pxl>(pshd, "shd_default_3d_pxl");
		if (!data_sys::load_file(R"(F:\projects\native_world\nw_gfx\src_glsl\default_3d_pxl.glsl)", str_buf)) { return false; }
		if (!pshd->remake(&str_buf[0])) { return false; }

		mem_ref<a_texture> txr;
		m_gfx->new_rsc<a_texture, texture_2d>(txr, "txr_nw_logo");
		image_bmp img("nw_logo.bmp");
		if (!data_sys::load_file(R"(F:\projects\native_world\data\image\nw_logo.bmp)", img)) { return false; }
		if (!txr->remake(img)) { return false; }
		
		mem_ref<gfx_material> gmtl;
		m_gfx->new_rsc<gfx_material>(gmtl, "gmt_default_3d");
		gmtl->add_shader(vshd);
		gmtl->add_shader(pshd);
		gmtl->add_texture(txr);
		if (!gmtl->remake()) { return false; }
		drb->add_rsc(gmtl);
		
		mem_ref<sampler> smp;
		m_gfx->new_rsc<sampler>(smp);
		drb->add_rsc(smp);

		return true;
	}
	inline bool gfx_state::init_skybox() {
		mem_ref<drawable_vtx> drb;
		m_gfx->new_rsc<drawable_vtx>(drb);

		drb->get_bufs().push_back(mem_ref<buf_vtx>());
		m_gfx->new_rsc<buf_vtx>(drb->get_bufs().back());

		mem_ref<gfx_material> gmtl;
		m_gfx->new_rsc<gfx_material>(gmtl, "gmt");
		drb->add_rsc(gmtl);

		mem_ref<a_texture> txr;
		image_bmp img("nw_skybox.bmp");
		if (!data_sys::load_file(R"(F:\projects\native_world\data\image\nw_sb.bmp)", img)) { return false; }
		m_gfx->new_rsc<a_texture, texture_cube>(txr, "txr_skybox_0");
		if (!txr->remake(img)) { return false; }

		return true;
	}
	inline void gfx_state::draw_scene() {
		static buf_16f16f16f unf_tform;

		unf_tform.view = m_camera_lad.get_view_mat();
		unf_tform.proj = m_camera_lad.get_proj_mat();

		auto& shd_rsc = m_gfx->get_rsc<shader_vtx>(0);
		auto* vshd = shd_rsc.get_ref<shader_vtx>();
		auto& sbuf = vshd->get_buffer(0);
		sbuf->on_draw();
		sbuf->set_data(sizeof(buf_16f16f16f), &unf_tform);

		auto& drb = m_gfx->get_rsc<drawable_vtx>(0);
		drb->on_draw();
	}
	inline void gfx_state::draw_skybox() {
		static buf_16f16f unf_tform;

		unf_tform.view = m4f( m3f(m_camera_lad.get_view_mat()) );
		unf_tform.proj = m_camera_lad.get_proj_mat();

		auto& shd = m_gfx->get_rsc<shader_vtx>(1);
		auto* vshd = shd.get_ref<shader_vtx>();
		auto& sbuf = vshd->get_buffer(1);
		sbuf->on_draw();
		sbuf->set_data(sizeof(buf_16f16f), &unf_tform);

		auto& drb = m_gfx->get_rsc<drawable_vtx>(1);
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
		if (!NW::imgui_init(m_core->get_window()->get_native(), m_core->get_graphics()->get_device(), m_core->get_graphics()->get_context())) { return false; }
		
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
		NW::imgui_quit();
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