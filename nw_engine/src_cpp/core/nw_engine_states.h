#ifndef NATIVE_ENGINE_STATES_H
#define NATIVE_ENGINE_STATES_H
#include <nw_core.hpp>
namespace NW
{
	/// abstract engine_state class
	/// description:
	/// --this is the main interaction part for the engine.
	class NW_API a_engine_state : public a_mem_user
	{
	public:
		a_engine_state(core_engine& engine);
		virtual ~a_engine_state();
		// --getters
		virtual inline cstring get_name() = 0;
		// --setters
		// --core_methods
		virtual bool init() = 0;
		virtual void quit() = 0;
		virtual void update() = 0;
		virtual void event_proc(cursor_event& crs_evt) = 0;
		virtual void event_proc(keyboard_event& kbd_evt) = 0;
		virtual void event_proc(window_event& wnd_evt) = 0;
	protected:
		core_engine* m_core;
	};
}
namespace NW
{
	/// game_state class
	class game_state : public a_engine_state
	{
	public:
		game_state(core_engine& engine);
		~game_state();
		// --getters
		virtual inline cstring get_name() override { return "game_state"; }
		// --core_methods
		virtual bool init() override;
		virtual void quit() override;
		virtual void update() override;
		virtual void event_proc(cursor_event& crs_evt) override;
		virtual void event_proc(keyboard_event& kbd_evt) override;
		virtual void event_proc(window_event& wnd_evt) override;
	};
}
#include <core/nw_gui_of.h>
namespace NW
{
	/// gui_state class
	/// description:
	class gui_state : public a_engine_state
	{
	public:
		gui_state(core_engine& engine);
		~gui_state();
		// --getters
		virtual inline cstring get_name() override { return "gui_state"; }
		// --core_methods
		virtual bool init() override;
		virtual void quit() override;
		virtual void update() override;
		virtual void event_proc(cursor_event& crs_evt) override;
		virtual void event_proc(keyboard_event& kbd_evt) override;
		virtual void event_proc(window_event& wnd_evt) override;
	private:
		ImGuiContext* m_gui_context;
		imgui_io* m_gui_io;
		imgui_style* m_gui_style;

		bit m_full_screen_persist;
		bit m_enable_dockspace;

		darray<mem_ref<a_gui_of>> m_gui_of;
	};
}
namespace NW
{
	/// graphics_state class
	class gfx_state : public a_engine_state
	{
	public:
		gfx_state(core_engine& rEngine);
		~gfx_state();
		// --getters
		virtual inline cstring get_name() override { return "gfx_state"; }
		// --core_methods
		virtual bool init() override;
		virtual void quit() override;
		virtual void update() override;
		virtual void event_proc(cursor_event& crs_evt) override;
		virtual void event_proc(keyboard_event& kbd_evt) override;
		virtual void event_proc(window_event& wnd_evt) override;
	private:
		inline bool init_scene();
		inline bool init_skybox();
		inline void draw_scene();
		inline void draw_skybox();
	private:
		gfx_engine* m_gfx;
		gfx_camera_lad m_camera_lad;
	};
}
#endif	// NATIVE_ENGINE_STATES_H