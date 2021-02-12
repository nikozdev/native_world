#ifndef NW_ENGINE_H
#define NW_ENGINE_H

#include <nw_core.hpp>
#include <core/nw_window.h>
#include <gfx/gfx_api.h>


namespace NW
{
	/// CoreEngine class
	/// Description:
	/// Interface:
	/// -> Get the static instance and Init() that
	/// -> If initialization is successed - add own states
	/// -> Run() command to start the main update loop
	class NW_API CoreEngine : public AEngine<CoreEngine, AEngineState>
	{
	public:
		CoreEngine();
		~CoreEngine();
		// --getters
		inline const char* GetName() const	{ return &m_strName[0]; }
		inline CoreWindow* GetWindow()		{ return m_pWindow.GetRef(); }
		inline GfxApi* GetGfx()				{ return m_pGfx.GetRef(); }
		inline Keyboard& GetKeyboard()		{ return m_Keyboard; }
		inline Cursor& GetCursor()			{ return m_Cursor; }
		// --core_methods
		virtual void Run() override;
		virtual bool Init() override;
		virtual void Quit() override;
		virtual void Update() override;
		virtual void OnEvent(AEvent& rEvt) override;
		// --data_methods
		String FDialogLoad(const char* strFilter);
		String FDialogSave(const char* strFilter);
	private:
		const char* m_strName;
		RefKeeper<CoreWindow> m_pWindow;
		RefKeeper<GfxApi> m_pGfx;
		Keyboard m_Keyboard;
		Cursor m_Cursor;
	};
}

#endif	// NW_ENGINE_H