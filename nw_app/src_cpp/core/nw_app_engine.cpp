#include "nw_app_pch.hpp"
#include "nw_app_engine.h"
#if (defined NW_API)
namespace NW
{
	app_engine::app_engine() :
		m_is_running(NW_FALSE),
		m_thr_run(thread()),
		m_states(states_t()),
		m_window(window_t()),
		m_keybod(keybod_t()),
		m_cursor(cursor_t()),
		m_timer(timer_t())
	{
	}
	app_engine::~app_engine()
	{
	}
	// --setters
	v1nil app_engine::set_keybod_enabled(v1bit enable) {
		m_keybod.set_enabled(enable);
		gui_set_cursor_enabled(enable);
	}
	v1nil app_engine::set_cursor_enabled(v1bit enable) {
		m_cursor.set_enabled(enable);
		gui_set_cursor_enabled(enable);
	}
	v1nil app_engine::rmv_state(cv1u key) {
		NW_CHECK(has_state(key), "not found!", return);
		delete m_states[key];
		m_states.erase(m_states.begin() + key);
	}
	v1nil app_engine::stop_running() {
		m_is_running = NW_FALSE;
	}
	// --==<core_methods>==--
	v1bit app_engine::init()
	{
		// NW_CHECK(!is_running(), "init is already done!", return NW_FALSE);
		m_is_running = NW_TRUE;

		NW_CHECK(mem_sys::get().init(NW_MAX_MEMORY), "failed init!", return NW_FALSE);
		NW_CHECK(iop_sys::get().init(), "failed init!", return NW_FALSE);
		NW_CHECK(cmp_sys::get().init(), "failed init!", return NW_FALSE);
		NW_CHECK(ent_sys::get().init(), "failed init!", return NW_FALSE);
		
		NW_CHECK(m_window.remake(NW_NAME_STR, 1200u, 800u), "failed remake!", return NW_FALSE);
		m_window.set_callback([this](iop_event_t& evt)->v1nil { return this->event_proc(evt); });

		NW_CHECK(gfx_engine::get().init(m_window.get_handle()), "failed init!", return NW_FALSE);
		gfx_engine::get().set_viewp(0u, 0u, m_window.get_size_x(), m_window.get_size_y());
		//gfx_engine::get().set_fmbuf_size(get_window_size_x(), get_window_size_y());

		for (auto& istate : m_states) { NW_CHECK(istate->init(), "failed init!", return NW_FALSE); }
		
		return NW_TRUE;
	}
	v1bit app_engine::quit()
	{
		// NW_CHECK(is_running(), "quit is already done!", return NW_FALSE);
		m_is_running = NW_FALSE;

		for (auto& istate : m_states) { NW_CHECK(istate->quit(), "failed quit!", return NW_FALSE); }
		while (has_state()) { rmv_state(); }
		m_states.clear();

		NW_CHECK(cmp_sys::get().quit(), "failed quit!", return NW_FALSE);
		NW_CHECK(ent_sys::get().quit(), "failed quit!", return NW_FALSE);
		NW_CHECK(iop_sys::get().quit(), "failed quit!", return NW_FALSE);
		NW_CHECK(mem_sys::get().quit(), "failed quit!", return NW_FALSE);

		return NW_TRUE;
	}
	v1nil app_engine::run()
	{
		auto run_loop = [this]()->v1bit {
			try {
				NW_CHECK(init(), "failed init!", return NW_FALSE);
				while (m_is_running) { update(); }
				NW_CHECK(quit(), "failed quit!", return NW_FALSE);
			}
			catch (NW::a_error& exc) {
				NW_ERROR(exc.get_str(), quit());
				return NW_FALSE;
			}
			catch (std::exception& exc) {
				NW_ERROR(exc.what(), quit());
				return NW_FALSE;
			}
			catch (...) {
				NW_ERROR("something went wrong", quit());
				return NW_FALSE;
			}
			return NW_TRUE;
		};

		m_thr_run = thread(run_loop);
	}
	v1nil app_engine::update()
	{
		for (auto& istate : m_states) { istate->update(); }
		m_keybod.update();
		m_cursor.update();
		m_timer.update();
		m_window.update();
		gfx_engine::get().update();
	}
	v1nil app_engine::event_proc(iop_event_t& evt)
	{
		if (evt.has_sort(NW_EVSORT_CURSOR)) {
			cursor_t::event_t& crs_evt = evt;
			m_cursor.event_proc(crs_evt);
		}
		else if (evt.has_sort(NW_EVSORT_KEYBOD)) {
			keybod_t::event_t& kbd_evt = evt;
			m_keybod.event_proc(kbd_evt);
			switch (evt.get_type()) {
			case NW_EVTYPE_KEYBOD_PRESS: {
				switch (kbd_evt.get_code()) {
				case NW_KEYCODE_ESCP: { stop_running(); return; break; }
				case NW_KEYCODE_F1: { m_window.set_opacity(0.1f); return; break; }
				case NW_KEYCODE_F2: { m_window.set_opacity(0.2f); return; break; }
				case NW_KEYCODE_F3: { m_window.set_opacity(0.3f); return; break; }
				case NW_KEYCODE_F4: { m_window.set_opacity(0.4f); return; break; }
				case NW_KEYCODE_F5: { m_window.set_opacity(0.5f); return; break; }
				case NW_KEYCODE_F6: { m_window.set_opacity(0.6f); return; break; }
				case NW_KEYCODE_F7: { m_window.set_opacity(0.7f); return; break; }
				case NW_KEYCODE_F8: { m_window.set_opacity(0.8f); return; break; }
				case NW_KEYCODE_F9: { m_window.set_opacity(0.9f); return; break; }
				case NW_KEYCODE_F10: { m_window.set_opacity(1.0f); return; break; }
				break;
				}
			}
			}
		}
		else if (evt.has_sort(NW_EVSORT_WINDOW)) {
			iop_event_wnd_t& wnd_evt = evt;
			switch (evt.get_type()) {
			case NW_EVTYPE_WINDOW_SIZED: {
				gfx_engine::get().set_viewp(0, 0, wnd_evt.get_val_x(), wnd_evt.get_val_y());
				//gfx_engine::get().set_fmbuf_size(wnd_evt.get_val_x(), wnd_evt.get_val_y());
				break;
			}
			case NW_EVTYPE_WINDOW_ACTIV: {
				if (wnd_evt.is_activ()) { if (is_cursor_enabled()) { set_cursor_enabled(NW_FALSE); } }
				else { if (is_cursor_enabled()) { set_cursor_enabled(NW_TRUE); } }
				break;
			}
			case NW_EVTYPE_WINDOW_CLOSE: {
				if (wnd_evt.is_close()) {
					stop_running();
					evt.set_handled(NW_TRUE);
					return;
					break;
				}
			}
		}
		}
		else if (evt.has_sort(NW_EVSORT_APPLIC)) {
			switch (evt.get_type()) {
			case NW_EVTYPE_APPLIC_DROPF: {
				break;
			}
			}
		}
		for (auto& istt : m_states) { if (evt.is_handled()) { return; } istt->event_proc(evt); }
	}
	// --==</core_methods>==--
}
#endif	// NW_API