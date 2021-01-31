#include <nw_pch.hpp>
#include <core/nw_core_engine.h>

#include <core/nw_window.h>

#include <sys/nw_io_sys.h>
#include <sys/nw_log_sys.h>
#include <sys/nw_data_sys.h>

#include <glib_engine.h>

namespace NW
{
	CoreEngine::CoreEngine() :
		m_bIsRunning(false), m_thrRun(Thread()), m_Memory(nullptr, 0),
		m_strName("nw_engine"),
		m_pWindow(RefKeeper<AWindow>(GetMemory())) { }
	CoreEngine::~CoreEngine() { }

	// --==<core_methods>==--
	void CoreEngine::Run()
	{
		Init();
		if (!m_bIsRunning) { return; }
		if (m_States.empty()) { Quit(); }

		auto fnRunning = [this]()->void {
			while (m_bIsRunning) { Update(); }
			Quit();
		};
		m_thrRun = Thread(fnRunning);
	}
	bool CoreEngine::Init()
	{
		if (m_bIsRunning) { return false; }
		m_Memory = MemArena(new Byte[1 << 18], 1 << 18);
	#if (defined NW_WINDOW)
		WindowInfo wInfo{ "graphics_engine", 1200, 1200 / 4 * 3, true, nullptr };
		#if (NW_WINDOW_GLFW & NW_WINDOW_GLFW)
		wInfo.wapiType = WAPI_GLFW;
		#endif
		AWindow::Create(wInfo, m_pWindow);
		if (!m_pWindow->Init()) { m_pWindow->OnQuit(); return false; }
	#endif	// NW_WINDOW
		m_pWindow->SetEventCallback([this](AEvent& rEvt)->void { return OnEvent(rEvt); });
		DataSys::OnInit();

		if (!GLIB::GEngine::Get().Init()) { return false; }

		for (auto& itState : m_States) { itState->Init(); }

		return (m_bIsRunning = true);
	}
	void CoreEngine::Quit()
	{
		if (!m_bIsRunning && m_pWindow.GetRef() == nullptr) { return; }
		m_bIsRunning = false;

		for (auto& itState : m_States) { itState->OnQuit(); }
		m_States.clear();

		DataSys::OnQuit();

		GLIB::GEngine::Get().Quit();
		
		m_pWindow->OnQuit();
		m_pWindow.Reset();

		delete[] m_Memory.GetDataBeg();
		m_Memory = MemArena(nullptr, 0);
	}

	void CoreEngine::Update()
	{
		for (auto& itState : m_States) { itState->Update(); }
	
		IOSys::Update();
		TimeSys::Update();
		
		m_pWindow->Update();
	}
	void CoreEngine::OnEvent(AEvent& rEvt)
	{
		// Dispatch particular events
		if (rEvt.IsInCategory(EC_MOUSE)) {
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
			break;
			case ET_MOUSE_PRESS:
				IOSys::s_Mouse.bsButtons[pmEvt->nButton].bNew = true;
			break;
			}
			if (rEvt.bIsHandled) return;
			for (auto& itState : m_States) { itState->OnEvent(*pmEvt); }
		}
		else if (rEvt.IsInCategory(EC_KEYBOARD)) {
			KeyboardEvent* pkEvt = static_cast<KeyboardEvent*>(&rEvt);
			switch (pkEvt->EvtType) {
			case ET_KEY_RELEASE:
				IOSys::s_Keyboard.bsKeys[pkEvt->unKeyCode].bNew = false;
				switch (pkEvt->unKeyCode) {
				case NW_KEY_ESCAPE_27:
					m_bIsRunning = false;
					rEvt.bIsHandled = true;
					break;
				case NW_KEY_M_77:
					IOSys::SetCursorIMode(IOSys::s_Mouse.iMode == IM_CURSOR_NORMAL ? IM_CURSOR_DISABLED : IM_CURSOR_NORMAL);
					break;
				}
				break;
			case ET_KEY_PRESS:
				IOSys::s_Keyboard.bsKeys[pkEvt->unKeyCode].bNew = true;
				break;
			case ET_KEY_CHAR:
				break;
			}
			if (rEvt.bIsHandled) { return; }
			for (auto& itState : m_States) { itState->OnEvent(*pkEvt); }
		}
		else if (rEvt.IsInCategory(EC_WINDOW)) {
			WindowEvent* pwEvt = static_cast<WindowEvent*>(&rEvt);
			switch (pwEvt->EvtType) {
			case ET_WINDOW_RESIZE:
				break;
			case ET_WINDOW_MOVE:
				break;
			case ET_WINDOW_FOCUS:
				break;
			case ET_WINDOW_CLOSE:
				m_bIsRunning = false;
				rEvt.bIsHandled = true;
				break;
			}
			if (rEvt.bIsHandled) { return; }
			for (auto& itState : m_States) { itState->OnEvent(*pwEvt); }
		}
	}
	// --==</core_methods>==--
}