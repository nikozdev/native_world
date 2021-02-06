#ifndef NWG_ENGINE_STATES_H
#define NWG_ENGINE_STATES_H

#include <nwlib/nwl_engine.h>
#include <nwg_pch.hpp>

namespace NWG
{
	/// GraphState class
	class GraphState : public NWL::AEngineState
	{
	public:
		GraphState();
		~GraphState();
		// --core_methods
		virtual bool Init() override;
		virtual void OnQuit() override;
		virtual void Update() override;
		virtual void OnEvent(MouseEvent& rmEvt) override;
		virtual void OnEvent(KeyboardEvent& rkEvt) override;
		virtual void OnEvent(WindowEvent& rwEvt) override;
	private:
		inline void DrawScene();
	private:
		GraphEngine& m_rGraph;

		GCamera m_gCamera;
		RefKeeper<ShaderBuf> m_pShdBuf;
	};
}
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
		virtual void OnEvent(MouseEvent& rmEvt) override;
		virtual void OnEvent(KeyboardEvent& rkEvt) override;
		virtual void OnEvent(WindowEvent& rwEvt) override;
	private:
		CoreEngine& m_rCore;
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
		virtual void OnEvent(MouseEvent& rmEvt) override;
		virtual void OnEvent(KeyboardEvent& rkEvt) override;
		virtual void OnEvent(WindowEvent& rwEvt) override;
	private:
		inline void BeginDraw();
		inline void EndDraw();
	private:
		CoreEngine& m_rCore;

		ImGuiContext* m_pGuiContext;
		ImGuiIO* m_pGuiIO;
		ImGuiStyle* m_pGuiStyle;

		bool m_bFullScreenPersist;
		bool m_bDockspace;
	};
}

#endif	// NWG_ENGINE_STATES_H