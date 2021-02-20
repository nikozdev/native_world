#ifndef NW_ENGINE_H
#define NW_ENGINE_H
#include <nw_core.hpp>
#include <core/nw_window.h>
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
		virtual ~CoreEngine();
		// --getters
		inline const char* GetName() const	{ return &m_strName[0]; }
		inline CoreWindow* GetWindow()		{ return m_pWindow; }
		inline GfxEngine* GetGfx()			{ return m_pGfx; }
		// --core_methods
		void Run();
		bool Init();
		void Quit();
		void Update();
		void OnEvent(AEvent& rEvt);
		// --data_methods
		String FDialogLoad(const char* strFilter);
		String FDialogSave(const char* strFilter);
	private:
		const char* m_strName;
		RefKeeper<CoreWindow> m_pWindow;
		RefKeeper<GfxEngine> m_pGfx;
	};
}

#endif	// NW_ENGINE_H