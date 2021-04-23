#include "nw_app_pch.hpp"
#include "nw_app_states.h"
#if (defined NW_API)
#	include "nw_app_engine.h"
#	include "nw_app_gui_of.h"
#	include "core/nw_gfx_data.h"
#	include "nw_app_delaunay.h"
namespace NW
{
	app_state_game::app_state_game() :
		app_state(type_info::get_name<app_state_game>())
	{
	}
	app_state_game::~app_state_game() { }

	// --==<core_methods>==--
	v1bit app_state_game::init()
	{
		gfx_engine::get().set_vsync(0u);

		// component creation
		if constexpr (NW_TRUE) {
			// 3d cube;
			if constexpr (NW_FALSE) {
				// buffer layout;
				if constexpr (NW_FALSE) {
					auto& vlayt = cmp_sys::get().new_ref<gfx_buf_layt>().get_val<mem_layt>();
					vlayt.add_node<v3f>("vsi_vtx_crd");
					vlayt.add_node<v2f>("vsi_txr_crd");
					vlayt.add_node<v3f>("vsi_nrm_crd");
					auto& vbuf = cmp_sys::get().new_ref<gfx_buf_vtx>().get_val<mem_buf>();
					auto& ibuf = cmp_sys::get().new_ref<gfx_buf_idx>().get_val<mem_buf>();
					vbuf.remake(vlayt, 24u, vtx_cube_3f2f3f);
					ibuf.remake(t_mem_layt<v1u>(), 36u, idx_cube);
					NW_CHECK(vlayt.remake(), "failed remake", return NW_FALSE);
					if constexpr (NW_FALSE) {
						for (v1u itr = 0u; itr < 24u; itr++) {
							auto& ivtx = vbuf[itr];
							ivtx[0u] = v3f{ vtx_cube_3f2f3f[itr * 8 + 0], vtx_cube_3f2f3f[itr * 8 + 1], vtx_cube_3f2f3f[itr * 8 + 2] };
							ivtx[1u] = v2f{ vtx_cube_3f2f3f[itr * 8 + 3], vtx_cube_3f2f3f[itr * 8 + 4] };
							ivtx[2u] = v3f{ vtx_cube_3f2f3f[itr * 8 + 5], vtx_cube_3f2f3f[itr * 8 + 6], vtx_cube_3f2f3f[itr * 8 + 7] };
						}
						for (v1u itr = 0u; itr < 36u; itr++) { ibuf[0u] = idx_cube[itr]; }
					}
					else if constexpr (NW_FALSE) {
						vbuf[0u].set_elem(0u, v3f{ -0.5f, -0.5f, 0.0f }, v2f{ 0.00f, 0.00f }, v3f{ 0.0f, 0.0f, -1.0f });
						vbuf[1u].set_elem(0u, v3f{ -0.5f, +0.5f, 0.0f }, v2f{ 0.00f, 1.00f }, v3f{ 0.0f, 0.0f, -1.0f });
						vbuf[2u].set_elem(0u, v3f{ +0.5f, +0.5f, 0.0f }, v2f{ 1.00f, 1.00f }, v3f{ 0.0f, 0.0f, -1.0f });
						vbuf[3u].set_elem(0u, v3f{ +0.5f, -0.5f, 0.0f }, v2f{ 1.00f, 0.00f }, v3f{ 0.0f, 0.0f, -1.0f });
						ibuf[0u] = 0u;
						ibuf[1u] = 1u;
						ibuf[2u] = 2u;
						ibuf[3u] = 2u;
						ibuf[4u] = 3u;
						ibuf[5u] = 0u;
					}
					vbuf.set_data();
					ibuf.set_data();
				}
			}
			// voronoi diagram;
			if constexpr (NW_TRUE) {
				// buffer layout;
				if constexpr (NW_TRUE) {
					auto& layt = cmp_sys::get().new_ref<gfx_buf_layt>().get_val<mem_layt>();
					layt.add_node<v2f>("vsi_vtx_crd");
					layt.add_node<v2f>("vsi_txr_crd");
					auto& vbuf = cmp_sys::get().new_ref<gfx_buf_vtx>().get_val<mem_buf>();
					auto& ibuf = cmp_sys::get().new_ref<gfx_buf_idx>().get_val<mem_buf>();
					NW_CHECK(vbuf.remake(layt, 4u, NW_NULL), "failed remake!", return NW_FALSE);
					NW_CHECK(ibuf.remake(t_mem_layt<v1u>("index"), 6u, NW_NULL), "failed remake!", return NW_FALSE);
					NW_CHECK(layt.remake(), "failed remake!", return NW_FALSE);
					if constexpr (NW_TRUE) {
						vbuf[0u].set_elem(0u, v2f{ -1.0f, -1.0f }, v2f{ 0.00f, 0.00f });
						vbuf[1u].set_elem(0u, v2f{ -1.0f, +1.0f }, v2f{ 0.00f, 1.00f });
						vbuf[2u].set_elem(0u, v2f{ +1.0f, +1.0f }, v2f{ 1.00f, 1.00f });
						vbuf[3u].set_elem(0u, v2f{ +1.0f, -1.0f }, v2f{ 1.00f, 0.00f });
						vbuf.set_data();
						ibuf[0u] = 0u;
						ibuf[1u] = 1u;
						ibuf[2u] = 2u;
						ibuf[3u] = 2u;
						ibuf[4u] = 3u;
						ibuf[5u] = 0u;
						ibuf.set_data();
					}
				}
				// material;
				if constexpr (NW_TRUE) {
					auto& gmtl = cmp_sys::get().new_ref<gfx_mtl>();
					auto& vshd = cmp_sys::get().new_ref<a_gfx_shd, gfx_shd_vtx>();
					auto& pshd = cmp_sys::get().new_ref<a_gfx_shd, gfx_shd_pxl>();
					gmtl->add_shd(vshd);
					gmtl->add_shd(pshd);
					vshd->set_source(R"(nw_gfx/src_glsl/voronoi_2d_vtx.glsl)");
					pshd->set_source(R"(nw_gfx/src_glsl/voronoi_2d_pxl.glsl)");
					NW_CHECK(vshd->remake(), "failed remake!", return NW_FALSE);
					NW_CHECK(pshd->remake(), "failed remake!", return NW_FALSE);
					NW_CHECK(gmtl->remake(), "failed remake!", return NW_FALSE);
				}
			}
			// camera
			if constexpr (NW_FALSE) {
				cmp_sys::get().new_ref<gfx_cam_lad>();
			}
		}
		// configs
		if constexpr (NW_FALSE) {
			glEnable(GL_DEPTH_TEST);
		}
		
		return NW_TRUE;
	}
	v1bit app_state_game::quit()
	{
		return NW_TRUE;
	}
	v1nil app_state_game::update()
	{
		auto timer = app_engine::get().get_timer();
		auto cursor = app_engine::get().get_cursor();
		auto keybod = app_engine::get().get_keybod();
		// default 3d scene
		if constexpr (NW_FALSE) {
			auto& camera = cmp_sys::get().get_ref<gfx_cam_lad>(NW_NULL).get_val();
			auto& sbuf = cmp_sys::get().get_ref<gfx_buf_shd>(NW_NULL).get_val<gfx_buf_shd>();

			// camera transform
			camera.on_draw(keybod, cursor, timer);
			sbuf["cst_view"] = camera.get_view();
			sbuf["cst_proj"] = camera.get_proj();
			// model transform
			if constexpr (NW_TRUE) {
				sbuf["cst_model"] = m4f::make_ident()
					* m4f::make_scale(v3f(1.5f))
					* m4f::make_rotat_xyz({ timer->get_curr(10.0f), timer->get_curr(10.0f), timer->get_curr(10.0f) })
					;
			}
			if constexpr (NW_FALSE) {
				sbuf["cst_model"] = m4f::make_ident()
					* m4f::make_scale<1u>(camera.get_ratio())
					;
			}
			// data setting
			static_cast<mem_buf&>(sbuf).set_data(NW_NULL);
			// draw command
			gfx_engine::get().get_cmd_buf().add_cmd(gfx_cmd(NW_GFX_CMD_IDX, NW_GFX_PRIM_TRIANGLES,
				t_sarray<a_gfx_cmp*, 3u> {
				cmp_sys::get().get_ref<gfx_mtl>(1u),
					cmp_sys::get().get_ref<gfx_buf_layt>(1u),
					cmp_sys::get().get_ref<gfx_buf_idx>(3u),
			}
			));
		}
		// voronoi diagram
		if constexpr (NW_TRUE) {
			gfx_engine::get().get_cmd_buf().add_cmd(gfx_cmd(NW_GFX_CMD_IDX, NW_GFX_PRIM_TRIANGLES,
				t_sarray<a_gfx_cmp*, 3u> {
				cmp_sys::get().get_ref<gfx_mtl>(0u),
					cmp_sys::get().get_ref<gfx_buf_layt>(0u),
					cmp_sys::get().get_ref<gfx_buf_idx>(1u),
			}
			));
			glUseProgram(3u);
			glUniform1f(0u, timer->get_curr());
		}
	}

