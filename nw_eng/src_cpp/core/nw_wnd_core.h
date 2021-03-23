#ifndef NW_CORE_WINDOW_H
#define NW_CORE_WINDOW_H
#include <nw_core.hpp>
#if (defined NW_WAPI)
#include "nw_wnd.h"
namespace NW
{
	/// wnd_core class
	class NW_API wnd_core : public a_wnd
	{
	public:
		using graphics = mem_ref<gfx_engine>;
		using keyboard = keyboard_state;
		using mouse = mouse_state;
		using raw_buf = darray<sbyte>;
	public:
		wnd_core(cinfo& information);
		virtual ~wnd_core();
		// --getters
		inline graphics& get_graphics()				{ return m_gfx; }
		inline const graphics& get_graphics() const	{ return m_gfx; }
		inline const mouse* get_mouse()	const		{ return &m_mouse; }
		inline const keyboard* get_keyboard() const	{ return &m_kbd; }
		// --setters
		void set_cursor_enabled(bit enable_cursor);
		// --predicates
		inline bit is_cursor_enabled() const		{ return m_mouse.is_cursor_enabled(); }
		// --core_methods
		void update();
#if (NW_WAPI & NW_WAPI_WIN)
		static inline LRESULT __stdcall event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static inline LRESULT __stdcall event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		inline LRESULT __stdcall event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#endif
	private:
		graphics m_gfx;
		keyboard m_kbd;
		mouse m_mouse;
#if (NW_WAPI & NW_WAPI_WIN)
		raw_buf m_raw_buf;
#endif
	};
}
#endif	// NW_WAPI
#endif	// NW_CORE_WINDOW_H