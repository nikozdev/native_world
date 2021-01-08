#ifndef NW_CORE_ENGINE_H
#define NW_CORE_ENGINE_H

#include <lib/nw_singleton.h>

#include <nw_decl.hpp>
#include <gl_decl.hpp>

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
	private: // engine-only attribs
		String m_strName;
		bool m_bIsRunning;

		RefOwner<AWindow> m_pWindow;

		States m_States;
		CoreState* m_pCurrState;
	private:
		CoreEngine();
		CoreEngine(CoreEngine& rCpy) = delete;
		void operator=(CoreEngine& rCpy) = delete;
	public:
		~CoreEngine();

		// --getters
		inline const char* GetName() { return &m_strName[0]; }
		inline AWindow* GetWindow() { return m_pWindow.get(); }
		inline States& GetStates() { return m_States; }
		inline CoreState* GetState() { return m_pCurrState; }
		CoreState* GetState(const char* strName);
		// --setters
		void SetName(const char* strName) { m_strName = strName; }
		void AddState(CoreState* pState);
		void RemoveState(const char* strName);
		void SwitchState(const char* strName);
		// -- Predicates
		bool IsRunning() const { return m_bIsRunning; }

		// --core_methods
		bool Init();
		void Run();
		void Quit();
	private:
		inline void Update();
		inline void OnEvent(AEvent& rEvt);
	};
}

#endif	// NW_CORE_ENGINE_H