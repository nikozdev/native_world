#ifndef NW_ENGINE_H
#define NW_ENGINE_H
#include <nw_core.hpp>
#include <core/nw_window.h>
#include <core/nw_engine_states.h>;
namespace NW
{
	/// core_engine class
	/// description:
	/// interface:
	class NW_API core_engine : public a_mem_user
	{
	public:
		using state = a_engine_state;
		using states = darray<state*>;
	public:
		core_engine(cstring name = "nw_engine");
		core_engine(const core_engine& copy) = delete;
		virtual ~core_engine();
		// --getters
		inline cstring get_name()			{ return &m_name[0]; }
		inline thread* get_run_thread()		{ return &m_thr_run; }
		inline core_window* get_window()	{ return m_wnd; }
		inline gfx_engine* get_graphics()	{ return m_gfx; }
		inline state* get_state(ui32 idx)	{ return m_states[idx]; }
		inline const keyboard_state* get_keyboard() const	{ return &m_kbd; }
		inline const cursor_state* get_cursor() const		{ return &m_crs; }
		inline const time_state* get_timer() const			{ return &m_timer; }
		// --setters
		void add_state(state& state);
		void rmv_state(ui8 idx);
		void stop_running();
		// --predicates
		inline bit is_running() const { return m_is_running; }
		// --operators
		void operator=(const core_engine& copy) = delete;
		// --core_methods
		bool init();
		void quit();
		void run();
		void update();
		void event_proc(a_event& evt);
		// --data_methods
		dstring dialog_load(cstring filter);
		dstring dialog_save(cstring filter);
	private:
		cstring m_name;
		thread m_thr_run;
		bit m_is_running;
		mem_ref<core_window> m_wnd;
		mem_ref<gfx_engine> m_gfx;
		states m_states;
		keyboard_state m_kbd;
		cursor_state m_crs;
		time_state m_timer;
	};
}
#endif	// NW_ENGINE_H