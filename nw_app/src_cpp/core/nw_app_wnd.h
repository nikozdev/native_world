#ifndef NW_APP_WINDOW_H
#define NW_APP_WINDOW_H
#include "nw_app_core.hpp"
#if (defined NW_WAPI)
namespace NW
{
	class NW_API app_wnd : public a_mem_cmp
	{
	public:
		using handle_t = window_handle;
		using handle_tc = const handle_t;
		using event_t = iop_event_wnd_t;
		using event_tc = const event_t;
	public:
		app_wnd();
		virtual ~app_wnd();
		// --getters
		inline handle_t& get_handle()        { return m_handle; }
		inline handle_tc& get_handle() const { return m_handle; }
		inline cv1u get_size() const     { return m_viewp[2] * m_viewp[3]; }
		inline cv2u get_size_xy() const  { return cv2u{ m_viewp[2], m_viewp[3] }; }
		inline cv1u get_size_x() const   { return m_viewp[2]; }
		inline cv1u get_size_y() const   { return m_viewp[3]; }
		inline cv2u get_coord_xy() const { return cv2u{ m_viewp[0], m_viewp[1] }; }
		inline cv1u get_coord_x() const  { return m_viewp[0]; }
		inline cv1u get_coord_y() const  { return m_viewp[1]; }
		inline cv4u get_viewp_xywh() const { return m_viewp; }
		inline cv1f get_size_ratio() const { return NW_CAST_FLOAT(get_size_x()) / NW_CAST_FLOAT(get_size_y()); }
		inline cv1u get_opacity() const  { return m_opacity; }
		inline cstr_t get_title() const  { return &m_title[0]; }
		// --setters
		v1nil set_size_xy(cv2u size_xy);
		inline v1nil set_size_x(cv1u size_x) { set_size_xy({size_x, get_size_y()}); }
		inline v1nil set_size_y(cv1u size_y) { set_size_xy({ get_size_x(), size_y }); }
		inline v1nil set_size_xy(cv1u size_x, cv1u size_y) { set_size_xy({ size_x, size_y }); }
		v1nil set_coord_xy(cv2u coord_xy);
		inline v1nil set_coord_x(cv1u coord_x) { set_size_xy({ coord_x, get_coord_y() }); }
		inline v1nil set_coord_y(cv1u coord_y) { set_size_xy({ get_coord_x(), coord_y }); }
		inline v1nil set_coord_xy(cv1u coord_x, cv1u coord_y) { set_coord_xy({ coord_x, coord_y }); }
		v1nil set_viewp_xywh(cv4u viewport);
		inline v1nil set_viewp_xywh(cv2u coord_xy, cv2u size_xy) { set_viewp_xywh({ coord_xy[0], coord_xy[1], size_xy[0], size_xy[1] }); }
		inline v1nil set_viewp_xywh(cv1u coord_x, cv1u coord_y, cv1u size_x, cv1u size_y) { set_viewp_xywh({ coord_x, coord_y, size_x, size_y }); }
		v1nil set_title(cstr_t title);
		v1nil set_enabled(v1bit is_enabled);
		v1nil set_hovered(v1bit is_hovered);
		v1nil set_focused(v1bit is_focused);
		v1nil set_opacity(v1f opacity);
		v1nil set_icon(const gfx_img& img);
		v1nil set_callback(const event_callback& event_proc);
		// --predicates
		inline v1bit is_enabled() const { return m_is_enabled; }
		inline v1bit is_hovered() const { return m_is_hovered; }
		inline v1bit is_focused() const { return m_is_focused; }
		// --core_methods
		v1nil update();
		v1bit remake();
	protected:
#	if (NW_WAPI & NW_WAPI_WIN)
		static LRESULT WINAPI event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static LRESULT WINAPI event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		inline LRESULT WINAPI event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#	endif
	protected:
		handle_t m_handle;
#	if (NW_WAPI & NW_WAPI_WIN)
		MSG m_msg;
		WNDCLASSEX m_class;
#	endif
		dstr_t m_title;
		dstr_t m_ver_str;
		v4u m_viewp;
		v1f m_ratio;
		v1f m_opacity;
		v1bit m_is_enabled = NW_FALSE;
		v1bit m_is_hovered = NW_FALSE;
		v1bit m_is_focused = NW_FALSE;
		event_callback m_event_proc;
	};
}
#endif	// NW_WAPI
#endif	// NW_APP_WINDOW_H
