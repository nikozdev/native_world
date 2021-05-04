#include "nw_app_pch.hpp"
#include "nw_app_states.h"
#if (defined NW_API)
#	include "nw_app_engine.h"
#	include "nw_app_gui_of.h"
#	include "core/nw_gfx_data.h"
#	include "num/nw_num_rect.h"
#	include "num/nw_num_circ.h"
#	include "num/nw_num_voron.h"
namespace NW
{
	inline gfx_img make_pxl_voron(cv2u size_xy, cv1u scount) {
		gfx_img image;
		image.remake(t_mem_layt<v4u08>("pixel"), size_xy);
		darray_t<v2f> sites(scount, NW_NULL);
		for (auto& isite : sites) { isite = v2f::make_rand(0.0f, 1.0f); }
		for (v1u ipxl = 0u; ipxl < image.get_size(); ipxl++) {
			v2f pxl_crd = v2f{
				NW_CAST_FLOAT(ipxl % image.get_size_x()),
				NW_CAST_FLOAT(ipxl / image.get_size_x())
			};
			pxl_crd[0] = (pxl_crd[0] == NW_NULL) ? pxl_crd[0] : pxl_crd[0] / NW_CAST_FLOAT(image.get_size_x());
			pxl_crd[1] = (pxl_crd[1] == NW_NULL) ? pxl_crd[1] : pxl_crd[1] / NW_CAST_FLOAT(image.get_size_y());
			v1u08 min_dst = static_cast<v1u08>(pxl_crd.get_near_idx(sites) * 255.0f / NW_CAST_FLOAT(scount));
			//v1u08 min_dst = static_cast<v1u08>(pxl_crd.get_near_idx(sites) * 255.0f / NW_CAST_FLOAT(scount));
			image[ipxl] = v4u08{ min_dst, min_dst, min_dst, 255u };
		}
		return image;
	}
}
namespace NW
{
	app_state_game::app_state_game() : app_state(type_info::get_name<app_state_game>()) { }
	app_state_game::~app_state_game() { }
	// --==<core_methods>==--
	v1bit app_state_game::init()
	{
		// component creation
		if constexpr (NW_TRUE) {
			// transformations
			if constexpr (NW_TRUE) {
				// cam_tform
				if constexpr (NW_TRUE) {
					auto& camera = cmp_sys::get().new_ref<gfx_tform_cam>();
					camera->set_slot(0u);
					camera->remake();
				}
			}
			// buffers;
			if constexpr (NW_TRUE)
			{
				// 3default
				if constexpr (NW_TRUE) {
					constexpr const size_tc dcount = 3u;
					auto& layt = cmp_sys::get().new_ref<gfx_buf_layt>().get_val();
					layt.get_layt().add_node<v3f>("vsi_vtx_crd").add_node<v2f>("vsi_txr_crd").add_node<v3f>("vsi_nrm_crd");
					auto& vbuf = cmp_sys::get().new_ref<gfx_buf_vtx>(layt, NW_NUM_POW(32u, dcount - 1u)).get_val();
					auto& ibuf = cmp_sys::get().new_ref<gfx_buf_idx>(t_mem_layt<v1u>("index"), NW_NUM_POW(32, dcount)).get_val();
					// d_rect
					if constexpr (NW_FALSE)
					{
						using rect_t = d_rect_t<dcount>;
						rect_t rect(rect_t::vec_t(0.0f), rect_t::vec_t(1.0f));
						for (v1u ivert(0u); ivert < rect.get_vcount(); ivert++) {
							auto icrd = rect.make_vert(rect, ivert);
							v3f vtx_crd = icrd;
							v2f txr_crd = ((icrd - rect.m_center) / rect.m_halfsz + 1.0f) / 2.0f; // convert from vertex to texture coordinate;
							v3f nrm_crd = (icrd - rect.m_center) / rect.m_halfsz;
							for (v1u idim(3u); idim < dcount; idim++) {
								vtx_crd[0u] += icrd[idim] / NW_CAST_FLOAT(dcount - 1u) * (idim % 2 != 0 ? +1.0f : -1.0f);
								vtx_crd[1u] += icrd[idim] / NW_CAST_FLOAT(dcount - 1u) * (idim % 2 != 0 ? +1.0f : -1.0f);
								vtx_crd[2u] += icrd[idim] / NW_CAST_FLOAT(dcount - 1u) * (idim % 2 != 0 ? +1.0f : -1.0f);
								nrm_crd = vtx_crd;
							}
							vbuf[ivert][0u].set<v3f>(vtx_crd);
							vbuf[ivert][1u].set<v2f>(txr_crd);
							vbuf[ivert][2u].set<v3f>(nrm_crd);
						}
						//auto idata = rect.get_idata_trig(0u);
						auto idata = rect.get_idata_trig(0u);
						for (v1u idx(0u); idx < idata.size(); idx++) { ibuf[idx].set<v1u>(idata[idx]); }
					}
					// d_circ
					if constexpr (NW_TRUE) {
						using circ_t = d_circ_t<dcount>;
						circ_t circ(circ_t::vert_t(0.0f), circ_t::vert_t(1.0f));
						for (v1u ivert(0u); ivert < vbuf.get_count(); ivert++) {
							auto icrd = circ.make_vert(circ, vbuf.get_count(), ivert);
							v3f vtx_crd = icrd;
							v2f txr_crd = ((icrd - circ.m_center) / circ.m_halfsz + 1.0f) / 2.0f; // convert from vertex to texture coordinate;
							v3f nrm_crd = (icrd - circ.m_center) / circ.m_halfsz;
							for (v1u idim(3u); idim < dcount; idim++) {
								vtx_crd[0u] += icrd[idim] / NW_CAST_FLOAT(dcount - 1u) * (idim % 2 != 0 ? +1.0f : -1.0f);
								vtx_crd[1u] += icrd[idim] / NW_CAST_FLOAT(dcount - 1u) * (idim % 2 != 0 ? +1.0f : -1.0f);
								vtx_crd[2u] += icrd[idim] / NW_CAST_FLOAT(dcount - 1u) * (idim % 2 != 0 ? +1.0f : -1.0f);
								nrm_crd = vtx_crd;
							}
							//vbuf[ivert].set_elem(0u, vtx_crd, txr_crd, nrm_crd);
							vbuf[ivert][0u].set<v3f>(vtx_crd);
							vbuf[ivert][1u].set<v2f>(txr_crd);
							vbuf[ivert][2u].set<v3f>(nrm_crd);
						}
						auto idata = circ.get_idata_trig(vbuf.get_count(), 0u);
						for (v1u idx(0u); idx < idata.size(); idx++) { ibuf[idx].set<v1u>(idata[idx]); }
					}
					vbuf.set_data();
					ibuf.set_data();
					NW_CHECK(layt.remake(), "failed remake", return NW_FALSE);
				}
				// voronoi;
				if constexpr (NW_FALSE) {
					auto& layt = cmp_sys::get().new_ref<gfx_buf_layt>().get_val();
					layt.get_layt().add_node<v2f>("vsi_vtx_crd").add_node<v2f>("vsi_txr_crd");
					auto& vbuf = cmp_sys::get().new_ref<gfx_buf_vtx>(layt, 4u).get_val<gfx_buf>();
					auto& ibuf = cmp_sys::get().new_ref<gfx_buf_idx>(t_mem_layt<v1u>("index"), vbuf.get_count()).get_val<gfx_buf>();
					if constexpr (NW_TRUE) {
						if constexpr (NW_FALSE) {
							for (v1u ivert = 0u; ivert < vbuf.get_count(); ivert++) {
								cv2f vtx_crd = circ2d_t::make_vert(circ2d_t(v2f{ 0.0f, 0.0f }, v2f{ 1.0f, 1.0f }), vbuf.get_count(), ivert);
								vbuf[ivert][0u] = v2f{ vtx_crd };
								vbuf[ivert][1u] = v2f{ (vtx_crd + 1.0f) / 2.0f };
								ibuf[ivert] = ivert;
							}
						}
						if constexpr (NW_TRUE) {
							for (v1u ivert = 0u; ivert < vbuf.get_count(); ivert++) {
								cv2f vtx_crd = rect2d_t::make_vert(rect2d_t(v2f{ 0.0f, 0.0f }, v2f{ 1.0f, 1.0f }), ivert);
								vbuf[ivert][0u] = v2f{ vtx_crd };
								vbuf[ivert][1u] = v2f{ (vtx_crd + 1.0f) / 2.0f };
							}
							ibuf[0u] = 0u;
							ibuf[1u] = 1u;
							ibuf[2u] = 2u;
							ibuf[3u] = 2u;
							ibuf[4u] = 3u;
							ibuf[5u] = 0u;
						}
					}
					vbuf.set_data();
					ibuf.set_data();
					NW_CHECK(layt.remake(), "remake error!", return NW_FALSE);
				}
			}
			// texture voronoi update;
			if constexpr (NW_FALSE) {
				auto& image = cmp_sys::get().get_ref<gfx_txr>(0u).get_val<gfx_img>();
				image = make_pxl_voron(image.get_size_xy() * 16u, 24u);
			}
			// materials;
			if constexpr (NW_TRUE) {
				// 3default
				if constexpr (NW_TRUE) {
					auto& vshd = cmp_sys::get().new_ref<gfx_shd, gfx_shd_vtx>(R"(nw_gfx\src_glsl\default_3d_vtx.glsl)");
					auto& pshd = cmp_sys::get().new_ref<gfx_shd, gfx_shd_pxl>(R"(nw_gfx\src_glsl\default_3d_pxl.glsl)");
					auto& vshd_bind = cmp_sys::get().new_ref<gfx_bind>(
						vshd,
						gfx_bind::buf_list_tc{ cmp_sys::get().get_ref<gfx_buf_shd>(1u) },
						gfx_bind::txr_list_tc{ },
						gfx_bind::smp_list_tc{ }
					);
					auto& pshd_bind = cmp_sys::get().new_ref<gfx_bind>(
						pshd,
						gfx_bind::buf_list_tc{ },
						gfx_bind::txr_list_tc{ cmp_sys::get().get_ref<gfx_txr>(0u) },
						gfx_bind::smp_list_tc{ cmp_sys::get().get_ref<gfx_smp>(0u) }
					);
					auto& gmtl = cmp_sys::get().new_ref<gfx_mtl>(gfx_mtl::bind_list_t{ vshd_bind, pshd_bind });
				}
				// voronoi;
				if constexpr (NW_FALSE) {
					auto& vshd = cmp_sys::get().new_ref<gfx_shd, gfx_shd_vtx>(R"(nw_gfx/src_glsl/voronoi_2d_vtx.glsl)");
					auto& pshd = cmp_sys::get().new_ref<gfx_shd, gfx_shd_pxl>(R"(nw_gfx/src_glsl/voronoi_2d_pxl.glsl)");
					auto& vshd_bind = cmp_sys::get().new_ref<gfx_bind>(
						vshd,
						gfx_bind::buf_list_tc{ },
						gfx_bind::txr_list_tc{ },
						gfx_bind::smp_list_tc{ }
						);
					auto& pshd_bind = cmp_sys::get().new_ref<gfx_bind>(
						pshd,
						gfx_bind::buf_list_tc{ },
						gfx_bind::txr_list_tc{ },
						gfx_bind::smp_list_tc{ }
					);
					auto& gmtl = cmp_sys::get().new_ref<gfx_mtl>(gfx_mtl::bind_list_tc{ vshd_bind, pshd_bind });
				}
			}
			// render graph;
			if constexpr (NW_TRUE) {
				auto& graph = gfx_engine::get().get_graph();
				graph
					.add_pass(
						gfx_pass(
							gfx_pass::cmd_list_tc{
							gfx_cmd(NW_GFX_CMD_IDX, NW_GFX_PRIM_POINTS,
									{
										cmp_sys::get().get_ref<gfx_buf_idx>(3u),
										cmp_sys::get().get_ref<gfx_buf_layt>(1u),
										cmp_sys::get().get_ref<gfx_mtl>(0u),
										cmp_sys::get().get_ref<gfx_state_depst>(0u)
									}
								),
							gfx_cmd(NW_GFX_CMD_IDX, NW_GFX_PRIM_LINES,
									{
										cmp_sys::get().get_ref<gfx_buf_idx>(3u),
										cmp_sys::get().get_ref<gfx_buf_layt>(1u),
										cmp_sys::get().get_ref<gfx_mtl>(0u),
										cmp_sys::get().get_ref<gfx_state_depst>(0u)
									}
								),
#						if (NW_TRUE)
							gfx_cmd(NW_GFX_CMD_IDX, NW_GFX_PRIM_TRIANGLES,
									{
										cmp_sys::get().get_ref<gfx_buf_idx>(3u),
										cmp_sys::get().get_ref<gfx_buf_layt>(1u),
										cmp_sys::get().get_ref<gfx_mtl>(0u),
										cmp_sys::get().get_ref<gfx_state_depst>(1u)
									}
								),
#						endif
							}
						)
					)
					.set_root(0u)
					;
				//graph.get_pass(0u).add_link(&graph.get_pass(1u));
				//graph.get_pass(1u).add_link(&graph.get_pass(2u));
				NW_CHECK(graph.remake(), "remake error!", return NW_FALSE);
			}
			// configurations
			if constexpr (NW_TRUE) {
				glPointSize(5.0f);
				glLineWidth(2.5f);
				gfx_engine::get().set_vsync(0u);
			}
		}
		
		return NW_TRUE;
	}
	v1bit app_state_game::quit()
	{
		return NW_TRUE;
	}
	v1nil app_state_game::update()
	{
		auto keybod = app_engine::get().get_keybod();
		auto cursor = app_engine::get().get_cursor();
		auto timer = app_engine::get().get_timer();
		// transform update
		if constexpr (NW_TRUE) {
			auto& camera = cmp_sys::get().get_ref<gfx_tform>(1u).get_val<gfx_tform_cam>();
			camera.update(keybod, cursor, timer);
		}
		// title setting
		if constexpr (NW_TRUE) {
			static dstr_t title = app_engine::get().get_window()->get_title();
			char_t buf[256u];
			sprintf_s(buf, "%s||ups:%d||", &title[0], static_cast<v1u>(timer->get_ups()));
			app_engine::get().get_window()->set_title(&buf[0]);
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
					auto& camera = cmp_sys::get().get_ref<gfx_tform_cam>(1u).get_val();
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
				if (cmp_sys::get().has_ref<gfx_tform_cam>(1u)) {
					auto& camera = cmp_sys::get().get_ref<gfx_tform_cam>(1u).get_val();
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
				if (strstr(app_evt.get_desc(), ".bmp")) {
					auto& img = cmp_sys::get().get_ref<gfx_txr_2d>(NW_NULL).get_val<gfx_img>();
					op_fstream_t stm(&app_evt.get_desc()[0], std::ios_base::out | std::ios_base::binary);
					stm << img;
					stm.close();
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