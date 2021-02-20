#ifndef NW_ENGINE_STATES_H
#define NW_ENGINE_STATES_H

#include <nw_core.hpp>
#include <core/nw_gui_of.h>

namespace NW
{
	/// GamerState class
	class GamerState : public NWL::AEngineState
	{
	public:
		GamerState();
		~GamerState();
		// --core_methods
		virtual bool Init() override;
		virtual void Quit() override;
		virtual void Update() override;
		virtual void OnEvent(CursorEvent& rmEvt) override;
		virtual void OnEvent(KeyboardEvent& rkEvt) override;
		virtual void OnEvent(WindowEvent& rwEvt) override;
	private:
		inline void DrawTestImage();
	private:
		CoreEngine& m_rEngine;

		DArray<UInt32> m_eIds;
		ImageInfo m_imgInfo;
		
		RefKeeper<ShaderProgram> m_psProg;
		RefKeeper<IndexedDrawable> m_pDrb;
	};
}
namespace NW
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
		virtual void Quit() override;
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
namespace NW
{
	/// GfxState class
	class GfxState : public NWL::AEngineState
	{
	public:
		GfxState();
		~GfxState();
		// --core_methods
		virtual bool Init() override;
		virtual void Quit() override;
		virtual void Update() override;
		virtual void OnEvent(CursorEvent& rmEvt) override;
		virtual void OnEvent(KeyboardEvent& rkEvt) override;
		virtual void OnEvent(WindowEvent& rwEvt) override;
	private:
		inline void DrawScene();
	private:
		CoreEngine& m_rEngine;
	};
}

#endif	// NW_ENGINE_STATES_H