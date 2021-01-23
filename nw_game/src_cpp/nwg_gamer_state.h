#ifndef NWG_GAMER_STATE
#define NWG_GAMER_STATE

#include <core/nw_core_state.h>

namespace NWG
{
	/// GameState class
	/// Description:
	class NW_API GamerState : public NW::CoreState
	{
	public:
		GamerState();
		~GamerState();

		// --getters
		virtual inline const char* GetName() override { return "game_state"; }
		// --setters

		// --core_methods
		virtual bool Init() override;
		virtual void Update() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		// --on_event_methods
		virtual void OnEvent(MouseEvent& rmEvt) override;
		virtual void OnEvent(KeyboardEvent& rkEvt) override;
		virtual void OnEvent(WindowEvent& rwEvt) override;
	};
}

#endif	// NWG_GAMER_STATE