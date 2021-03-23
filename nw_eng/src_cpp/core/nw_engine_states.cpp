#include <nw_pch.hpp>
#include <core/nw_engine_states.h>

#include <core/nw_engine.h>
#include <core/nw_gui_of.h>

namespace NW
{
	a_core_state::a_core_state(core_engine& engine) :
		m_core(&engine)
	{
	}
	a_core_state::~a_core_state()
	{
	}
}
namespace NW
{
	game_core_state::game_core_state(core_engine& engine) :
		a_core_state(engine) { }
	game_core_state::~game_core_state() { }

	// --==<core_methods>==--
	bool game_core_state::init()
	{
		return true;
	}
	void game_core_state::quit()
	{
	}
	void game_core_state::update() { }

	void game_core_state::event_proc(a_event& evt) { }
	// --==</core_methods>==--
}
namespace NW
{
	gfx_core_state::gfx_core_state(core_engine& engine) :
		a_core_state(engine),
		m_gfx(nullptr),
		m_cam_lad(camera_lad())
	{
	}
	gfx_core_state::~gfx_core_state()
	{
	}
	// --==<core_methods>==--
	bool gfx_core_state::init()
	{
		m_gfx = m_core->get_graphics();
		m_gfx->set_swap_delay(0u);

		m_gfx->new_ent<gfx_ent_cube>();

		return true;
	}
	void gfx_core_state::quit()
	{
		m_gfx = nullptr;
	}
	void gfx_core_state::update()
	{
		m_cam_lad.update(m_core->get_keyboard(), m_core->get_mouse(), m_core->get_timer());
		
		const static dstr title = m_core->get_window()->get_title();
		sbyte title_buf[256];
		sprintf_s(&title_buf[0], 256, "%s||ups:%d||ents:%zd||", &title[0], static_cast<v1si>(m_core->get_time_ups()), m_gfx->get_ent_tab<a_gfx_ent>().size());
		m_core->get_window()->set_title(&title_buf[0]);

		buf_16f16f16f buf_tform;
		buf_tform.view = m_cam_lad.get_view(),
			buf_tform.proj = m_cam_lad.get_proj();
		
		mem_ref<buf_shd> sbuf = m_gfx->get_cmp<buf_shd>(0);
		mem_ref<a_gfx_ent> cube = m_gfx->get_ent_ref<a_gfx_ent>(0);
		if constexpr (true) {
			buf_tform.model = m4f(1.0f) * mat_move(m4f(1.0f), v3f{ +3.0f, 0.0f, 0.0f });
			sbuf->set_data(sizeof(buf_tform), &buf_tform);
			cube->on_draw();
			buf_tform.model = m4f(1.0f) * mat_move(m4f(1.0f), v3f{ -3.0f, 0.0f, 0.0f });
			sbuf->set_data(sizeof(buf_tform), &buf_tform);
			cube->on_draw();
			
			buf_tform.model = m4f(1.0f) * mat_move(m4f(1.0f), v3f{ 0.0f, +3.0f, 0.0f });
			sbuf->set_data(sizeof(buf_tform), &buf_tform);
			cube->on_draw();
			buf_tform.model = m4f(1.0f) * mat_move(m4f(1.0f), v3f{ 0.0f, -3.0f, 0.0f });
			sbuf->set_data(sizeof(buf_tform), &buf_tform);
			cube->on_draw();

			buf_tform.model = m4f(1.0f) * mat_move(m4f(1.0f), v3f{ 0.0f, 0.0f, +3.0f });
			sbuf->set_data(sizeof(buf_tform), &buf_tform);
			cube->on_draw();
			buf_tform.model = m4f(1.0f) * mat_move(m4f(1.0f), v3f{ 0.0f, 0.0f, -3.0f });
			sbuf->set_data(sizeof(buf_tform), &buf_tform);
			cube->on_draw();
		}
		if (m_core->get_mouse()->is_held(MSC_1)) {
			buf_tform.model = mat_move(m4f(1.0f), m_cam_lad.get_front_dir(2.0f)) * m_cam_lad.get_tform();
			m_gfx->get_cmp<buf_shd>(0).get_ref<buf_shd>()->set_data(sizeof(buf_16f16f16f), &buf_tform);
			m_gfx->get_ent<a_gfx_ent>(0)->on_draw();
		}
	}

