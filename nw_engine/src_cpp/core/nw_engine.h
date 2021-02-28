#ifndef NW_ENGINE_H
#define NW_ENGINE_H
#include <nw_core.hpp>
#include <core/nw_window.h>
#include <core/nw_engine_states.h>;
namespace NW
{
	/// CoreEngine class
	/// Description:
	/// Interface:
	class NW_API CoreEngine : public AMemUser
	{
	public:
		using State = AEngineState;
		using States = DArray<RefKeeper<State>>;
	public:
		CoreEngine(const char* strName = "nw_engine");
		CoreEngine(const CoreEngine& rCpy) = delete;
		virtual ~CoreEngine();
		// --getters
		inline const char* GetName()	{ return &m_strName[0]; }
		inline Thread* GetRunThread()	{ return &m_thrRun; }
		inline Keyboard* GetKeyboard()	{ return &m_Keyboard; }
		inline Cursor* GetCursor()		{ return &m_Cursor; }
		inline CoreWindow* GetWindow()	{ return m_Window; }
		inline GfxEngine* GetGfx()		{ return m_Graphics; }
		inline States& GetStates()		{ return m_States; }
		inline RefKeeper<State>& GetState(UInt32 unIdx) { return m_States[unIdx]; }
		// --setters
		template<class SType, typename ... Args>
		RefKeeper<State>& AddState(Args&& ... Arguments);
		void AddState(RefKeeper<State>& rState);
		void RmvState(UInt8 nIdx);
		void StopRunning();
		// --predicates
		inline Bit IsRunning() const { return m_bIsRunning; }
		// --operators
		void operator=(const CoreEngine& rCpy) = delete;
		// --core_methods
		bool Init();
		void Quit();
		void Run();
		void Update();
		void OnEvent(AEvent& rEvt);
		// --data_methods
		String DialogLoad(const char* strFilter);
		String DialogSave(const char* strFilter);
	private:
		const char* m_strName;
		Bit m_bIsRunning;
		Thread m_thrRun;
		Mutex m_mtxRun;
		States m_States;
		Keyboard m_Keyboard;
		Cursor m_Cursor;
		RefKeeper<CoreWindow> m_Window;
		RefKeeper<GfxEngine> m_Graphics;
	};
	template<class SType, typename ... Args>
	RefKeeper<CoreEngine::State>& CoreEngine::AddState(Args&& ... Arguments) {
		m_mtxRun.lock();
		m_States.push_back(RefKeeper<AEngineState>());
		m_States.back().MakeRef<SType>(*this, std::forward<Args>(Arguments)...);
		m_mtxRun.unlock();
		return m_States.back();
	}
}

#endif	// NW_ENGINE_H