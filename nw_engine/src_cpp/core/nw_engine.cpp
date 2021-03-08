#include <nw_pch.hpp>
#include <core/nw_engine.h>
namespace NW
{
	core_engine::core_engine(cstring name) :
		m_name(&name[0]),
		m_is_running(false), m_thr_run(thread()),
		m_gfx(mem_ref<gfx_engine>()),
		m_wnd(mem_ref<core_window>()),
		m_kbd(keyboard_state()), m_crs(cursor_state())
	{
		mem_sys::on_init(1 << 23);
		data_sys::on_init();
		log_sys::on_init();

		ent_sys::on_init();
		cmp_sys::on_init();
	}
	core_engine::~core_engine()
	{
		m_gfx.reset();
		m_wnd.reset();
		m_states.clear();

		ent_sys::on_quit();
		cmp_sys::on_quit();

		log_sys::on_quit();

		data_sys::on_quit();
		mem_sys::on_quit();
	}
	// --setters
	void core_engine::add_state(state& state) { m_states.push_back(&state); }
	void core_engine::rmv_state(ui8 idx) { if (m_states.size() <= idx) { return; } m_states.erase(m_states.begin() + idx); }
	void core_engine::stop_running() { m_is_running = false; }
	// --==<core_methods>==--
	bool core_engine::init()
	{
		if (m_wnd.is_valid() || m_gfx.is_valid()) { return false; }
		if (!m_is_running) { m_is_running = true; }

		m_wnd.make_ref<core_window>(window_info{ &m_name[0], 1200, 800 });
		m_wnd->set_event_callback([this](a_event& evt)->void { return this->event_proc(evt); });

		m_gfx.make_ref<gfx_engine>(*m_wnd->get_native());
		m_gfx->set_viewport(0, 0, m_wnd->get_size_x(), m_wnd->get_size_y());
		m_gfx->set_vsync(true);

		for (auto& istate : m_states) { if (!istate->init()) { return false; } }
		
		return true;
	}
	void core_engine::quit()
	{
		if (!m_wnd.is_valid() || !m_gfx.is_valid()) { return; }
		if (!m_is_running) { m_is_running = true; }
		
		for (auto& istate : m_states) { istate->quit(); }

		m_gfx.reset();
		m_wnd.reset();
	}
	void core_engine::run()
	{
		m_is_running = true;
		auto run_loop = [this]()->void {
			try {
				if (!init()) { return; }
				while (m_is_running) { update(); }
				quit();
			}
			catch (NWL::error& exc) {
				std::cout << exc;
				quit();
			}
			catch (std::exception& exc) {
				std::cout << exc.what();
				quit();
			}
		};
		m_thr_run = thread(run_loop);
	}
	void core_engine::update()
	{
		for (auto& istate : m_states) { istate->update(); }
		log_sys::update();
		m_crs.update();
		m_kbd.update();
		m_timer.update();
		m_wnd->update();
		m_gfx->update();
	}
	void core_engine::event_proc(a_event& evt)
	{
		if (evt.is_in_category(EVC_CURSOR)) {
			cursor_event& crs_evt = static_cast<cursor_event&>(evt);
			m_crs.event_proc(crs_evt);
			for (auto& istate : m_states) { if (evt.is_handled) return; istate->event_proc(crs_evt); }
			if (crs_evt.is_handled) { return; }
		}
		else if (evt.is_in_category(EVC_KEYBOARD)) {
			keyboard_event& kbd_evt = static_cast<keyboard_event&>(evt);
			m_kbd.event_proc(kbd_evt);
			for (auto& istate : m_states) { if (evt.is_handled) { return; } istate->event_proc(kbd_evt); }
			if (kbd_evt.is_handled) { return; }
			if (kbd_evt.type == EVT_KEYBOARD_RELEASED) {
				switch (kbd_evt.code) {
				case KC_M:
					m_crs.set_mode((m_crs.get_mode() == CRS_DEFAULT) ? CRS_CAPTURED : CRS_DEFAULT);
					break;
				case KC_ESCAPE:
					stop_running();
					evt.is_handled = true;
					break;
				}
			}
		}
		else if (evt.is_in_category(EVC_WINDOW)) {
			window_event& wnd_evt = static_cast<window_event&>(evt);
			for (auto& istate : m_states) { if (evt.is_handled) { return; } istate->event_proc(wnd_evt); }
			if (wnd_evt.is_handled) { return; }
			if (wnd_evt.type == EVT_WINDOW_CLOSE) {
				stop_running();
				evt.is_handled = true;
			}
		}
	}

	dstring core_engine::dialog_save(cstring filter) { return data_sys::dialog_save(filter, m_wnd->get_native()); }
	dstring core_engine::dialog_load(cstring filter) { return data_sys::dialog_load(filter, m_wnd->get_native()); }
	// --==</core_methods>==--
}