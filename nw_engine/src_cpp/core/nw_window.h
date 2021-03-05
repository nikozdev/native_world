#ifndef NW_CORE_WINDOW_H
#define NW_CORE_WINDOW_H
#include <nw_core.hpp>
namespace NW
{
	struct NW_API window_info : public a_info
	{
	public:
		dstring title = "default";
		dstring api_version = "default";
		ui16 coord_x = 0, coord_y = 0;
		ui16 size_x = 800, size_y = 600;
		f32 aspect_ratio = 800.0f / 600.0f;
		f32 opacity= 0.0f;
		bit is_hovered = false;
		bit is_focused = false;
		bit is_enabled = false;
		event_callback on_event = [](a_event&)->void { return; };
	public:
		window_info(cstring window_title = "default", ui16 width = 800, ui16 height = 600);
		// --operators
		virtual out_stream& operator<<(out_stream& stm) const override;
		virtual in_stream& operator>>(in_stream& stm) override;
	};
}
namespace NW
{
	/// core_window class
	class NW_API core_window : public a_mem_user
	{
	public:
		core_window(const window_info& info);
		core_window(const core_window& copy) = delete;
		virtual ~core_window();
		// --getters
		inline ui16 get_coord_x() const	{ return m_info.coord_x; }
		inline ui16 get_coord_y() const	{ return m_info.coord_y; }
		inline ui16 get_size_x() const	{ return m_info.size_x; }
		inline ui16 get_size_y() const	{ return m_info.size_y; }
		inline ui16 get_opacity() const	{ return m_info.opacity; }
		inline cstring get_title() const{ return &m_info.title[0]; }
		inline HWND& get_native()		{ return m_native; }
		inline const window_info& get_info() const { return m_info; }
		// --setters
		void set_title(cstring title);
		void set_focused(bit is_focused);
		void set_enabled(bit enable);
		void set_opacity(f32 opacity);
		void set_icon(const image_info& info);
		void set_event_callback(const event_callback& on_event);
		void set_keyboard_mode(keyboard_modes kbdMode);
		void set_cursor_mode(cursor_modes crsMode);
		// --predicates
		inline bit is_hovered() const	{ return m_info.is_hovered; }
		inline bit  is_focused() const	{ return m_info.is_focused; }
		inline bit is_enabled() const	{ return m_info.is_enabled; }
		// --operators
		void operator=(const core_window& copy) = delete;
		// --core_methods
		void update();
	private:
		static LRESULT __stdcall msg_proc_init(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static LRESULT __stdcall msg_proc_static(HWND wnd, UINT msd, WPARAM wparam, LPARAM lparam);
		LRESULT __stdcall msg_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
	private:
		core_engine* core;
		window_info m_info;
		HWND m_native;
		MSG m_msg;
		WNDCLASSEX m_class;
		PAINTSTRUCT m_pts;
	};
}
#endif	// NW_CORE_WINDOW_H
/*
* Development started 18.10.2020
* --Tutorials used:
* -->Cherno GameEngine series
* -->MihailRis - Minecraft Voxel Engine
*/