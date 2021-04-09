#ifndef NW_APP_ENGINE_STATES_H
#define NW_APP_ENGINE_STATES_H
#include "nw_app_core.hpp"
#if (defined NW_API)
namespace NW
{
	/// abstract core_state class
	/// description:
	/// --this is the main interaction part for the engine.
	class NW_API a_core_state : public a_mem_cmp
	{
	public:
		a_core_state(core_engine& engine);
		virtual ~a_core_state();
		// --getters
		virtual inline cstr get_name() const = 0;
		// --setters
		// --core_methods
		virtual bool init() = 0;
		virtual void quit() = 0;
		virtual void update() = 0;
		virtual void event_proc(a_event& evt) = 0;
	protected:
		core_engine* m_core;
	};
}
namespace NW
{
	/// game_core_state class
	class game_core_state : public a_core_state
	{
	public:
		game_core_state(core_engine& engine);
		~game_core_state();
		// --getters
		virtual inline cstr get_name() const override { return "game_core_state"; }
		// --core_methods
		virtual bool init() override;
		virtual void quit() override;
		virtual void update() override;
		virtual void event_proc(a_event& evt) override;
	};
}
namespace NW
{
	/// graphics_core_state class
	class gfx_core_state : public a_core_state
	{
	public:
		gfx_core_state(core_engine& engine);
		~gfx_core_state();
		// --getters
		virtual inline cstr get_name() const override { return "gfx_core_state"; }
		// --core_methods
		virtual bool init() override;
		virtual void quit() override;
		virtual void update() override;
		virtual void event_proc(a_event& evt) override;
	private:
		gfx_engine* m_gfx;
		gfx_cam_lad m_cam_lad;
	};
}
#include <core/nw_app_gui_of.h>
namespace NW
{
	/// gui_core_state class
	/// description:
	class gui_core_state : public a_core_state
	{
		using gui_ref = mem_ref<a_gui_of>;
		using gui_refs = darray<gui_ref>;
	public:
		gui_core_state(core_engine& engine);
		~gui_core_state();
		// --getters
		virtual inline cstr get_name() const override { return "gui_core_state"; }
		// --core_methods
		virtual bool init() override;
		virtual void quit() override;
		virtual void update() override;
		virtual void event_proc(a_event& evt) override;
	private:
		gui_refs m_gui_refs;
	};
}
#endif	// NW_API
#endif	// NW_APP_ENGINE_STATES_H
