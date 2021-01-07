#ifndef NW_CORE_STATE_H
#define NW_CORE_STATE_H

#include <nw_decl.hpp>

namespace NW
{
	/// CoreState class
	/// Description:
	/// -- This is the main interaction part for the engine.
	/// -- State handles Lua virtual machiene which allows to use Lua scripts
	class NW_API CoreState
	{
	public:
		CoreState();
		virtual ~CoreState();

		// --getters
		virtual inline const char* GetName() { return "engine_state"; }
		// --setters
		// -- Predicates
		virtual bool IsEnabled() { return m_bIsEnabled; }
		// --core_methods
		virtual bool Init();
		virtual void Update();
		virtual void OnEnable();
		virtual void OnDisable();
		// --on_event_methods
		virtual void OnEvent(MouseEvent& rmEvt);
		virtual void OnEvent(KeyboardEvent& rkEvt);
		virtual void OnEvent(WindowEvent& rwEvt);
	private:
		bool m_bIsEnabled;
	};
}
#endif	// NW_CORE_STATE_H