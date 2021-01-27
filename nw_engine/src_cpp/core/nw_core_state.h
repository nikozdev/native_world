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
		virtual ~CoreState() = default;

		// --getters
		virtual inline const char* GetName() = 0;
		// --setters
		// --core_methods
		virtual bool Init() = 0;
		virtual void OnQuit() = 0;
		virtual void Update() = 0;
		virtual void OnEnable() = 0;
		virtual void OnDisable() = 0;
		virtual void OnEvent(MouseEvent& rmEvt) = 0;
		virtual void OnEvent(KeyboardEvent& rkEvt) = 0;
		virtual void OnEvent(WindowEvent& rwEvt) = 0;
	};
}
#endif	// NW_CORE_STATE_H