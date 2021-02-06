#ifndef NW_CORE_ENGINE_H
#define NW_CORE_ENGINE_H

#include <core/nw_window.h>

#include <nwlib/nwl_engine.h>

#include <nw_decl.hpp>
#include <glib_core.hpp>

namespace NW
{
	/// CoreEngine class
	/// Description:
	/// -- Manages
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
		inline const char* GetName()	const		{ return &m_strName[0]; }
		inline AppWindow* GetWindow()					{ return m_pWindow.GetRef(); }
		// --setters
		void SetName(const char* strName)			{ strcpy(&m_strName[0], &strName[0]); }

		// --core_methods
		virtual void Run() override;
		virtual bool Init() override;
		virtual void Quit() override;
		virtual void Update() override;
		virtual void OnEvent(AEvent& rEvt) override;
	private:
		Char m_strName[256];
		RefKeeper<AppWindow> m_pWindow;
	};
}

#endif	// NW_CORE_ENGINE_H