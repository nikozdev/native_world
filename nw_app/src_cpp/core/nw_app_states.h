#ifndef NW_APP_ENGINE_STATES_H
#define NW_APP_ENGINE_STATES_H
#include "nw_app_core.hpp"
#if (defined NW_API)
namespace NW
{
	/// application_state class
	/// description:
	/// --this is the main interaction part for the engine.
	class NW_API app_state : public a_name_owner
	{
	public:
		app_state(cstr_t name) : a_name_owner(name) { }
		virtual ~app_state() = default;
		// --getters
		// --setters
		// --core_methods
		virtual v1bit init() = 0;
		virtual v1bit quit() = 0;
		virtual v1nil update() = 0;
		virtual v1nil event_proc(iop_event_t& evt) = 0;
	protected:
	};
}
namespace NW
{
	/// application_state_game class
	class app_state_game : public app_state
	{
	public:
		app_state_game();
		~app_state_game();
		// --getters
		// --core_methods
		virtual v1bit init() override;
		virtual v1bit quit() override;
		virtual v1nil update() override;
		virtual v1nil event_proc(iop_event_t& evt) override;
	};
}
#	include <core/nw_app_gui_of.h>
namespace NW
{
	/// gui_core_state class
	/// description:
	class app_state_gui : public app_state
	{
		using gui_ref = mem_ref<a_gui_of>;
		using gui_refs = t_darray<gui_ref>;
	public:
		app_state_gui();
		~app_state_gui();
		// --getters
		// --core_methods
		virtual v1bit init() override;
		virtual v1bit quit() override;
		virtual v1nil update() override;
		virtual v1nil event_proc(iop_event_t& evt) override;
	private:
		gui_refs m_gui_refs;
	};
}
#endif	// NW_API
#endif	// NW_APP_ENGINE_STATES_H
