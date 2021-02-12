#ifndef NWG_ENGINE_STATES_H
#define NWG_ENGINE_STATES_H

#include <nwg_pch.hpp>

namespace NWG
{
	/// GamerState class
	class GamerState : public NWL::AEngineState
	{
	public:
		GamerState();
		~GamerState();
		// --core_methods
		virtual bool Init() override;
		virtual void OnQuit() override;
		virtual void Update() override;
		virtual void OnEvent(CursorEvent& rmEvt) override;
		virtual void OnEvent(KeyboardEvent& rkEvt) override;
		virtual void OnEvent(WindowEvent& rwEvt) override;
	private:
		CoreEngine& m_rEngine;
		DArray<UInt32> m_eIds;
	};
}
namespace NWG
{
	/// GuiState class
	/// Description:
	class GuiState : public NWL::AEngineState
	{
	public:
		GuiState();
		~GuiState();
		// --core_methods
		virtual bool Init() override;
		virtual void OnQuit() override;
		virtual void Update() override;
		virtual void OnEvent(CursorEvent& rmEvt) override;
		virtual void OnEvent(KeyboardEvent& rkEvt) override;
		virtual void OnEvent(WindowEvent& rwEvt) override;
	private:
		inline void BeginDraw();
		inline void EndDraw();
	private:
		CoreEngine& m_rEngine;

		ImGuiContext* m_pGuiContext;
		ImGuiIO* m_pGuiIO;
		ImGuiStyle* m_pGuiStyle;

		bool m_bFullScreenPersist;
		bool m_bDockspace;
	};
}

#endif	// NWG_ENGINE_STATES_H