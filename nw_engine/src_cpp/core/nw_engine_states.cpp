#include <nw_pch.hpp>
#include <core/nw_engine_states.h>

#include <core/nw_engine.h>
#include <core/nw_gui_of.h>

#include <data/data_mesh.h>

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

	void game_state::on_event(cursor_event& evt) { }
	void game_state::on_event(keyboard_event& evt) { }
	void game_state::on_event(window_event& evt) { }
	// --==</core_methods>==--
}
namespace NW
{
	template<typename vtype, typename itype = ui32>
	struct NWL_API gfx_mesh_data
	{
	public:
		darray<vtype> vtxs;
		darray<itype> idxs;
		gfx_primitives prim;
	public:
		gfx_mesh_data(gfx_primitives primitive_topology = GPT_TRIANGLES) :
			vtxs(3, vtype()), idxs(3, itype()), prim(primitive_topology) { }
		// --core_methods
		void gen_idx_data() {
			idxs.clear();
			switch (prim) {
			if (vtxs.size() < 1) { break; }
			case GPT_POINTS: case GPT_LINE_LOOP: case GPT_LINE_STRIP: case GPT_TRIANGLE_FAN: {
				for (ui32 iv = 0; iv < vtxs.size() - 0; iv += 1) {
					idxs.push_back(iv + 0);
				}
				break;
			}
			case GPT_LINES: {
				if (vtxs.size() < 2) { break; }
				for (ui32 iv = 0; iv < vtxs.size() - 1; iv += 1) {
					idxs.push_back(iv + 0);
					idxs.push_back(iv + 1);
				}
				break;
			}
			case GPT_TRIANGLES: {
				if (vtxs.size() < 3) { break; }
				for (ui32 iv = 1; iv < vtxs.size() - 1; iv += 1) {
					idxs.push_back(iv - iv);
					idxs.push_back(iv + 0);
					idxs.push_back(iv + 1);
				}
				break;
			}
			case GPT_TRIANGLE_STRIP: {
				if (vtxs.size() < 3) { break; }
				for (ui32 iv = 1; iv < vtxs.size() - 0; iv += 1) {
					idxs.push_back(iv - iv);
					idxs.push_back(iv + 0);
				}
				break;
			}
			default: break;
			}
		}
	};

	//static mem_ref<a_gfx_ent> s_drb;
	
	gfx_state::gfx_state(core_engine& rEngine) :
		a_engine_state(rEngine),
		m_gfx(m_core->get_graphics()),
		m_camera_lad() { }
	gfx_state::~gfx_state() { }
	// --==<core_methods>==--
	bool gfx_state::init() {
		m_gfx = m_core->get_graphics();
		
		gfx_mesh_data<vtx2f2f, ui32> mesh_data(GPT_TRIANGLES);
		mesh_data.vtxs.resize(10);
		mesh_data.vtxs[0].vtx_crd = v2f32{ -0.5f,	-0.5f };
		mesh_data.vtxs[1].vtx_crd = v2f32{ -0.5f,	+0.5f };
		mesh_data.vtxs[2].vtx_crd = v2f32{ +0.0f,	+0.75f };
		mesh_data.vtxs[3].vtx_crd = v2f32{ +0.5f,	+0.5f };
		mesh_data.vtxs[4].vtx_crd = v2f32{ +0.5f,	-0.5f };
		
		mesh_data.vtxs[5].vtx_crd = v2f32{ +0.0f,	-0.75f };
		mesh_data.vtxs[6].vtx_crd = v2f32{ +0.0f,	-0.75f };
		mesh_data.vtxs[7].vtx_crd = v2f32{ +0.0f,	-0.75f };
		mesh_data.vtxs[8].vtx_crd = v2f32{ +0.0f,	-0.75f };
		mesh_data.vtxs[9].vtx_crd = v2f32{ +0.0f,	-0.75f };
		mesh_data.gen_idx_data();

		m_gfx->new_cmp<idx_buf>();
		m_gfx->get_cmp<a_gfx_buf>(1).
			get_ref<idx_buf>()->remake<ui32>(mesh_data.idxs.size(), &mesh_data.idxs[0]);
		m_gfx->new_cmp<vtx_buf>();
		m_gfx->get_cmp<a_gfx_buf>(2).
			get_ref<vtx_buf>()->remake<vtx2f2f>(mesh_data.vtxs.size(), &mesh_data.vtxs[0]);

		mem_ref<gfx_material> gmtl;
		m_gfx->new_cmp<gfx_material>("gmt_default_2d");
		gmtl.set_ref<gfx_material>(m_gfx->get_cmp<gfx_material>(1));
		if (!gmtl->load_file(R"(D:\dev\native_world\data\shader\shp_2d_default.shd)")) { return false; }
		
		mem_ref<a_texture> tex;
		m_gfx->new_cmp<texture2d>("txr_nw_logo");
		tex.set_ref(m_gfx->get_cmp<a_texture>(1));
		if (!tex->load_file(R"(D:\dev\native_world\data\image\nw_logo.bmp)")) { return false; }
		
		gmtl->get_shader(SHD_PIXEL)->get_texture(0).set_ref(tex);
		//s_drb->add_cmp(gmtl);

		return true;
	}
	void gfx_state::quit()
	{
		//s_drb.reset();
	}
	void gfx_state::update()
	{
		m_camera_lad.update();
		draw_scene();
	}