	v1nil app_state_game::event_proc(iop_event_t& evt)
	{
		if (evt.has_sort(NW_EVSORT_KEYBOD)) {
			iop_event_kbd& kbd_evt = evt;
			switch (evt.get_type()) {
			case NW_EVTYPE_KEYBOD_RAISE:
				switch (kbd_evt.get_code()) {
				case NW_KEYCODE_M: {
					auto& camera = cmp_sys::get().get_ref<gfx_cam_lad>(NW_NULL).get_val();
					if (!app_engine::get().is_cursor_enabled()) {
						app_engine::get().set_cursor_enabled(NW_TRUE);
						camera.set_mode(NW_CAMERA_2D);
					}
					else {
						app_engine::get().set_cursor_enabled(NW_FALSE);
						camera.set_mode(NW_CAMERA_3D);
					}
					break;
				}
				}
				break;
			}
		}
		else if (evt.has_sort(NW_EVSORT_WINDOW)) {
			iop_event_wnd_t& wnd_evt = evt;
			switch (wnd_evt.get_type()) {
			case NW_EVTYPE_WINDOW_SIZED: {
				if (cmp_sys::get().has_ref<gfx_cam_lad>(NW_NULL)) {
					auto& camera = cmp_sys::get().get_ref<gfx_cam_lad>(NW_NULL).get_val();
					camera.set_ratio(wnd_evt.get_val_x(), wnd_evt.get_val_y());
				}
				break;
			}
			}
		}
		else if (evt.has_sort(NW_EVSORT_APPLIC)) {
			iop_event_app_t& app_evt = static_cast<iop_event_app_t& > (evt);
			switch (app_evt.get_type()) {
			case NW_EVTYPE_APPLIC_DROPF: {
				if (strstr(app_evt.get_desc(), ".bmp") != NW_NULL) { }
				break;
			}
			}
		}
	}
	// --==</core_methods>==--
}
namespace NW
{
	app_state_gui::app_state_gui() :
		app_state(type_info::get_name<app_state_gui>())
	{
	}
	app_state_gui::~app_state_gui()
	{
	}
	// --==<core_methods>==--
	v1bit app_state_gui::init()
	{
		NW_CHECK(gui_init(gfx_engine::get().get_context()), "failed init", return NW_FALSE);

		gui_ref ref;
		
		ref.make_ref<gui_of_app_engine>();
		ref->set_enabled(NW_TRUE);
		m_gui_refs.push_back(ref);
		
		ref.make_ref<gui_of_io_sys>();
		ref->set_enabled(NW_TRUE);
		m_gui_refs.push_back(ref);

		ref.make_ref<gui_of_gfx_engine>();
		ref->set_enabled(NW_TRUE);
		m_gui_refs.push_back(ref);

		return NW_TRUE;
	}
	v1bit app_state_gui::quit()
	{
		gui_quit();

		return NW_TRUE;
	}

	v1nil app_state_gui::update()
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

	v1nil app_state_gui::event_proc(iop_event_t& evt)
	{
		if (evt.has_type(NW_EVTYPE_WINDOW_SIZED)) {
			iop_event_wnd_t& wnd_evt = static_cast<iop_event_wnd_t&>(evt);
		}
	}
	// --==</core_methods>==--
}
#endif	// NW_API