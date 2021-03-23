#ifndef NW_WINDOW_H
#define NW_WINDOW_H
#include <nw_core.hpp>
#if (defined NW_WAPI)
namespace NW
{
	/// window_info struct
	struct NW_API window_info : public a_info
	{
	public:
		dstr title = "default";
		dstr api_version = "default";
		v1ui coord_x = 0, coord_y = 0;
		v1ui size_x = 800, size_y = 600;
		v1f aspect_ratio = 800.0f / 600.0f;
		v1f opacity = 0.0f;
		bit is_hovered = false;
		bit is_focused = false;
		bit is_enabled = false;
		event_callback event_proc = [](a_event&)->void { return; };
	public:
		window_info(cstr window_title = "default", v1ui width = 800, v1ui height = 600);
		// --operators
		virtual stm_out& operator<<(stm_out& stm) const override;
		virtual stm_in& operator>>(stm_in& stm) override;
	};
}
namespace NW
{
	class NW_API a_wnd : public a_mem_user
	{
	public:
		using info = window_info;
		using cinfo = const window_info;
		using handle = window_handle;
		using ñhandle = const window_handle;
	public:
		a_wnd(cinfo& information);
		virtual ~a_wnd();
		// --getters
		inline cinfo& get_info() const      { return m_info; }
		inline v2ui get_coord() const       { return { m_info.coord_x, m_info.coord_y }; }
		inline v1ui get_coord_x() const     { return m_info.coord_x; }
		inline v1ui get_coord_y() const     { return m_info.coord_y; }
		inline v2ui get_size() const        { return { m_info.size_x, m_info.size_y }; }
		inline v1ui get_size_x() const      { return m_info.size_x; }
		inline v1ui get_size_y() const      { return m_info.size_y; }
		inline v4ui get_viewport() const    { return { m_info.coord_x, m_info.coord_y, m_info.size_x, m_info.size_y }; }
		inline v1ui get_opacity() const     { return m_info.opacity; }
		inline cstr get_title() const       { return &m_info.title[0]; }
		inline handle& get_handle()         { return m_handle; }
		inline ñhandle& get_handle() const  { return m_handle; }
		// --setters
		void set_title(cstr title);
		void set_focused(bit is_focused);
		void set_enabled(bit enable);
		void set_opacity(v1f opacity);
		void set_icon(const img_cmp& img);
		void set_callback(const event_callback& event_proc);
		// --predicates
		inline bit is_hovered() const { return m_info.is_hovered; }
		inline bit is_focused() const { return m_info.is_focused; }
		inline bit is_enabled() const { return m_info.is_enabled; }
		// --core_methods
		void update();
	protected:
#if (NW_WAPI & NW_WAPI_WIN)
		static LRESULT __stdcall event_proc_init(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static LRESULT __stdcall event_proc_static(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		inline LRESULT __stdcall event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#endif
	protected:
		info m_info;
		handle m_handle;
#if (NW_WAPI & NW_WAPI_WIN)
		MSG m_msg;
		WNDCLASSEX m_class;
#endif
	};
}
#endif	// NW_WAPI
#endif	// NW_WINDOW_H