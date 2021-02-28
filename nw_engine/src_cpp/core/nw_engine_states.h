#ifndef NW_ENGINE_STATES_H
#define NW_ENGINE_STATES_H
#include <nw_core.hpp>
namespace NW
{
	/// AEngineState class
	/// Description:
	/// -- This is the main interaction part for the engine.
	/// -- State handles Lua virtual machiene which allows to use Lua scripts
	class NWL_API AEngineState : public AMemUser
	{
	public:
		AEngineState(CoreEngine& rEngine);
		virtual ~AEngineState();
		// --getters
		virtual inline const char* GetName() = 0;
		// --setters
		// --core_methods
		virtual bool Init() = 0;
		virtual void Quit() = 0;
		virtual void Update() = 0;
		virtual void OnEvent(CursorEvent& rmEvt) = 0;
		virtual void OnEvent(KeyboardEvent& rkEvt) = 0;
		virtual void OnEvent(WindowEvent& rwEvt) = 0;
	protected:
		CoreEngine* m_pEngine;
	};
}
namespace NW
{
	/// GamerState class
	class GamerState : public AEngineState
	{
	public:
		GamerState(CoreEngine& rEngine);
		~GamerState();
		// --getters
		virtual inline const char* GetName() override { return "gamer_state"; }
		// --core_methods
		virtual bool Init() override;
		virtual void Quit() override;
		virtual void Update() override;
		virtual void OnEvent(CursorEvent& rmEvt) override;
		virtual void OnEvent(KeyboardEvent& rkEvt) override;
		virtual void OnEvent(WindowEvent& rwEvt) override;
	};
}
struct ImGuiContext;
struct ImGuiIO;
struct ImGuiStyle;
namespace NW
{
	/// GuiState class
	/// Description:
	class GuiState : public AEngineState
	{
	public:
		GuiState(CoreEngine& rEngine);
		~GuiState();
		// --getters
		virtual inline const char* GetName() override { return "gui_state"; }
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
	class GfxState : public AEngineState
	{
	public:
		GfxState(CoreEngine& rEngine);
		~GfxState();
		// --getters
		virtual inline const char* GetName() override { return "gfx_state"; }
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
		GfxEngine* m_pGfx;
	};
}

#endif	// NW_ENGINE_STATES_H