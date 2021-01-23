#ifndef NWG_CREATOR_STATE
#define NWG_CREATOR_STATE

#include <core/nw_core_state.h>

namespace NWG
{
	/// CreatorState class
	/// Description:
	class NW_API CreatorState : public NW::CoreState
	{
	public:
		CreatorState();
		~CreatorState();

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

#endif	// NWG_CREATOR_STATE