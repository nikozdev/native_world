#ifndef NW_ENGINE_STATE_H
#define NW_ENGINE_STATE_H

#include <nw_decl.hpp>

namespace NW
{
	/// EngineState class
	/// Description:
	/// -- This is the main interaction part for the engine.
	/// -- State handles Lua virtual machiene which allows to use Lua scripts
	class NW_API EngineState
	{
	public:
		EngineState();
		virtual ~EngineState();

		// -- Getters
		virtual inline const char* GetName() { return "engine_state"; }
		// -- Setters
		// -- Predicates
		virtual bool IsEnabled() { return m_bIsEnabled; }
		// -- Core Methods
		virtual bool Init();
		virtual void Update();
		virtual void OnEnable();
		virtual void OnDisable();
		// -- OnEvent Methods
		virtual void OnEvent(MouseEvent& rmEvt);
		virtual void OnEvent(KeyboardEvent& rkEvt);
		virtual void OnEvent(WindowEvent& rwEvt);
	private:
		bool m_bIsEnabled;
	};
}
#endif	// NW_ENGINE_STATE_H