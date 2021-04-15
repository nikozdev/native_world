#include "nw_app_pch.hpp"
#include "nw_app_engine_states.h"

#include "nw_app_engine.h"
#include "nw_app_gui_of.h"
#include "core/nw_gfx_data.h"

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
	v1bit game_core_state::init()
	{
		return NW_TRUE;
	}
	v1nil game_core_state::quit()
	{
	}
	v1nil game_core_state::update() { }

	v1nil game_core_state::event_proc(a_event& evt) { }
	// --==</core_methods>==--
}
namespace NW
{
	gfx_core_state::gfx_core_state(core_engine& engine) :
		a_core_state(engine),
		m_gfx(NW_NULL),
		m_cam_lad(gfx_cam_lad())
	{
	}
	gfx_core_state::~gfx_core_state()
	{
	}
	// --==<core_methods>==--
	v1bit gfx_core_state::init()
	{
		m_gfx = m_core->get_graphics();
		m_gfx->set_swap_delay(0u);
		m_gfx->set_clear_color(get_rand(0.0f, 1.0f), get_rand(0.0f, 1.0f), get_rand(0.0f, 1.0f), 1.0f);
		if constexpr (NW_TRUE) {
			if constexpr (NW_TRUE) {
				auto& s_vbuf = m_gfx->new_cmp<gfx_buf_vtx>();
				NW_CHECK(s_vbuf.get_ref<a_gfx_buf>()->remake(vtx_quad_3f2f3f), "failed remake", return NW_FALSE);
				
				auto& s_ibuf = m_gfx->new_cmp<gfx_buf_idx>();
				NW_CHECK(s_ibuf.get_val<a_gfx_buf>().remake(idx_quad), "failed remake", return NW_FALSE);
#		if (NW_FALSE)
				s_vbuf->add_elem(v3f{ -0.5f, -0.5f, +0.0f }, v2f{ 0.0f, 0.0f }, v3f{ +0.0f, +0.0f, -1.0f });
				s_vbuf->add_elem(v3f{ -0.5f, +0.5f, +0.0f }, v2f{ 0.0f, 1.0f }, v3f{ +0.0f, +0.0f, -1.0f });
				s_vbuf->add_elem(v3f{ +0.5f, +0.5f, +0.0f }, v2f{ 1.0f, 1.0f }, v3f{ +0.0f, +0.0f, -1.0f });
				s_vbuf->add_elem(v3f{ +0.5f, -0.5f, +0.0f }, v2f{ 1.0f, 0.0f }, v3f{ +0.0f, +0.0f, -1.0f });
#		elif (NW_FALSE)
				s_vbuf->get_elem(0u).set<v3f>(v3f{ -0.5f, -0.5f, 0.0f });
				s_vbuf->get_elem(1u).set<v3f>(v3f{ -0.5f, +0.5f, 0.0f });
				s_vbuf->get_elem(2u).set<v3f>(v3f{ +0.5f, +0.5f, 0.0f });
				s_vbuf->get_elem(3u).set<v3f>(v3f{ +0.5f, -0.5f, 0.0f });
#		elif (NW_FALSE)
				auto& lay = s_vbuf->get_layt();
				lay["vti_vtx_crd"] = v3f{ -1.0f, -1.0f, 0.0f };
				lay["vti_txr_crd"] = v2f{ 0.0f, 0.0f };
				lay["vti_nrm_crd"] = v3f{ -0.0f, -0.0f, 0.0f };
				lay.moveto(1u);
				lay["vti_vtx_crd"] = v3f{ -1.0f, +1.0f, 0.0f };
				lay["vti_txr_crd"] = v2f{ 0.0f, 1.0f };
				lay["vti_nrm_crd"] = v3f{ -0.0f, +0.0f, 0.0f };
				lay.moveto(1u);
				lay["vti_vtx_crd"] = v3f{ +1.0f, +1.0f, 0.0f };
				lay["vti_txr_crd"] = v2f{ 1.0f, 1.0f };
				lay["vti_nrm_crd"] = v3f{ +0.0f, +0.0f, 0.0f };
				lay.moveto(1u);
				lay["vti_vtx_crd"] = v3f{ +1.0f, -1.0f, 0.0f };
				lay["vti_txr_crd"] = v2f{ 1.0f, 0.0f };
				lay["vti_nrm_crd"] = v3f{ +0.0f, -0.0f, 0.0f };
				lay.moveto(1u);
				s_vbuf->update();
#		endif
				auto& s_layt = m_gfx->new_cmp<gfx_buf_layt>();
				s_layt->add_node<v3f>("vsi_vtx_crd");
				s_layt->add_node<v2f>("vsi_txr_crd");
				s_layt->add_node<v3f>("vsi_nrm_crd");
				s_layt->add_vbuf(s_vbuf);
				s_layt->set_ibuf(s_ibuf);
				NW_CHECK(s_layt->remake(), "failed remake", return NW_FALSE);
				
				auto& s_sbuf = m_gfx->new_cmp<gfx_buf_shd>();
				s_sbuf->add_node<m4f>("cst_model");
				s_sbuf->add_node<m4f>("cst_view");
				s_sbuf->add_node<m4f>("cst_proj");
				NW_CHECK(s_sbuf->remake(sizeof(m4f) * 3u, 1u), "failed remake", return NW_FALSE);
				s_sbuf->get_node("cst_model") = m4f::make_ident();
				s_sbuf->get_node("cst_view") = m4f::make_ident();
				s_sbuf->get_node("cst_proj") = m4f::make_ident();
				s_sbuf->set_data(1u, s_sbuf->get_bytes(), NW_NULL);
				
				auto& s_vshd = m_gfx->new_cmp<a_gfx_shd, gfx_shd_vtx>();
				NW_CHECK(s_vshd->remake(shd_default_3d_vtx), "failed remake", return NW_FALSE);
				auto& s_pshd = m_gfx->new_cmp<a_gfx_shd, gfx_shd_pxl>();
				NW_CHECK(s_pshd->remake(shd_default_3d_pxl), "failed remake", return NW_FALSE);
				
				auto& s_mtl = m_gfx->new_cmp<gfx_mtl>();
				s_mtl->add_shd(s_vshd);
				s_mtl->add_shd(s_pshd);
				NW_CHECK(s_mtl->remake(), "failed remake", return NW_FALSE);
				s_mtl->set_buf(0u, s_sbuf, 0u);
			}
		}
		return NW_TRUE;
	}
	v1nil gfx_core_state::quit()
	{
		m_gfx = NW_NULL;
	}

