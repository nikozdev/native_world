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
				// 4default
				if constexpr (NW_FALSE) {
					auto& layt = cmp_sys::get().new_ref<gfx_buf_layt>().get_val();
					layt.get_layt().add_node<v3f>("vsi_vtx_crd").add_node<v2f>("vsi_txr_crd").add_node<v3f>("vsi_nrm_crd");
					auto& vbuf = cmp_sys::get().new_ref<gfx_buf_vtx>(layt, 256u).get_val();
					auto& ibuf = cmp_sys::get().new_ref<gfx_buf_idx>(t_mem_layt<v1u>("index"), 1024u).get_val();
					// d_rect
					if constexpr (NW_TRUE) {
						using rect_t = d_rect_t<4u>;
						rect_t rect(rect_t::vert_t(+0.0f), rect_t::vert_t(+1.0f));
						auto vdata = rect.get_vdata();
						for (v1u itr(0u); itr < rect.get_vcount(); itr += 1) {
							auto ivert = vbuf[itr];
							auto icrd = vdata[itr];
							ivert[0u].set<v3f>(v3f{ icrd[0] + icrd[3] + icrd[4], icrd[1] + icrd[3] + icrd[4], icrd[2] + icrd[3] + icrd[4] });
							ivert[1u].set<v2f>(v2f{ icrd[0] * 0.5f + 1.0f, icrd[1] * 0.5f + 1.0f });
							ivert[2u].set<v3f>(v3f{ icrd[0], icrd[1], icrd[2] });
						}
						auto idata = rect.get_idata_wire();
						for (v1u itr(0u); itr < idata.size(); itr++) { ibuf[itr].set<v1u>(idata[itr]); }
					}
					vbuf.set_data();
					ibuf.set_data();
					NW_CHECK(layt.remake(), "remake error!", return NW_FALSE);
				}
				// 3default
				if constexpr (NW_TRUE) {
					auto& layt = cmp_sys::get().new_ref<gfx_buf_layt>().get_val();
					layt.get_layt().add_node<v3f>("vsi_vtx_crd").add_node<v2f>("vsi_txr_crd").add_node<v3f>("vsi_nrm_crd");
					auto& vbuf = cmp_sys::get().new_ref<gfx_buf_vtx>(layt, 256u).get_val();
					auto& ibuf = cmp_sys::get().new_ref<gfx_buf_idx>(t_mem_layt<v1u>("index"), 2048u).get_val();
					// cube_data
					if constexpr (NW_FALSE)
					{
						for (v1u itr = 0u; itr < vbuf.get_count(); itr++) {
							auto ivert = vbuf[itr];
							ivert[0u] = v3f{ vtx_cube_3f2f3f[itr * 8 + 0], vtx_cube_3f2f3f[itr * 8 + 1], vtx_cube_3f2f3f[itr * 8 + 2] };
							ivert[1u] = v2f{ vtx_cube_3f2f3f[itr * 8 + 3], vtx_cube_3f2f3f[itr * 8 + 4] };
							ivert[2u] = v3f{ vtx_cube_3f2f3f[itr * 8 + 5], vtx_cube_3f2f3f[itr * 8 + 6], vtx_cube_3f2f3f[itr * 8 + 7] };
						}
						for (v1u itr = 0u; itr < ibuf.get_count(); itr++) { ibuf[itr] = idx_cube[itr]; }
					}
					// d_rect
					if constexpr (NW_TRUE)
					{
						using rect_t = d_rect_t<4u>;
						rect_t rect(rect_t::vec_t(0.0f), rect_t::vec_t(1.0f));
						for (v1u itr(0u); itr < rect.get_vcount(); itr += 1) {
							auto ivert = vbuf[itr];
							auto icrd = rect.get_vert(itr);
							ivert[0u].set<v3f>(v3f{ icrd[0] + icrd[3] / 2.0f, icrd[1] + icrd[3] / 2.0f, icrd[2] + icrd[3] / 2.0f });
							ivert[1u].set<v2f>(v2f{ icrd[0] * 0.5f + 0.5f, icrd[1] * 0.5f + 0.5f });
							ivert[2u].set<v3f>(v3f{ icrd[0], icrd[1], icrd[2] });
						}
#						if (NW_FALSE)
						// --
						ibuf[2u * 0u + 0u].set<v1u>(0b00u); ibuf[2u * 0u + 1u].set<v1u>(0b01u);
						ibuf[2u * 1u + 0u].set<v1u>(0b00u); ibuf[2u * 1u + 1u].set<v1u>(0b10u);
						// --
						ibuf[2u * 2u + 0u].set<v1u>(0b01u); ibuf[2u * 2u + 1u].set<v1u>(0b10u);
						// --
						ibuf[2u * 3u + 0u].set<v1u>(0b10u); ibuf[2u * 3u + 1u].set<v1u>(0b01u);
#						elif (NW_FALSE)
						// --
						ibuf[2u * 0u + 0u].set<v1u>(0b000u); ibuf[2u * 0u + 1u].set<v1u>(0b001u);
						ibuf[2u * 1u + 0u].set<v1u>(0b000u); ibuf[2u * 1u + 1u].set<v1u>(0b010u);
						ibuf[2u * 2u + 0u].set<v1u>(0b000u); ibuf[2u * 2u + 1u].set<v1u>(0b100u);
						// --
						ibuf[2u * 3u + 0u].set<v1u>(0b111u); ibuf[2u * 3u + 1u].set<v1u>(0b011u);
						ibuf[2u * 4u + 0u].set<v1u>(0b111u); ibuf[2u * 4u + 1u].set<v1u>(0b101u);
						ibuf[2u * 5u + 0u].set<v1u>(0b111u); ibuf[2u * 5u + 1u].set<v1u>(0b110u);
						// --
						ibuf[2u * 6u + 0u].set<v1u>(0b011u); ibuf[2u * 6u + 1u].set<v1u>(0b001u);
						ibuf[2u * 7u + 0u].set<v1u>(0b011u); ibuf[2u * 7u + 1u].set<v1u>(0b010u);
						// --
						ibuf[2u * 8u + 0u].set<v1u>(0b110u); ibuf[2u * 8u + 1u].set<v1u>(0b010u);
						ibuf[2u * 9u + 0u].set<v1u>(0b110u); ibuf[2u * 9u + 1u].set<v1u>(0b100u);
						// --
						ibuf[2u * 10u + 1u].set<v1u>(0b001u); ibuf[2u * 10u + 0u].set<v1u>(0b101u);
						// --
						ibuf[2u * 11u + 1u].set<v1u>(0b100u); ibuf[2u * 11u + 0u].set<v1u>(0b101u);
						// --
#						elif (NW_FALSE)
						v1u last(0u);
						v1u dims = 2u;
						v1u bins = 1 << dims;
						for (v1u idim(0u); idim < dims; idim++) {
							v1u num(0u);
							for (v1u idgt(0u); idgt < idim; idgt++) { num += 1 << idgt; }
							for (v1u inum(0u); inum <= idim; inum++) {
								for (v1u imask(0u); imask < 2u; imask++) {
									v1u dgt = (num >> inum) % 2;
									v1u next = 1u << inum;
									ibuf[last++].set<v1u>(num);
									ibuf[last++].set<v1u>(num + (dgt ? -next : +next));
									num = bins - num;
								}
							}
						}
#						elif (NW_FALSE)
						for (v1u itr(0u); itr < 8u; itr += 1) {
							ibuf[(itr + 0u) * 2u + 0u].set<v1u>((itr + 0u) % 8); ibuf[(itr + 0u) * 2u + 1u].set<v1u>((itr + 1u) % 8);
							ibuf[(itr + 1u) * 2u + 0u].set<v1u>((itr + 0u) % 8); ibuf[(itr + 1u) * 2u + 1u].set<v1u>((itr + 2u) % 8);
							ibuf[(itr + 2u) * 2u + 0u].set<v1u>((itr + 3u) % 8); ibuf[(itr + 2u) * 2u + 1u].set<v1u>((itr + 2u) % 8);
							ibuf[(itr + 3u) * 2u + 0u].set<v1u>((itr + 3u) % 8); ibuf[(itr + 3u) * 2u + 1u].set<v1u>((itr + 1u) % 8);
						}
#						elif (NW_FALSE)
						v1u last(0u);
						for (v1u itr(0u); itr < 8u; itr++) {
							v1u dgt_count;
							dgt_count = get_dgt_count(itr, 2u, 1);
							for (v1u idgt(0u); idgt < dgt_count; idgt++) {
								v1u dgt = (itr >> idgt) % 2;
								ibuf[last++].set<v1u>(itr);
								ibuf[last++].set<v1u>((itr + (!dgt << idgt)) % 8u);
							}
						}
#						elif (NW_FALSE)
						v1u dims(3u), bits(1 << dims), last(0u), num(0u);
						for (v1u idim(0u); idim < dims; idim++) {
							for (v1u itr(0u); itr < dims - idim; itr++) {
								v1u step(1u << itr); v1u temp(num);
								if (step == num) { step = step << (dims - 1u); } // temporary solution
								// set up an edge;
								ibuf[last++].set<v1u>(temp); ibuf[last++].set<v1u>((temp + step) % bits);
								// invert number and set another edge;
								temp = bits - 1u - num;
								ibuf[last++].set<v1u>(temp); ibuf[last++].set<v1u>((temp - step) % bits);
							}
							num = idim + (1 << idim);
						}
#						elif (NW_FALSE)
						v1u last(0u), num(0u), bits(rect3d_t::get_vcount()), dims(rect3d_t::get_count());
						for (v1u idim(0u); idim < (dims - 1u); idim++) {
							v1u temp(dims - 1u);
							v1u bit = num + (1 << idim) % 8;
							while (temp > 0) {
								ibuf[last++].set<v1u>(num); ibuf[last++].set<v1u>(num + bit);
								temp -= 1u;
							}
						}
						num = bits - 1u;
						for (v1u idim(0u); idim < dims; idim++) { ibuf[last++].set<v1u>(num); ibuf[last++].set<v1u>(num - (1u << idim)); }
#						elif (NW_FALSE)
						v1u dims(3u), bits(1u << dims), last(0u);
						for (v1u inum(0u); inum < (bits / 2u); inum += 1u << inum) {
							for (v1u istep(inum); istep < (bits - 1u - inum); istep <<= 1u) {
								v1u temp(inum);
								// set up an edge;
								ibuf[last++].set<v1u>(temp); ibuf[last++].set<v1u>((temp + istep) % bits);
								// invert number and set another edge;
								temp = bits - 1u - inum;
								ibuf[last++].set<v1u>(temp); ibuf[last++].set<v1u>((temp - istep) % bits);
							}
						}
#						elif (NW_FALSE)
						v1u dims(3u), bits(1u << dims), last(0u);
						for (v1u idim(0u); idim < dims; idim += 1u) {
							for (v1u istep(1u << (dims - 1u)), temp(idim); istep > temp; istep = (istep >> 1u), temp = 1u << idim) {
								ibuf[last++].set<v1u>(temp); ibuf[last++].set<v1u>(temp + istep);
								temp = bits - 1u - idim;
								ibuf[last++].set<v1u>(temp); ibuf[last++].set<v1u>(temp - istep);
							}
						}
#						elif (NW_FALSE) // finally it works! but there is an excess; 2d: 4 edges = 8 indicies; 3d: 12 edges = 24 indicies; 4d: 32 edges = 64 indicies;
						v1u dims(3u), bits(1u << dims), last(0u), inum(0u), cmpr(0u);
						for (v1u idim(0u); idim < dims; idim += 1u) {
							for (v1u istep(bits >> 1u), temp(inum); istep > cmpr; istep >>= 1u) {
								ibuf[last++].set<v1u>(temp); ibuf[last++].set<v1u>(temp + istep);
								temp = bits - 1u - temp;
								ibuf[last++].set<v1u>(temp); ibuf[last++].set<v1u>(temp - istep);
							}
							for (v1u istep(1u), temp(inum); istep < cmpr; istep <<= 1u) {
								ibuf[last++].set<v1u>(temp); ibuf[last++].set<v1u>(temp + istep);
								temp = bits - 1u - temp;
								ibuf[last++].set<v1u>(temp); ibuf[last++].set<v1u>(temp - istep);
							}
							inum = 1u << idim;
							cmpr += inum;
						}
						last = 0u;
#						elif (NW_FALSE) // it works with excessful indices;
						v1u dims(2u), bits(1u << dims), last(0u), inum(0u);
						for (v1u idim(0u); idim < dims; idim += 1u) {
							for (v1u istep(bits >> 1u); istep > inum; istep >>= 1u) {
								ibuf[last++].set<v1u>(inum); ibuf[last++].set<v1u>(inum + istep);
								ibuf[last++].set<v1u>(bits - 1u - inum); ibuf[last++].set<v1u>(bits - 1u - inum - istep);
							}
							for (v1u istep(bits >> dims); istep < inum; istep <<= 1u) {
								ibuf[last++].set<v1u>(inum); ibuf[last++].set<v1u>(inum + istep);
								ibuf[last++].set<v1u>(bits - 1u - inum); ibuf[last++].set<v1u>(bits - 1u - inum - istep);
							}
							inum = 1u << idim;
						}
						last = 0u;
#						elif (NW_TRUE)
						auto idata = rect.get_idata_wire();
						for (v1u idx(0u); idx < idata.size(); idx++) { ibuf[idx].set<v1u>(idata[idx]); }
#						elif (NW_TRUE)
						for (v1u itr(0u); itr < vbuf.get_count(); itr++) {
							auto vert = vbuf[itr];
							auto vcrd = circ3d_t::make_vert(circ3d_t(v3f(0.0f), v3f(1.0f)), vbuf.get_count(), itr);
							vert[0u] = v3f{ vcrd[0], vcrd[1], vcrd[2] };
							vert[1u] = v2f{ vcrd[0], vcrd[1] };
							vert[2u] = v3f{ vcrd[0], vcrd[1], vcrd[2] };
						}
						for (v1u itr(0u); itr < vbuf.get_count(); itr++) {
							ibuf[itr * 2u + 0u].set<v1u>((itr + 0u));
							ibuf[itr * 2u + 1u].set<v1u>((itr + 1u));
						}
#						elif (NW_TRUE)
#						endif
					}
					// d_circ
					if constexpr (NW_FALSE) {
						using circ_t = d_circ_t<3u>;
						circ_t circ(circ_t::vert_t(0.0f), circ_t::vert_t(1.0f));
						//circ.m_halfsz = circ_t::vert_t::make_rand(1.0f, 3.0f);
						for (v1u ivert(0u); ivert < vbuf.get_count(); ivert++) {
							auto icrd = circ.make_vert(circ, vbuf.get_count(), ivert);
							if constexpr (circ_t::get_dcount() == 2u) {
								vbuf[ivert][0u].set<v3f>(v3f{ icrd[0], icrd[1], 0.0f });
								vbuf[ivert][1u].set<v2f>(v2f{ icrd[0] * 0.5f + 0.5f, icrd[1] * 0.5f + 0.5f });
								vbuf[ivert][2u].set<v3f>(v3f{ icrd[0], icrd[1], -1.0f });
							}
							if constexpr (circ_t::get_dcount() == 3u) {
								vbuf[ivert][0u].set<v3f>(v3f{ icrd[0], icrd[1], icrd[2] });
								vbuf[ivert][1u].set<v2f>(v2f{ ( icrd[0] + circ.m_halfsz[0] ) / circ.m_halfsz[0] / 2.0f, ( icrd[1] + circ.m_halfsz[1] ) / circ.m_halfsz[1] / 2.0f } );
								vbuf[ivert][2u].set<v3f>(v3f{ icrd[0], icrd[1], icrd[2] });
							}
						}
#					if (NW_FALSE) // simple point list
						for (v1u idx(0u); idx < vbuf.get_count(); idx += 1u) { ibuf[idx] = idx; }
#					elif (NW_FALSE) // a little bit tricky line list
						for (v1u idx(0u); idx < vbuf.get_count() - 1u; idx += 1u) {
							ibuf[idx * 2u + 0u].set<v1u>(idx + 0u);
							ibuf[idx * 2u + 1u].set<v1u>(idx + 1u);
						}
#					elif (NW_FALSE) // hardcore hell of a lot and very hot triangle list
						num_vec_tc<v1u, 2u> sizes = num_vec_tc<v1u, 2u>::make_pows(vbuf.get_count(), 0.5f);
						for (v1u idx(0u); idx < sizes[0] + sizes[1]; idx += 1u) {
							ibuf[idx * 6u + 0u].set<v1u>( ( idx + 0u ) % sizes[0]);
							ibuf[idx * 6u + 1u].set<v1u>( ( idx + 1u ) % sizes[0]);
							ibuf[idx * 6u + 2u].set<v1u>( ( idx + 2u ) % sizes[0]);
							ibuf[idx * 6u + 2u].set<v1u>( ( idx + 3u ) % sizes[0]);
							ibuf[idx * 6u + 3u].set<v1u>( ( idx + 0u ) % sizes[0]);
							ibuf[idx * 6u + 0u].set<v1u>( ( idx + 1u ) % sizes[0]);
						}
#					elif (NW_TRUE) // hardcore hell of a lot and very hot triangle list
						if constexpr (circ_t::get_dcount() == 2u) {
							num_vec_tc<v1u, circ_t::get_dcount() - 1u> sizes = num_vec_tc<v1u, circ_t::get_dcount() - 1u>::make_pows(vbuf.get_count(), 0.5f);
							for (v1u idx(0u); idx < sizes[0]; idx += 1u) {
								ibuf[idx * 3u + 0u].set<v1u>((1000u));
								ibuf[idx * 3u + 1u].set<v1u>((idx + 0u) % sizes[0]);
								ibuf[idx * 3u + 2u].set<v1u>((idx + 1u) % sizes[0]);
							}
						}
						if constexpr (circ_t::get_dcount() == 3u) {
							num_vec_tc<v1u, circ_t::get_dcount()> sizes = num_vec_tc<v1u, circ_t::get_dcount()>::make_pows(vbuf.get_count(), 0.5f);
							for (v1u idx(0u); idx < sizes[0] + sizes[1]; idx += 1u) {
								ibuf[idx * 6u + 0u].set<v1u>((idx + 0u) % sizes[0]);
								ibuf[idx * 6u + 1u].set<v1u>((idx + 1u) % sizes[0]);
								ibuf[idx * 6u + 2u].set<v1u>((idx + sizes[1] + 0u) % sizes[0]);
								ibuf[idx * 6u + 3u].set<v1u>((idx + sizes[1] + 0u) % sizes[0]);
								ibuf[idx * 6u + 4u].set<v1u>((idx + sizes[1] + 1u) % sizes[0]);
								ibuf[idx * 6u + 5u].set<v1u>((idx + 1u) % sizes[0]);
							}
						}
#					endif
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
				// 4default
				if constexpr (NW_FALSE) {
					auto& vshd = cmp_sys::get().new_ref<gfx_shd, gfx_shd_vtx>(R"(nw_gfx/src_glsl/default_4d_vtx.glsl)");
					auto& pshd = cmp_sys::get().new_ref<gfx_shd, gfx_shd_pxl>(R"(nw_gfx/src_glsl/default_4d_pxl.glsl)");
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
#						if (NW_FALSE)
								gfx_cmd(NW_GFX_CMD_VTX, NW_GFX_PRIM_POINTS,
									{
										cmp_sys::get().get_ref<gfx_buf_vtx>(3u),
										cmp_sys::get().get_ref<gfx_buf_layt>(1u),
										cmp_sys::get().get_ref<gfx_mtl>(0u),
										cmp_sys::get().get_ref<gfx_state_depst>(0u)
									}
								),
								gfx_cmd(NW_GFX_CMD_VTX, NW_GFX_PRIM_LINES,
									{
										cmp_sys::get().get_ref<gfx_buf_vtx>(3u),
										cmp_sys::get().get_ref<gfx_buf_layt>(1u),
										cmp_sys::get().get_ref<gfx_mtl>(0u),
										cmp_sys::get().get_ref<gfx_state_depst>(0u)
									}
								),
#						endif
#						if (NW_FALSE)
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
								gfx_cmd(NW_GFX_CMD_IDX, NW_GFX_PRIM_TRIANGLES,
									{
										cmp_sys::get().get_ref<gfx_buf_idx>(3u),
										cmp_sys::get().get_ref<gfx_buf_layt>(1u),
										cmp_sys::get().get_ref<gfx_mtl>(0u),
										cmp_sys::get().get_ref<gfx_state_depst>(1u)
									}
								),
#						elif (NW_TRUE)
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
#						endif
							}
						)
					)
					.add_pass(
						gfx_pass(
							gfx_pass::cmd_list_tc{
#					if (NW_FALSE)
								gfx_cmd(NW_GFX_CMD_IDX, NW_GFX_PRIM_TRIANGLE_FAN,
								{
									cmp_sys::get().get_ref<gfx_buf_idx>(6u),
									cmp_sys::get().get_ref<gfx_buf_layt>(2u),
									cmp_sys::get().get_ref<gfx_mtl>(1u),
									cmp_sys::get().get_ref<gfx_state_depst>(0u)
								}
							),
								gfx_cmd(NW_GFX_CMD_IDX, NW_GFX_PRIM_POINTS,
								{
									cmp_sys::get().get_ref<gfx_buf_idx>(4u),
									cmp_sys::get().get_ref<gfx_buf_layt>(1u),
									cmp_sys::get().get_ref<gfx_mtl>(0u)
								}
							),
#					endif
							}
						)
					)
					.set_root(0u)
					;
				graph.get_pass(0u).add_link(&graph.get_pass(1u));
				//graph.get_pass(1u).add_link(&graph.get_pass(2u));
				NW_CHECK(graph.remake(), "remake error!", return NW_FALSE);
			}
			// configurations
			if constexpr (NW_TRUE) {
				glPointSize(10.0f);
				glLineWidth(5.0f);
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