	void gfx_state::on_event(cursor_event& evt) { m_camera_lad.on_event(evt); }
	void gfx_state::on_event(keyboard_event& evt)
	{
		switch (evt.type) {
		case EVT_KEYBOARD_FREE:
			switch (evt.code) {
			case KC_1: m_gfx->set_primitive(GPT_TRIANGLES); break;
			case KC_2: m_gfx->set_primitive(GPT_TRIANGLE_STRIP); break;
			case KC_3: m_gfx->set_primitive(GPT_TRIANGLE_FAN); break;
			case KC_4: m_gfx->set_primitive(GPT_LINE_STRIP); break;
			case KC_5: m_gfx->set_primitive(GPT_LINES); break;
			case KC_6: m_gfx->set_primitive(GPT_LINE_LOOP); break;
			case KC_7: m_gfx->set_primitive(GPT_POINTS); break;
			}
			break;
		default: break;
		}
		m_camera_lad.on_event(evt);
	}
	void gfx_state::on_event(window_event& evt) {
		m_camera_lad.on_event(evt);
		switch (evt.type) {
		case EVT_WINDOW_RESIZE: {
			v4si32 rectViewport = m_gfx->get_configs().viewport;
			rectViewport[2] = evt.val_x;
			rectViewport[3] = evt.val_y;
			m_gfx->set_viewport(rectViewport[0], rectViewport[1], rectViewport[2], rectViewport[3]);
			break;
		}
		case EVT_WINDOW_MOVE: { break; }
		default: break;
		}
	}
	// --==</core_methods>==--

	// --==<implementation_methods>==--
	inline void gfx_state::draw_scene() {
		m_gfx->begin_draw();
		m_gfx->get_cmp<gfx_material>(1)->on_draw();
		m_gfx->get_cmp<a_gfx_buf>(1)->on_draw();
		m_gfx->get_cmp<a_gfx_buf>(2)->on_draw();
		m_gfx->end_draw();
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
	bool gui_state::init()
	{
		IMGUI_CHECKVERSION();
		GUI::CreateContext();

		m_gui_context = GUI::GetCurrentContext();
		m_gui_io = &GUI::GetIO();
		m_gui_style = &GUI::GetStyle();

		m_gui_io->ConfigFlags |=
			ImGuiConfigFlags_NavEnableKeyboard |
			ImGuiConfigFlags_ViewportsEnable |
			ImGuiConfigFlags_DockingEnable;
		m_gui_io->ConfigDockingWithShift = true;
		GUI::StyleColorsDark();
		if (m_gui_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			m_gui_style->WindowRounding = 0.0f;
			m_gui_style->Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
#if(defined NW_PLATFORM_WINDOWS)
		GUI::Win32Init(m_core->get_window()->get_native());
#endif
#if (NWG_GAPI & NWG_GAPI_OGL)
		GUI::OglInit("#version 130");
#endif
#if (NWG_GAPI & NWG_GAPI_DX)
		GUI::Dx11Init(core.get_gfx()->get_device(), core.get_gfx()->get_context());
#endif
		m_gui_of.resize(10);
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
#if (NWG_GAPI & NWG_GAPI_OGL)
		GUI::OglShutdown();
#endif
#if (NWG_GAPI & NWG_GAPI_DX)
		GUI::Dx11Shutdown();
#endif
#if(defined NW_PLATFORM_WINDOWS)
		GUI::Win32Shutdown();
#endif
		GUI::DestroyContext();
	}
	void gui_state::update() {
		begin_draw();
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

		end_draw();
	}

	void gui_state::on_event(cursor_event& rmEvt) { }
	void gui_state::on_event(keyboard_event& rkEvt) { }
	void gui_state::on_event(window_event& rwEvt) { }
	// --==</core_methods>==--

	// --==<implementation_methods>==--
	inline void gui_state::begin_draw() {
#if (NWG_GAPI & NWG_GAPI_OGL)
		GUI::OglNewFrame();
#endif
#if (NWG_GAPI & NWG_GAPI_DX)
		GUI::Dx11NewFrame();
#endif
#if (defined NW_PLATFORM_WINDOWS)
		GUI::Win32NewFrame();
#endif
		GUI::NewFrame();
		// note: switch this to true to enable dockspace
		bool bFullScreen = m_full_screen_persist;
		static ImGuiDockNodeFlags DockspaceFlags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		if (bFullScreen) {
			ImGuiViewport* Viewport = GUI::GetMainViewport();
			GUI::SetNextWindowPos(Viewport->Pos);
			GUI::SetNextWindowSize(Viewport->Size);
			GUI::SetNextWindowViewport(Viewport->ID);
			GUI::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			GUI::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		GUI::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		GUI::Begin("dockspace", &m_enable_dockspace, WindowFlags);
		GUI::PopStyleVar();

		if (bFullScreen) { GUI::PopStyleVar(2); }

		// dock space
		float nMinSizeW = m_gui_style->WindowMinSize.x;
		m_gui_style->WindowMinSize.x = 100.0f;
		m_gui_style->Alpha = 1.0f;
		if (m_gui_io->ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID nDockspaceId = GUI::GetID("DockSpace");
			GUI::DockSpace(nDockspaceId, ImVec2(0.0f, 0.0f), DockspaceFlags);
		}
		m_gui_style->WindowMinSize.x = nMinSizeW;
	}
	inline void gui_state::end_draw() {
		GUI::End();
		GUI::Render();
#if (NWG_GAPI & NWG_GAPI_OGL)
		GUI::OglRenderDrawData(GUI::GetDrawData());
#endif
#if (NWG_GAPI & NWG_GAPI_DX)
		GUI::Dx11RenderDrawData(GUI::GetDrawData());
#endif
		GUI::EndFrame();
		if (m_gui_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
#if (defined NW_PLATFORM_WINDOWS)
			GUI::UpdatePlatformWindows();
			GUI::RenderPlatformWindowsDefault();
#endif
		}
	}
	// --==</implementation_methods>==--
}