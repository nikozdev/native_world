#ifndef NWG_GAME_STATE
#define NWG_GAME_STATE

#include <core/nw_core_state.h>

namespace NWG
{
	/// GameState class
	/// Description:
	/// -- This is the main interaction part for the engine.
	/// -- State handles Lua virtual machiene which allows to use Lua scripts
	class NW_API GameState : public NW::CoreState
	{
	public:
		GameState();
		~GameState();

		// --getters
		virtual inline const char* GetName() override { return "game_state"; }
		// --setters

		// --core_methods
		virtual bool Init() override;
		virtual void Update() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		// --on_event_methods
		virtual void OnEvent(NW::MouseEvent& rmEvt) override;
		virtual void OnEvent(NW::KeyboardEvent& rkEvt) override;
		virtual void OnEvent(NW::WindowEvent& rwEvt) override;
	};
}

#endif	// NWG_GAME_STATE