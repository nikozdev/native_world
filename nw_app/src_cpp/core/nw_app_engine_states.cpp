#include "nw_app_pch.hpp"
#include "nw_app_engine_states.h"

#include "nw_app_engine.h"
#include "nw_app_gui_of.h"

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
		m_cam_lad(gfx_cam_lad())
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
		m_gfx->set_clear_color(get_rand(0.0f, 1.0f), get_rand(0.0f, 1.0f), get_rand(0.0f, 1.0f), 1.0f);

		for (v1u itr = 0u; itr < 5u; itr++) {
			m_gfx->new_ent<gfx_ent_cube>(get_rand(0u, 2u))
				->set_crd({ get_rand(-1.0f, +1.0f), get_rand(-3.0f, +3.0f), get_rand(-3.0f, +3.0f) })
				.set_rtn({ get_rand(-180.0f, 180.0f), get_rand(-180.0f, 180.0f), get_rand(-180.0f, 180.0f) })
				.set_scl({ get_rand(0.05f, 1.0f), get_rand(0.05f, 1.0f), get_rand(0.05f, 1.0f) });
		}

		return true;
	}
	void gfx_core_state::quit()
	{
		m_gfx = nullptr;
	}
	void gfx_core_state::update()
	{
		v1u cmp_count = 0u;
		for (auto& itabs : m_gfx->get_cmp_reg()) { cmp_count += itabs.second.size(); }
		const static dstr title = m_core->get_window()->get_title();
		sbyte title_buf[256];
		sprintf_s(
			&title_buf[0], 256, "%s||ups:%d||ents:%zd||cmps:%d",
			&title[0],
			static_cast<v1s>(m_core->get_time_ups()),
			m_gfx->get_ent_tab<a_gfx_ent>().size(), cmp_count
		);
		m_core->get_window()->set_title(&title_buf[0]);

		m_cam_lad.update(m_core->get_keyboard(), m_core->get_mouse(), m_core->get_timer());
		
		buf_m4fm4fm4f buf_tform;
		buf_tform.model = m4f::make_ident();
		buf_tform.view = m_cam_lad.get_view(),
		buf_tform.proj = m_cam_lad.get_proj();
		
		mem_ref<gfx_buf_shd> sbuf = m_gfx->get_cmp<gfx_buf_shd>(0);
		sbuf->set_data(&buf_tform);
		if constexpr (true) {
			for (auto& icube : m_gfx->get_ent_tab<a_gfx_ent>()) {
				v1u eid = static_cast<v1u>(icube.second.get_ref<a_ent>()->get_id());
				//icube.second.get_ref<gfx_ent_mesh>()->set_rtn(v3f{
				//	NW_MATH_SIN(m_core->get_time_curr()), 0.0f, NW_MATH_COS(m_core->get_time_curr())
				//	});
			}
		}
		if constexpr (true) {
			for (auto& ient : m_gfx->get_ent_tab<a_gfx_ent>()) {
				sbuf->set_data(&ient.second.get_ref<gfx_ent_mesh>()->get_tform());
				ient.second->on_draw();
			}
		}
	}

	void gfx_core_state::event_proc(a_event& evt)
	{
		if (evt.is_in_category(EVC_KEYBOARD)) {
			kbd_event& kbd_evt = static_cast<kbd_event&>(evt);
			switch (evt.type) {
			case EVT_KBD_RELEASED:
				switch (kbd_evt.code) {
				case KBC_1: m_gfx->set_prim(PRIM_TRIANGLES); break;
				case KBC_2: m_gfx->set_prim(PRIM_TRIANGLE_STRIP); break;
				case KBC_M:
					if (!m_core->is_cursor_enabled()) {
						m_core->set_cursor_enabled(true);
						m_cam_lad.set_mode(CAM_2D);
					}
					else {
						m_core->set_cursor_enabled(false);
						m_cam_lad.set_mode(CAM_3D);
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
				m_cam_lad.set_ratio(wnd_evt.val_x, wnd_evt.val_y);
				break;
			}
			case EVT_WND_MOVE: { break; }
			}
		}
		else if (evt.is_in_category(EVC_APPLICATION)) {
			app_event& app_evt = static_cast<app_event&>(evt);
			switch (app_evt.type) {
			case EVT_APP_DROP_FILE: {
				if (strstr(app_evt.desc, ".bmp") != NW_NULL) {
					img_bmp img;
					if (!io_sys::get().load_file(app_evt.desc, img)) { return; }
					v1u gfx_txrid = 0;
					std::cin >> gfx_txrid;
					if (!m_gfx->get_cmp<a_gfx_txr>(gfx_txrid).get_ref<a_gfx_txr>()->remake(img)) { throw run_error(__FILE__, __LINE__); }
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
		if (!NW_GUI::gui_init(
			m_core->get_window()->get_handle(),
			m_core->get_graphics()->get_dvch(),
			m_core->get_graphics()->get_ctxh())
			) { throw init_error(__FILE__, __LINE__); return false; }

		gui_ref ref;
		
		ref.make_ref<gui_of_core_engine>(*m_core);
		ref->set_enabled(true);
		m_gui_refs.push_back(ref);
		
		ref.make_ref<gui_of_io_sys>();
		ref->set_enabled(true);
		m_gui_refs.push_back(ref);
		
		ref.make_ref<gui_of_gfx_engine>(*m_core->get_graphics());
		ref->set_enabled(true);
		m_gui_refs.push_back(ref);

		return true;
	}
	void gui_core_state::quit()
	{
		NW_GUI::gui_quit();
	}
	void gui_core_state::update() {
		NW_GUI::gui_begin_frame();

		if (NW_GUI::BeginMenuBar()) {
			if (NW_GUI::BeginMenu("view")) {
				for (auto& igui : m_gui_refs) { igui->draw_checkbox(); }
				NW_GUI::EndMenu();
			}
			NW_GUI::EndMenuBar();
		}
		for (auto& igui_of : m_gui_refs) { igui_of->on_draw(); }

		NW_GUI::gui_end_frame();
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