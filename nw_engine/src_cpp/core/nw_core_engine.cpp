#include <nw_pch.hpp>
#include <core/nw_core_engine.h>

#include <core/nw_core_state.h>

#include <core/nw_graph_engine.h>
#include <gl/gcontext/nw_window.h>

#include <ecs/nw_scene.h>

#include <sys/nw_ev_sys.h>
#include <sys/nw_io_sys.h>
#include <sys/nw_time_sys.h>
#include <sys/nw_mem_sys.h>
#include <sys/nw_rand_sys.h>
#include <sys/nw_log_sys.h>
#include <sys/nw_data_sys.h>
#include <sys/nw_gui_sys.h>

namespace NW
{
	CoreEngine::CoreEngine() :
		m_strName("NW_Application"),
		m_bIsRunning(false),
		m_pCurrState(nullptr),
		m_pWindow(nullptr)
	{ }
	CoreEngine::~CoreEngine()
	{ }

	// --setters
	CoreState* CoreEngine::GetState(const char* strName) {
		auto itState = FIND_BY_NAME(m_States, CoreState*, strName, ->GetName);
		return itState == m_States.end() ? nullptr : *itState;
	}
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
		WindowInfo WindowInfo{ &m_strName[0], 800, 600, true, nullptr };
		m_pWindow.reset(AWindow::Create(WindowInfo, WApiTypes::WAPI_GLFW));
		if (!m_pWindow->Init()) {
			LogSys::WriteErrStr(NW_ERR_NO_INIT, "Window is not initialized!");
			m_pWindow->OnQuit();
			return false;
		}
		m_pWindow->SetEventCallback([this](AEvent& rEvt)->void { return OnEvent(rEvt); });
	#if (defined NW_GRAPHICS)
		GApiTypes GApiType = GApiTypes::GAPI_NONE;
		#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		GApiType = GApiTypes::GAPI_OPENGL;
		#elif (NW_GRAPHICS & NW_GRAPHICS_COUT)
		GApiType = GApiTypes::GAPI_COUT;
		#endif
	#endif	// NW_GRAPHICS
		if (!GraphEngine::Init(GApiType)) {
			LogSys::WriteErrStr(NW_ERR_NO_INIT, "GraphEngine is not initialized!");
			GraphEngine::OnQuit();
			m_pWindow->OnQuit();
			return false;
		}

		EvSys::OnInit();
		DataSys::OnInit();
		GuiSys::OnInit();

		return true;
	}
	void CoreEngine::Quit()
	{
		m_bIsRunning = false;
	}
	void CoreEngine::Run()
	{
		// -- Preparation
		m_bIsRunning = true;
		if (m_pCurrState == nullptr) { AddState(MemSys::NewT<CoreState>()); }
		// -- Main Loop
		while (m_bIsRunning) { Update(); }
		// -- Quit code
		while (!m_States.empty()) {
			(*m_States.begin())->OnDisable();
			MemSys::DelT<CoreState>(*m_States.begin());
			m_States.erase(m_States.begin());
		}
		DataSys::OnQuit();
		GuiSys::OnQuit();

		GraphEngine::OnQuit();
		m_pWindow->OnQuit();

		system("\a");
		LogSys::WriteStr("NW_CoreEngine has been quited");
	}
	inline void CoreEngine::Update()
	{	
		GuiSys::BeginDraw();
		GuiSys::Update();
		m_pCurrState->Update();
		GuiSys::EndDraw();

		m_pWindow->Update();
		GraphEngine::Update();
	
		IOSys::Update();
		TimeSys::Update();
		EvSys::Update();
	}
	// --==</core_methods>==--

	// --==<--on_event_methods>==--
	void CoreEngine::OnEvent(AEvent& rEvt)
	{
		// Dispatch particular events
		if (MouseEvent* pmEvt = dynamic_cast<MouseEvent*>(&rEvt)) {
			if (rEvt.bIsHandled) return;
			m_pCurrState->OnEvent(*pmEvt);
		} else if (KeyboardEvent* pkEvt = dynamic_cast<KeyboardEvent*>(&rEvt)) {
			switch (pkEvt->EvtType) {
			case ET_KEY_RELEASE:
				KeyboardEvent* pkrEvt = dynamic_cast<KeyboardEvent*>(&rEvt);
				switch (pkrEvt->unKeyCode) {
				case NW_KEY_ESCAPE_27: Quit(); break;
				}
			}
			if (rEvt.bIsHandled) return;
			m_pCurrState->OnEvent(*pkEvt);
		} else if (WindowEvent* pwEvt = dynamic_cast<WindowEvent*>(&rEvt)) {
			switch (pwEvt->EvtType) {
			case ET_WINDOW_CLOSE:
				m_bIsRunning = false;
				break;
				if (rEvt.bIsHandled) return;
				m_pCurrState->OnEvent(*pwEvt);
			}
		}
	}
	// --==</--on_event_methods>==--
}