	v1nil gfx_core_state::update()
	{
		m_cam_lad.update(m_core->get_keyboard(), m_core->get_mouse(), m_core->get_timer());

		auto sbuf = m_gfx->get_cmp<gfx_buf_shd>(2u);
		sbuf->get_node("cst_view") = m_cam_lad.get_view();
		sbuf->get_node("cst_proj") = m_cam_lad.get_proj();
		sbuf->set_data(1u, NW_NULL);
		m_gfx->get_cmp<gfx_mtl>(0u)->on_draw();
		
		m_gfx->get_cmd_buf().add_cmd(
			gfx_cmd_idx(m_gfx->get_cmp<gfx_buf_idx>(1u).get_ref<gfx_buf_idx>(),
			m_gfx->get_cmp<gfx_buf_vtx>(0u).get_ref<gfx_buf_vtx>(),
			1u, NW_PRIM_TRIANGLES)
		);
	}

	v1nil gfx_core_state::event_proc(a_event& evt)
	{
		if (evt.is_in_category(EVC_KEYBOARD)) {
			kbd_event& kbd_evt = static_cast<kbd_event&>(evt);
			switch (evt.type) {
			case EVT_KBD_RELEASED:
				switch (kbd_evt.code) {
				case KBC_M:
					if (!m_core->is_cursor_enabled()) {
						m_core->set_cursor_enabled(NW_TRUE);
						m_cam_lad.set_mode(NW_CAMERA_2D);
					}
					else {
						m_core->set_cursor_enabled(false);
						m_cam_lad.set_mode(NW_CAMERA_3D);
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
				if (strstr(app_evt.desc, ".bmp") != NW_NULL) { }
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
	v1bit gui_core_state::init()
	{
		if (!gui_init(
			m_core->get_window()->get_handle(),
			m_core->get_graphics()->get_dvch(),
			m_core->get_graphics()->get_ctxh())
			) { throw init_error(__FILE__, __LINE__); return false; }

		gui_ref ref;
		
		ref.make_ref<gui_of_core_engine>(*m_core);
		ref->set_enabled(NW_TRUE);
		m_gui_refs.push_back(ref);
		
		ref.make_ref<gui_of_io_sys>();
		ref->set_enabled(NW_TRUE);
		m_gui_refs.push_back(ref);

		ref.make_ref<gui_of_gfx_engine>(*m_core->get_graphics());
		ref->set_enabled(NW_TRUE);
		m_gui_refs.push_back(ref);

		return NW_TRUE;
	}
	v1nil gui_core_state::quit()
	{
		gui_quit();
	}

	v1nil gui_core_state::update()
	{
		gui_begin_frame();

		if (NW_GUI::BeginMenuBar()) {
			if (NW_GUI::BeginMenu("view")) {
				for (auto& igui : m_gui_refs) { igui->draw_checkbox(); }
				NW_GUI::EndMenu();
			}
			NW_GUI::EndMenuBar();
		}
		for (auto& igui_of : m_gui_refs) { igui_of->on_draw(); }

		gui_end_frame();
	}

	v1nil gui_core_state::event_proc(a_event& evt)
	{
		if (evt.type == EVT_WND_RESIZE) {
			wnd_event& wnd_evt = static_cast<wnd_event&>(evt);
			m_core->get_graphics()->set_fbuf_size(wnd_evt.val_x, wnd_evt.val_y);
		}
	}
	// --==</core_methods>==--
}