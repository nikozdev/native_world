#include <nw_pch.hpp>
#include <core/nw_core_engine.h>

#include <core/nw_window.h>
#include <glib/core/nw_gengine.h>

#include <sys/nw_io_sys.h>
#include <sys/nw_time_sys.h>
#include <sys/nw_log_sys.h>
#include <sys/nw_data_sys.h>
#include <sys/nw_gui_sys.h>

namespace NW
{
	CoreEngine::CoreEngine() :
		m_bIsRunning(false), m_thrRun(Thread()), m_Memory(nullptr, 0),
		m_strName("nw_engine"), m_wapiType(WApiTypes::WAPI_NONE),
		m_pCurrState(nullptr) { }
	CoreEngine::~CoreEngine() { }

	// --setters
	void CoreEngine::AddState(CoreState* pState)
	{
		if (pState == nullptr) { return; }
		States::iterator itState = std::find(m_States.begin(), m_States.end(), pState);
		if (itState != m_States.end()) { return; }
		m_States.push_back(pState);
		if (!pState->Init()) { Quit(); }
		if (m_pCurrState == nullptr) { SwitchState(pState->GetName()); }
	}
	void CoreEngine::RmvState(const char* strName)
	{
		States::iterator itState = find_if(m_States.begin(), m_States.end(),
			[=](CoreState* pState)->bool { return strcmp(strName, &pState->GetName()[0]) == 0; });
		if (itState == m_States.end()) return;
		CoreState* pState = *itState;
		pState->OnDisable();
		m_States.erase(itState);
		if (m_pCurrState == pState) {
			m_pCurrState = nullptr;
			if (m_States.size() > 0) { SwitchState((*m_States.begin())->GetName()); }
			else { m_pCurrState = nullptr; Quit(); }
		}
	}
	void CoreEngine::SwitchState(const char* strName)
	{
		States::iterator itState = find_if(m_States.begin(), m_States.end(),
			[=](CoreState* pState)->bool { return strcmp(strName, &pState->GetName()[0]) == 0; }); if (itState == m_States.end()) { return; }
		if (m_pCurrState != nullptr) { m_pCurrState->OnDisable(); }
		m_pCurrState = *itState;
		m_pCurrState->OnEnable();
	}

	// --==<core_methods>==--
	bool CoreEngine::Init(Size szMem)
	{
		m_Memory = MemArena(new Byte[szMem], szMem);
		m_bIsRunning = true;
	#if (defined NW_WINDOW)
		m_wapiType = WAPI_NONE;
		WindowInfo WindowInfo{ "graphics_engine", 1200, 1200 / 4 * 3, true, nullptr };
		#if (NW_WINDOW_GLFW & NW_WINDOW_GLFW)
		m_wapiType = WAPI_GLFW;
		m_pWindow.MakeRef<WindowOgl>(GetMemory(), WindowInfo);
		if (!m_pWindow->Init()) { m_pWindow->OnQuit(); return false; }
		#endif
	#endif	// NW_WINDOW

		if (!GEngine::Get().Init(szMem >> 2)) { LogSys::WriteErrStr(NWL_ERR_NO_INIT, "GEngine is not initialized!"); return false; }
		m_pWindow->SetEventCallback([this](AEvent& rEvt)->void { return OnEvent(rEvt); });
		DataSys::OnInit();
		GuiSys::OnInit();

		return true;
	}
	void CoreEngine::Quit()
	{
		if (!m_bIsRunning) { return; }
		m_bIsRunning = false;

		while (!m_States.empty()) { (*m_States.begin())->OnDisable(); m_States.erase(m_States.begin()); }
		DataSys::OnQuit();
		GuiSys::OnQuit();
		GEngine::Get().Quit();

		m_pWindow->OnQuit();
		delete[] m_Memory.GetDataBeg();
		m_Memory = MemArena(nullptr, 0);
	}

	void CoreEngine::Run()
	{
		m_bIsRunning = true;
		if (m_pCurrState == nullptr) { Quit(); }
		
		// m_thrRun = std::thread([this]()->void { while (m_bIsRunning) { Update(); } });
		while (m_bIsRunning) { Update(); };
		Quit();
	}
	inline void CoreEngine::Update()
	{
		GuiSys::BeginDraw();
		GuiSys::Update();
		
		m_pCurrState->Update();
		
		GuiSys::EndDraw();
	
		IOSys::Update();
		TimeSys::Update();

		GEngine::Get().Update();
	}
	// --==</core_methods>==--

	// --==<--on_event_methods>==--
	void CoreEngine::OnEvent(AEvent& rEvt)
	{
		// Dispatch particular events
		if (rEvt.EvtType == ET_MOUSE_MOVE || rEvt.EvtType == ET_MOUSE_SCROLL || rEvt.EvtType == ET_MOUSE_RELEASE || rEvt.EvtType == ET_MOUSE_PRESS) {
			MouseEvent* pmEvt = static_cast<MouseEvent*>(&rEvt);
			switch (pmEvt->EvtType) {
			case ET_MOUSE_MOVE:
				IOSys::s_Mouse.xMoveDelta = pmEvt->nX - IOSys::s_Mouse.xMove;
				IOSys::s_Mouse.yMoveDelta = pmEvt->nY - IOSys::s_Mouse.yMove;
				IOSys::s_Mouse.xMove = pmEvt->nX;
				IOSys::s_Mouse.yMove = pmEvt->nY;
				break;
			case ET_MOUSE_SCROLL:
				IOSys::s_Mouse.xScroll = pmEvt->nX;
				IOSys::s_Mouse.yScroll = pmEvt->nY;
				break;
			case ET_MOUSE_RELEASE:
				IOSys::s_Mouse.bsButtons[pmEvt->nButton].bNew = false;
			case ET_MOUSE_PRESS:
				IOSys::s_Mouse.bsButtons[pmEvt->nButton].bNew = true;
			}
			if (rEvt.bIsHandled) return;
			m_pCurrState->OnEvent(*pmEvt);
		}
		else if (KeyboardEvent* pkEvt = dynamic_cast<KeyboardEvent*>(&rEvt)) {
			switch (pkEvt->EvtType) {
			case ET_KEY_RELEASE:
				IOSys::s_Keyboard.bsKeys[pkEvt->unKeyCode].bNew = false;
				switch (pkEvt->unKeyCode) {
				case NW_KEY_ESCAPE_27: Quit(); break;
				case NW_KEY_M_77:
					IOSys::SetCursorIMode(IOSys::s_Mouse.iMode == IM_CURSOR_NORMAL ? IM_CURSOR_DISABLED : IM_CURSOR_NORMAL);
					break;
				}
			case ET_KEY_PRESS:
				IOSys::s_Keyboard.bsKeys[pkEvt->unKeyCode].bNew = true;
				break;
			}
			if (rEvt.bIsHandled) return;
			m_pCurrState->OnEvent(*pkEvt);
		}
		else if (WindowEvent* pwEvt = dynamic_cast<WindowEvent*>(&rEvt)) {
			switch (pwEvt->EvtType) {
			case ET_WINDOW_CLOSE: Quit(); break;
			}
			if (rEvt.bIsHandled) return;
			m_pCurrState->OnEvent(*pwEvt);
		}
	}
	// --==</--on_event_methods>==--
}