#ifndef CHEER_NIK_ENGINE_H
#define CHEER_NIK_ENGINE_H

#include <core/nw_singleton.h>

#include <nw_decl.hpp>
#include <gl_decl.hpp>

namespace NW
{
	/// Engine class
	/// Description:
	/// -- Manages
	/// Interface:
	/// -> Get the static instance and Init() that
	/// -> If initialization is successed - add own states
	/// -> Run() command to start the main update loop
	class NW_API Engine : public ASingleton<Engine>
	{
	public:
		using States = DArray<EngineState*>;
		friend class ASingleton<Engine>;
	private: // engine-only attribs
		String m_strName;
		bool m_bIsRunning;

		RefOwner<AWindow> m_pWindow;

		States m_States;
		EngineState* m_pCurrState;
	private:
		Engine();
		Engine(Engine& rCpy) = delete;
		void operator=(Engine& rCpy) = delete;
	public:
		~Engine();

		// -- Getters
		inline const char* GetName() { return &m_strName[0]; }
		inline AWindow* GetWindow() { return m_pWindow.get(); }
		inline States& GetStates() { return m_States; }
		inline EngineState* GetState() { return m_pCurrState; }
		EngineState* GetState(const char* strName);
		// -- Setters
		void SetName(const char* strName) { m_strName = strName; }
		void AddState(EngineState* pState);
		void RemoveState(const char* strName);
		void SwitchState(const char* strName);
		// -- Predicates
		bool IsRunning() const { return m_bIsRunning; }

		// -- Core Methods
		bool Init();
		void Run();
		void Quit();
	private:
		inline void Update();
		inline void OnEvent(AEvent& rEvt);
	};
}

#endif	// CHEER_NIK_ENGINE_H