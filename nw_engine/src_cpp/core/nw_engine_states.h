#ifndef NW_ENGINE_STATES_H
#define NW_ENGINE_STATES_H

#include <nw_core.hpp>
#include <gfx_core.hpp>

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
		virtual void OnQuit() override;
		virtual void Update() override;
		virtual void OnEvent(CursorEvent& rmEvt) override;
		virtual void OnEvent(KeyboardEvent& rkEvt) override;
		virtual void OnEvent(WindowEvent& rwEvt) override;
	private:
		inline void DrawScene();
	private:
		CoreEngine& m_rEngine;

		RefKeeper<ShaderBuf> m_pShdBuf;
	};
}
namespace NW
{
	/// GfxState class
	class GfxDxState : public NWL::AEngineState
	{
	public:
		GfxDxState();
		~GfxDxState();
		// --core_methods
		virtual bool Init() override;
		virtual void OnQuit() override;
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