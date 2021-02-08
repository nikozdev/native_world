#ifndef GFX_ENGINE_STATES_H
#define GFX_ENGINE_STATES_H

#include <gfx_core.hpp>

#include <nwlib/nwl_engine.h>

namespace NW
{
	/// GfxState class
	class GfxState : public NW::AEngineState
	{
	public:
		GfxState();
		~GfxState();
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
		GfxEngine& m_rGraph;

		RefKeeper<ShaderBuf> m_pShdBuf;
	};
}
namespace NW
{
	/// GfxTestState class
	class GfxTestState : public NW::AEngineState
	{
	public:
		GfxTestState();
		~GfxTestState();
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
		GfxEngine& m_rGraph;
	};
}

#endif	// GFX_ENGINE_STATES_H