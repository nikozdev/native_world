#ifndef NW_SUB_WINDOW_H
#define NW_SUB_WINDOW_H
#include <nw_core.hpp>
#if (defined NW_WAPI)
#include "nw_wnd.h"
namespace NW
{
	class NW_API wnd_sub : public a_wnd
	{
	public:
		wnd_sub(wnd_core& core, cinfo& information);
		virtual ~wnd_sub();
		// --getters
		// --setters
		// --core_methods
		void update();
	protected:
#if (NW_WAPI & NW_WAPI_WIN)
		static LRESULT __stdcall event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static LRESULT __stdcall event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		inline LRESULT __stdcall event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#endif
	protected:
		wnd_core* m_core;
	};
}
#endif	// NW_WAPI
#endif	// NW_SUB_WINDOW_H