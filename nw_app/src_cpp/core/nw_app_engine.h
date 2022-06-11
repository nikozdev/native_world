#ifndef NW_APP_CORE_ENGINE_H
#define NW_APP_CORE_ENGINE_H
#include "nw_app_core.hpp"
#if (defined NW_API)
#	include "nw_app_wnd_core.h"
#	include "nw_app_states.h"
#	include "std/nw_std_sing.h"
namespace NW
{
	/// app_engine class
	/// description:
	/// interface:
	class NW_API app_engine : public t_singleton<app_engine>
	{
	public:
		using engine_t = app_engine;
		using engine_tc = const engine_t;
		using state_t = app_state;
		using state_tc = const state_t;
		using states_t = darray_t<state_t*>;
		using states_tc = const states_t;
		using window_t = app_wnd_core;
		using window_tc = const window_t;
		using keybod_t = iop_keybod_t;
		using keybod_tc = const keybod_t;
		using kbd_code_t = keybod_t::code_t;
		using kbd_code_tc = keybod_t::code_tc;
		using cursor_t = iop_cursor_t;
		using cursor_tc = const cursor_t;
		using crs_code_t = cursor_t::code_t;
		using crs_code_tc = cursor_t::code_tc;
		using timer_t = time_state;
		using timer_tc = const timer_t;
	public:
		app_engine();
		virtual ~app_engine();
		// --getters
		inline thread* get_run_thread() { return &m_thr_run; }
		inline state_t* get_state(cv1u key)        { NW_CHECK(has_state(key), "not found!", return NW_NULL); return m_states[key]; }
		inline state_tc* get_state(cv1u key) const { NW_CHECK(has_state(key), "not found!", return NW_NULL); return m_states[key]; }
		inline window_t* get_window()        { return &m_window; }
		inline window_tc* get_window() const { return &m_window; }
		inline keybod_t* get_keybod()        { return &m_keybod; }
		inline keybod_tc* get_keybod() const { return &m_keybod; }
		inline cursor_t* get_cursor()        { return &m_cursor; }
		inline cursor_tc* get_cursor() const { return &m_cursor; }
		inline timer_t* get_timer()        { return &m_timer; }
		inline timer_tc* get_timer() const { return &m_timer; }
		inline cv1f get_timer_curr(v1f scale = 1.0f) const  { return m_timer.get_curr(scale); }
		inline cv1f get_timer_last(v1f scale = 1.0f) const  { return m_timer.get_last(scale); }
		inline cv1f get_timer_delta(v1f scale = 1.0f) const { return m_timer.get_delta(scale); }
		inline cv1f get_timer_ups(v1f scale = 1.0f) const   { return m_timer.get_ups(scale); }
		inline cstr_t get_window_title() const { return m_window.get_title(); }
		inline cv1u get_window_size_x() const  { return m_window.get_size_x(); }
		inline cv1u get_window_size_y() const  { return m_window.get_size_y(); }
		inline cv2u get_window_size_xy() const { return m_window.get_size_xy(); }
		inline cv1f get_cursor_move_coord_x() const                 { return get_cursor()->get_move_coord_x(); }
		inline cv1f get_cursor_move_coord_y() const                 { return get_cursor()->get_move_coord_y(); }
		inline cv1f get_cursor_free_coord_x(crs_code_tc code) const { return get_cursor()->get_free_coord_x(code); }
		inline cv1f get_cursor_free_coord_y(crs_code_tc code) const { return get_cursor()->get_free_coord_y(code); }
		inline cv1f get_cursor_free_delta_x(crs_code_tc code) const { return get_cursor()->get_free_delta_x(code); }
		inline cv1f get_cursor_free_delta_y(crs_code_tc code) const { return get_cursor()->get_free_delta_y(code); }
		inline cv1f get_cursor_held_coord_x(crs_code_tc code) const { return get_cursor()->get_held_coord_x(code); }
		inline cv1f get_cursor_held_coord_y(crs_code_tc code) const { return get_cursor()->get_held_coord_y(code); }
		inline cv1f get_cursor_held_delta_x(crs_code_tc code) const { return get_cursor()->get_held_delta_x(code); }
		inline cv1f get_cursor_held_delta_y(crs_code_tc code) const { return get_cursor()->get_held_delta_y(code); }
		// --setters
		engine_t& set_keybod_enabled(v1bit enable);
		engine_t& set_cursor_enabled(v1bit enable);
		engine_t& set_cursor_bounds(cv4s& bounds);
		template<class tstate, typename ... args>
		engine_t& add_state(args ... arguments) { m_states.push_back(new tstate(std::forward<args>(arguments)...)); return *this; }
		engine_t& rmv_state(cv1u key = NW_NULL);
		engine_t& stop_running();
		// --predicates
		inline v1bit has_state(cv1u key = 0u) const { return m_states.size() > key; }
		inline v1bit has_state(cstr_t key) const { for (auto& istate : m_states) { if (istate->has_name(key)) { return NW_TRUTH; } } return NW_FALSE; }
		inline v1bit is_running() const { return m_is_running; }
		inline v1bit is_keybod_enabled() const  { return m_keybod.is_enabled(); }
		inline v1bit is_keybod_raise(kbd_code_tc code) const { return m_keybod.is_raise(code); }
		inline v1bit is_keybod_press(kbd_code_tc code) const { return m_keybod.is_press(code); }
		inline v1bit is_keybod_free(kbd_code_tc code) const  { return m_keybod.is_free(code); }
		inline v1bit is_keybod_held(kbd_code_tc code) const  { return m_keybod.is_held(code); }
		inline v1bit is_cursor_enabled() const  { return m_cursor.is_enabled(); }
		inline v1bit is_cursor_raise(crs_code_tc code) const { return m_cursor.is_raise(code); }
		inline v1bit is_cursor_press(crs_code_tc code) const { return m_cursor.is_press(code); }
		inline v1bit is_cursor_free(crs_code_tc code) const  { return m_cursor.is_free(code); }
		inline v1bit is_cursor_held(crs_code_tc code) const  { return m_cursor.is_held(code); }
		// --operators
		// --core_methods
		v1bit init();
		v1bit quit();
		v1nil run();
		v1nil update();
		v1nil event_proc(iop_event_t& evt);
		// --data_methods
	private:
		thread m_thr_run;
		v1bit m_is_running;
		states_t m_states;
		window_t m_window;
		keybod_t m_keybod;
		cursor_t m_cursor;
		timer_t m_timer;
	};
}
#endif	// NW_API
#endif	// NW_APP_CORE_ENGINE_H
