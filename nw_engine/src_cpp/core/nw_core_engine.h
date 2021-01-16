#ifndef NW_CORE_ENGINE_H
#define NW_CORE_ENGINE_H

#include <nwlib/nw_singleton.h>

#include <core/nw_core_state.h>

#include <glib_decl.hpp>

namespace NW
{
	/// CoreEngine class
	/// Description:
	/// -- Manages
	/// Interface:
	/// -> Get the static instance and Init() that
	/// -> If initialization is successed - add own states
	/// -> Run() command to start the main update loop
	class NW_API CoreEngine : public ASingleton<CoreEngine>
	{
	public:
		using States = DArray<CoreState*>;
		friend class ASingleton<CoreEngine>;
	private:
		CoreEngine();
		CoreEngine(CoreEngine& rCpy) = delete;
		void operator=(CoreEngine& rCpy) = delete;
	public:
		~CoreEngine();

		// --getters
		inline const char* GetName() { return &m_strName[0]; }
		inline States& GetStates() { return m_States; }
		inline CoreState* GetState() { return m_pCurrState; }
		inline CoreState* GetState(const char* strName);
		// --setters
		void SetName(const char* strName) { m_strName = strName; }
		void AddState(CoreState* pState);
		void RemoveState(const char* strName);
		void SwitchState(const char* strName);
		// --predicates
		bool IsRunning() const { return m_bIsRunning; }

		// --core_methods
		bool Init();
		void Run();
		void Quit();
	private:
		inline void Update();
		inline void OnEvent(AEvent& rEvt);
	private:
		String m_strName;
		bool m_bIsRunning;

		States m_States;
		CoreState* m_pCurrState;
	};
	inline CoreState* CoreEngine::GetState(const char* strName) {
		States::iterator itState = std::find_if(m_States.begin(), m_States.end(),
			[=](CoreState* pState)->bool {return strcmp(pState->GetName(), strName) == 0; });
		return itState == m_States.end() ? nullptr : *itState;
	}
}

#endif	// NW_CORE_ENGINE_H