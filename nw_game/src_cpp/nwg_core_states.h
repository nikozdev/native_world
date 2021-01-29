#ifndef NWG_CORE_STATES_H
#define NWG_CORE_STATES_H

#include <core/nw_core_state.h>
#include <nwg_pch.hpp>

namespace NWG
{
	/// DrawerState class
	/// Description:
	class DrawerState : public NW::CoreState
	{
	public:
		DrawerState();
		~DrawerState();

		// --getters
		virtual inline const char* GetName() override { return "drawer_state"; }
		// --setters

		// --core_methods
		virtual bool Init() override;
		virtual void OnQuit() override;
		virtual void Update() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual void OnEvent(MouseEvent& rmEvt) override;
		virtual void OnEvent(KeyboardEvent& rkEvt) override;
		virtual void OnEvent(WindowEvent& rwEvt) override;
	private:
		GLIB::GCamera m_GCamera;
		NWL::RefOwner<GLIB::AFrameBuf> m_pFmBuf;
		NWL::RefOwner<GLIB::AVertexBuf> m_pVtxBuf;
		NWL::RefOwner<GLIB::AIndexBuf> m_pIdxBuf;
		NWL::RefOwner<GLIB::AShaderBuf> m_pShdBuf;
		NWL::LinearAllocator m_pVtxData;
		NWL::LinearAllocator m_pShdData;
		NWL::LinearAllocator m_pIdxData;
	};
}
namespace NWG
{
	/// GuiState class
	/// Description:
	class GuiState : public NW::CoreState
	{
	public:
		GuiState();
		~GuiState();

		// --getters
		virtual inline const char* GetName() override { return "gui_state"; }
		// --setters

		// --core_methods
		virtual bool Init() override;
		virtual void OnQuit() override;
		virtual void Update() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual void OnEvent(MouseEvent& rmEvt) override;
		virtual void OnEvent(KeyboardEvent& rkEvt) override;
		virtual void OnEvent(WindowEvent& rwEvt) override;
	private:
		ImGuiContext* m_pGuiContext;
		ImGuiIO* m_pGuiIO;
		ImGuiStyle* m_pGuiStyle;
	};
}
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
		virtual inline const char* GetName() override { return "creator_state"; }
		// --setters

		// --core_methods
		virtual bool Init() override;
		virtual void OnQuit() override;
		virtual void Update() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual void OnEvent(MouseEvent& rmEvt) override;
		virtual void OnEvent(KeyboardEvent& rkEvt) override;
		virtual void OnEvent(WindowEvent& rwEvt) override;
	private:
		GuiState m_guiState;
		DrawerState m_drwState;
	};
}

#endif	// NWG_CORE_STATES_H