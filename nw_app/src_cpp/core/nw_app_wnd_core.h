#ifndef NW_APP_CORE_WINDOW_H
#define NW_APP_CORE_WINDOW_H
#include "nw_app_core.hpp"
#if (defined NW_WAPI)
#	include "nw_app_wnd.h"
namespace NW
{
	/// app_wnd_core class
	class NW_API app_wnd_core : public app_wnd
	{
	public:

		using raw_buf_t = t_darray<sbyte_t>;
	public:
		app_wnd_core();
		virtual ~app_wnd_core();
		// --getters
		// --setters
		// --predicates
		// --core_methods
		v1nil update();
		v1bit remake();
		inline v1bit remake(cv2u size_xy) { set_size_xy(size_xy); return remake(); }
		inline v1bit remake(cstr_t title) { set_title(title); return remake(); }
		inline v1bit remake(cstr_t title, cv2u size_xy) { set_title(title); set_size_xy(size_xy); return remake(); }
		inline v1bit remake(cstr_t title, cv1u size_x, cv1u size_y) { set_title(title); set_size_x(size_x); set_size_y(size_y); return remake(); }
#	if (NW_WAPI & NW_WAPI_WIN)
		static LRESULT WINAPI event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static LRESULT WINAPI event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		inline LRESULT WINAPI event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#	endif	// WAPI_WIN
	private:
#	if (NW_WAPI & NW_WAPI_WIN)
		raw_buf_t m_raw_buf;
#	endif
	};
}
#endif	// NW_WAPI
#endif	// NW_APP_CORE_WINDOW_H
