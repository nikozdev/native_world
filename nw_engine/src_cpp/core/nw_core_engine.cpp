#include <nw_pch.hpp>
#include <core/nw_core_engine.h>

#include <glib/core/nw_gengine.h>
#include <glib/gcontext/nw_window.h>

#include <sys/nw_io_sys.h>
#include <sys/nw_time_sys.h>
#include <sys/nw_mem_sys.h>
#include <sys/nw_log_sys.h>
#include <sys/nw_data_sys.h>
#include <sys/nw_gui_sys.h>

namespace NW
{
	CoreEngine::CoreEngine() :
		m_strName("nw_engine"),
		m_bIsRunning(false),
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
	void CoreEngine::RemoveState(const char* strName)
	{
		States::iterator itState = FIND_BY_FUNC(m_States, CoreState*, strName, ->GetName);
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
		States::iterator itState = FIND_BY_FUNC(m_States, CoreState*, strName, ->GetName);
		if (itState == m_States.end()) { return; }
		if (m_pCurrState != nullptr) { m_pCurrState->OnDisable(); }
		m_pCurrState = *itState;
		m_pCurrState->OnEnable();
	}

	// --==<core_methods>==--
	bool CoreEngine::Init()
	{
	#if (defined NW_WINDOW)
		WApiTypes WApiType = WApiTypes::WAPI_NONE;
		#if (NW_WINDOW_GLFW & NW_WINDOW_GLFW)
		WApiType = WApiTypes::WAPI_GLFW;
		#endif
	#endif	// NW_WINDOW
	#if (defined NW_GRAPHICS)
		GApiTypes GApiType = GApiTypes::GAPI_NONE;
		#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		GApiType = GApiTypes::GAPI_OPENGL;
		#endif
	#endif	// NW_GRAPHICS
		if (!GEngine::Get().Init(WApiType, GApiType)) {
			LogSys::WriteErrStr(NW_ERR_NO_INIT, "GEngine is not initialized!");
			return false;
		}
		GEngine::Get().GetWindow()->SetEventCallback([this](AEvent& rEvt)->void { return OnEvent(rEvt); });

		DataSys::OnInit();
		GuiSys::OnInit();

		return true;
	}
	void CoreEngine::Quit()
	{
		if (!m_bIsRunning) { return; }
		m_bIsRunning = false;
		system("\a");
	}

	void CoreEngine::Run()
	{
		m_bIsRunning = true;
		if (m_pCurrState == nullptr) { Quit(); }
		while (m_bIsRunning) { Update(); }

		while (!m_States.empty()) {
			(*m_States.begin())->OnDisable();
			MemSys::DelT<CoreState>(*m_States.begin());
			m_States.erase(m_States.begin());
		}
		DataSys::OnQuit();
		GuiSys::OnQuit();

		GEngine::Get().Quit();


		system("\a");
		LogSys::WriteStr("CoreEngine has been quited");
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
		if (MouseEvent* pmEvt = dynamic_cast<MouseEvent*>(&rEvt)) {
			switch (pmEvt->EvtType) {
			case ET_MOUSE_MOVE:
				IOSys::s_Cursor.xMoveDelta = pmEvt->nX - IOSys::s_Cursor.xMove;
				IOSys::s_Cursor.yMoveDelta = pmEvt->nY - IOSys::s_Cursor.yMove;
				IOSys::s_Cursor.xMove = pmEvt->nX;
				IOSys::s_Cursor.yMove = pmEvt->nY;
				break;
			case ET_MOUSE_SCROLL:
				IOSys::s_xScroll = pmEvt->nX;
				IOSys::s_yScroll = pmEvt->nY;
				break;
			case ET_MOUSE_RELEASE:
				IOSys::s_bsMsBtns[pmEvt->nButton].bNew = false;
			case ET_MOUSE_PRESS:
				IOSys::s_bsMsBtns[pmEvt->nButton].bNew = true;
			}
			if (rEvt.bIsHandled) return;
			m_pCurrState->OnEvent(*pmEvt);
		}
		else if (KeyboardEvent* pkEvt = dynamic_cast<KeyboardEvent*>(&rEvt)) {
			switch (pkEvt->EvtType) {
			case ET_KEY_RELEASE:
				IOSys::s_bsKeys[pkEvt->unKeyCode].bNew = false;
				switch (pkEvt->unKeyCode) {
				case NW_KEY_ESCAPE_27: Quit(); break;
				case NW_KEY_M_77:
					IOSys::SetInputMode(IOSys::s_Cursor.bBlocked ? IM_CURSOR_NORMAL : IM_CURSOR_DISABLED);
					IOSys::s_Cursor.bBlocked = !IOSys::s_Cursor.bBlocked;
					break;
				}
			case ET_KEY_PRESS:
				IOSys::s_bsKeys[pkEvt->unKeyCode].bNew = true;
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