	void gfx_core_state::event_proc(a_event& evt)
	{
		if (evt.is_in_category(EVC_KEYBOARD)) {
			kbd_event& kbd_evt = static_cast<kbd_event&>(evt);
			switch (evt.type) {
			case EVT_KBD_RELEASED:
				switch (kbd_evt.code) {
				case KBC_1: m_gfx->set_primitive(GPT_TRIANGLES); break;
				case KBC_2: m_gfx->set_primitive(GPT_TRIANGLE_STRIP); break;
				case KBC_M:
					if (m_core->is_cursor_enabled()) {
						m_core->set_cursor_enabled(false);
						m_cam_lad.set_mode(GCM_3D);
						m_cam_lad.set_type(GCT_PERSPECT);
					}
					else {
						m_core->set_cursor_enabled(true);
						m_cam_lad.set_mode(GCM_2D);
						m_cam_lad.set_type(GCT_ORTHO);
					}
					break;
				}
				break;
			}
		}
		else if (evt.is_in_category(EVC_WINDOW)) {
			wnd_event& wnd_evt = static_cast<wnd_event&>(evt);
			switch (wnd_evt.type) {
			case EVT_WND_RESIZE: {
				v4si viewp = m_gfx->get_viewport();
				viewp[0] = 0;
				viewp[1] = 0;
				viewp[2] = wnd_evt.val_x;
				viewp[3] = wnd_evt.val_y;
				m_gfx->set_viewport(viewp);
				m_cam_lad.set_ratio(viewp[2] - viewp[0], viewp[3] - viewp[1]);
				break;
			}
			case EVT_WND_MOVE: { break; }
			}
		}
		else if (evt.is_in_category(EVC_APPLICATION)) {
			app_event& app_evt = static_cast<app_event&>(evt);
			switch (app_evt.type) {
			case EVT_APP_DROP_FILE: {
				cstr file_format = str_part_right(app_evt.desc, '.');
				if (str_is_equal(file_format, ".bmp")) {
					img_bmp img;
					if (!data_sys::get().load_file(app_evt.desc, img)) { return; }
					v1ui txr_id = 0;
					std::cin >> txr_id;
					if (!m_gfx->get_cmp<a_txr>(txr_id).get_ref<a_txr>()->remake(img)) { throw run_error(__FILE__, __LINE__); }
				}
				break;
			}
			}
		}
	}
	// --==</core_methods>==--
}
namespace NW
{
	gui_core_state::gui_core_state(core_engine& core) :
		a_core_state(core)
	{
	}
	gui_core_state::~gui_core_state()
	{
	}
	// --==<core_methods>==--
	bool gui_core_state::init()
	{
		if (!gui_init(
			m_core->get_window()->get_handle(),
			m_core->get_graphics()->get_dvch(),
			m_core->get_graphics()->get_ctxh())
			) { throw init_error(__FILE__, __LINE__); return false; }

		gui_ref ref;
		
		ref.make_ref<gui_of_core_engine>(*m_core);
		ref->set_enabled(true);
		m_gui_refs.push_back(ref);
		
		ref.make_ref<gui_of_data_sys>();
		ref->set_enabled(true);
		m_gui_refs.push_back(ref);
		
		ref.make_ref<gui_of_gfx_engine>(*m_core->get_graphics());
		ref->set_enabled(true);
		m_gui_refs.push_back(ref);

		return true;
	}
	void gui_core_state::quit()
	{
		gui_quit();
	}
	void gui_core_state::update() {
		gui_begin_frame();

		if (GUI::BeginMenuBar()) {
			if (GUI::BeginMenu("view")) {
				for (auto& igui : m_gui_refs) { igui->draw_checkbox(); }
				GUI::EndMenu();
			}
			GUI::EndMenuBar();
		}
		for (auto& igui_of : m_gui_refs) { igui_of->on_draw(); }

		gui_end_frame();
	}

	void gui_core_state::event_proc(a_event& evt)
	{
		if (evt.type == EVT_WND_RESIZE) {
			wnd_event& wnd_evt = static_cast<wnd_event&>(evt);
			m_core->get_graphics()->set_fbuf_size(wnd_evt.val_x, wnd_evt.val_y);
		}
	}
	// --==</core_methods>==--
}