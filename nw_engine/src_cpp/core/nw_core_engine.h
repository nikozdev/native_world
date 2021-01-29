#ifndef NW_CORE_ENGINE_H
#define NW_CORE_ENGINE_H

#include <core/nw_core_state.h>
#include <core/nw_window.h>
#include <glib_api.h>

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
	class NW_API CoreEngine : public ASingleton<CoreEngine>
	{
	public:
		using States = HashMap<String, CoreState*>;
		friend class ASingleton<CoreEngine>;
	private:
		CoreEngine();
	public:
		~CoreEngine();

		// --getters
		inline AMemAllocator& GetMemory()		{ return m_Memory; }
		inline Thread& GetRunThread()			{ return m_thrRun; }

		inline const char* GetName() const		{ return &m_strName[0]; }

		inline AWindow* GetWindow()				{ return m_pWindow.GetRef(); }
		inline States& GetStates()				{ return m_States; }
		inline CoreState* GetState()			{ return m_itState; }
		inline CoreState* GetState(const char* strName);
		// --setters
		void SetName(const char* strName) { m_strName = strName; }
		
		inline void AddState(CoreState& rState);
		inline void RmvState(const char* strName);
		inline void SwitchState(const char* strName);
		// --predicates
		inline Bit IsRunning() const	{ return m_bIsRunning; }

		// --core_methods
		bool Init(Size szMemory);
		void Quit();
		void Update();
		void Run(Size szMemory);
		void OnEvent(AEvent& rEvt);
		// --memory_methods
		template <typename MType, typename...Args>
		inline MType* NewT(Args...Arguments) { return NWL::NewT<MType>(GetMemory(), Arguments...); }
		template <typename MType>
		inline MType* NewTArr(UInt64 unAlloc) { return NWL::NewTArr<MType>(GetMemory(), unAlloc); }
		template <typename MType>
		inline void DelT(MType* pBlock) { NWL::DelT<MType>(pBlock, GetMemory()); }
		template <typename MType>
		inline void DelTArr(MType* pBlock, UInt64 unDealloc) { NWL::DelTArr<MType>(pBlock, unDealloc, GetMemory()); }
	private:
		Bit m_bIsRunning;
		Thread m_thrRun;
		MemArena m_Memory;

		String m_strName;

		RefKeeper<AWindow> m_pWindow;
		States m_States;
		CoreState* m_itState;
	};
	// --getters
	inline CoreState* CoreEngine::GetState(const char* strName) {
		States::iterator itState = m_States.find(strName);
		return itState == m_States.end() ? nullptr : itState->second;
	}
	// --setters
	inline void CoreEngine::AddState(CoreState& rState) {
		auto& itState = m_States.find(rState.GetName());
		if (itState != m_States.end()) { RmvState(rState.GetName()); }
		m_States[rState.GetName()] = &rState;
	}
	inline void CoreEngine::RmvState(const char* strName)
	{
		States::iterator itState = m_States.find(strName);
		if (itState == m_States.end()) return;
		CoreState* pState = itState->second;
		m_States.erase(itState);
		if (m_itState == pState) {
			m_itState = nullptr;
			if (m_States.size() > 0) { SwitchState(m_States.begin()->second->GetName()); }
			else { m_itState = nullptr; Quit(); }
		}
	}
	inline void CoreEngine::SwitchState(const char* strName)
	{
		States::iterator itState = m_States.find(strName);
		if (itState == m_States.end()) { return; }
		if (m_itState != nullptr) { m_itState->OnDisable(); }
		m_itState = itState->second;
		m_itState->OnEnable();
	}
}

#endif	// NW_CORE_ENGINE_H