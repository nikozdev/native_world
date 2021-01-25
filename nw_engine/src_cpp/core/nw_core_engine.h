#ifndef NW_CORE_ENGINE_H
#define NW_CORE_ENGINE_H

#include <core/nw_core_state.h>
#include <nw_decl.hpp>

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
		inline AMemAllocator& GetMemory() { return m_Memory; }
		inline Thread& GetRunThread() { return m_thrRun; }

		inline const char* GetName() { return &m_strName[0]; }
		inline WApiTypes GetWApiType() { return m_wapiType; }

		inline AWindow* GetWindow() { return m_pWindow.GetRef(); }
		inline States& GetStates() { return m_States; }
		inline CoreState* GetState() { return m_pCurrState; }
		inline CoreState* GetState(const char* strName);
		// --setters
		void SetName(const char* strName) { m_strName = strName; }
		void AddState(CoreState* pState);
		void RmvState(const char* strName);
		void SwitchState(const char* strName);
		// --predicates
		Bit IsRunning() const { return m_bIsRunning; }

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
		inline void DelT(MType* pBlock) { NWL::DelT<MType>(GetMemory(), pBlock); }
		template <typename MType>
		inline void DelTArr(MType* pBlock, UInt64 unDealloc) { NWL::DelTArr<MType>(GetMemory(), pBlock, unDealloc); }
	private:
		Bit m_bIsRunning;
		Thread m_thrRun;
		MemArena m_Memory;

		String m_strName;
		WApiTypes m_wapiType;
		RefOwner<AWindow> m_pWindow;

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