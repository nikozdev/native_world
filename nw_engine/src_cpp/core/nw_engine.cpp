#include <nw_pch.hpp>
#include <core/nw_engine.h>

#include <core/nw_engine_state.h>

#include <gl/control/nw_drawer.h>
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
	Engine::Engine() :
		m_strName("NW_Application"),
		m_bIsRunning(false),
		m_pCurrState(nullptr),
		m_pWindow(nullptr)
	{ }
	Engine::~Engine()
	{ }

	// -- Setters
	EngineState* Engine::GetState(const char* strName) {
		auto itState = FIND_BY_NAME(m_States, EngineState*, strName, ->GetName);
		return itState == m_States.end() ? nullptr : *itState;
	}
	// -- Setters
	void Engine::AddState(EngineState* pState)
	{
		if (pState == nullptr) { return; }
		States::iterator itState = std::find(m_States.begin(), m_States.end(), pState);
		if (itState != m_States.end()) { return; }
		m_States.push_back(pState);
		if (!pState->Init()) { Quit(); }
		if (m_pCurrState == nullptr) { SwitchState(pState->GetName()); }
	}
	void Engine::RemoveState(const char* strName)
	{
		States::iterator itState = FIND_BY_FUNC(m_States, EngineState*, strName, ->GetName);
		if (itState == m_States.end()) return;
		EngineState* pState = *itState;
		pState->OnDisable();
		m_States.erase(itState);
		if (m_pCurrState == pState) {
			m_pCurrState = nullptr;
			if (m_States.size() > 0) { SwitchState((*m_States.begin())->GetName()); }
			else { m_pCurrState = nullptr; Quit(); }
		}
	}
	void Engine::SwitchState(const char* strName)
	{
		States::iterator itState = FIND_BY_FUNC(m_States, EngineState*, strName, ->GetName);
		if (itState == m_States.end()) { return; }
		if (m_pCurrState != nullptr) { m_pCurrState->OnDisable(); }
		m_pCurrState = *itState;
		m_pCurrState->OnEnable();
	}

	// ========<Core Methods>========
	bool Engine::Init()
	{
		WindowInfo WindowInfo{ &m_strName[0], 800, 600, true, nullptr };
		m_pWindow.reset(AWindow::Create(WindowInfo, WApiTypes::WAPI_GLFW));
		if (!m_pWindow->Init()) {
			LogSys::WriteErrStr(NW_ERR_NO_INIT, "Window is not initialized!");
			m_pWindow->OnQuit();
			return false;
		}
		m_pWindow->SetEventCallback(NW_BIND_FN(Engine::OnEvent));
	#if (defined NW_GRAPHICS)
		GApiTypes GApiType = GApiTypes::GAPI_NONE;
		#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		GApiType = GApiTypes::GAPI_OPENGL;
		#elif (NW_GRAPHICS & NW_GRAPHICS_COUT)
		GApiType = GApiTypes::GAPI_COUT;
		#endif
	#endif	// NW_GRAPHICS
		if (!Drawer::Init(GApiType)) {
			LogSys::WriteErrStr(NW_ERR_NO_INIT, "Drawer is not initialized!");
			Drawer::OnQuit();
			m_pWindow->OnQuit();
			return false;
		}

		EvSys::OnInit();
		DataSys::OnInit();
		GuiSys::OnInit();

		return true;
	}
	void Engine::Quit()
	{
		m_bIsRunning = false;
	}
	void Engine::Run()
	{
		// -- Preparation
		m_bIsRunning = true;
		if (m_pCurrState == nullptr) { AddState(MemSys::NewT<EngineState>()); }
		// -- Main Loop
		while (m_bIsRunning) { Update(); }
		// -- Quit code
		while (!m_States.empty()) {
			(*m_States.begin())->OnDisable();
			MemSys::DelT<EngineState>(*m_States.begin());
			m_States.erase(m_States.begin());
		}
		DataSys::OnQuit();
		GuiSys::OnQuit();

		Drawer::OnQuit();
		m_pWindow->OnQuit();

		system("\a");
		LogSys::WriteStr("NW_Engine has been quited");
	}
	inline void Engine::Update()
	{	
		GuiSys::BeginDraw();
		GuiSys::Update();
		m_pCurrState->Update();
		GuiSys::EndDraw();

		m_pWindow->Update();
		Drawer::Update();
	
		IOSys::Update();
		TimeSys::Update();
		EvSys::Update();
	}
	// ========</Core Methods>========

	// ========<OnEvent Methods>========
	void Engine::OnEvent(AEvent& rEvt)
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
	// ========</OnEvent Methods>========
}