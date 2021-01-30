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
		using States = DArray<CoreState*>;
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
		inline CoreState* GetState(UInt32 unIdx) { return m_States[unIdx]; }
		// --setters
		void SetName(const char* strName) { m_strName = strName; }
		
		inline void AddState(CoreState& rState);
		inline void RmvState(UInt32 unIdx);
		// --predicates
		inline Bit IsRunning() const	{ return m_bIsRunning; }

		// --core_methods
		void Run();
		bool Init();
		void Quit();
		void Update();
		void OnEvent(AEvent& rEvt);
		// --memory_methods
		template <typename MType, typename...Args>
		inline MType* NewT(Args...Arguments) { return NWL::NewT<MType>(GetMemory(), Arguments...); }
		template <typename MType>
		inline MType* NewTArr(UInt64 unAlloc) { return NWL::NewTArr<MType>(GetMemory(), unAlloc); }
		template <typename MType>
		inline void DelT(MType* pBlock) { NWL::DelT<MType>(GetMemory(), pBlock); }
		template <typename MType>
		inline void DelTArr(MType* pBlock, UInt64 unDealloc) { NWL::DelTArr<MType>(GetMemory(), pBlock, unDealloc); }
	private:
		Bit m_bIsRunning;
		Thread m_thrRun;
		MemArena m_Memory;

		String m_strName;

		RefKeeper<AWindow> m_pWindow;
		States m_States;
	};
	// --setters
	inline void CoreEngine::AddState(CoreState& rState) { m_States.push_back(&rState); }
	inline void CoreEngine::RmvState(UInt32 unIdx)
	{
		States::iterator& itState = m_States.begin() + unIdx;
		if (itState == m_States.end()) { return; }
		m_States.erase(itState);
	}
}

#endif	// NW_CORE_ENGINE_H