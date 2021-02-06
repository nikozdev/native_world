#include <nw_pch.hpp>
#include <core/nw_core_engine.h>

#include <core/nw_window.h>

#include <sys/nw_io_sys.h>
#include <sys/nw_data_sys.h>

#include <glib_engine.h>
//#include <cmd_engine.h>

namespace NW
{
	CoreEngine::CoreEngine() :
		AEngine(),
		m_strName("nw_engine"),
		m_pWindow(RefKeeper<AppWindow>()) { }
	CoreEngine::~CoreEngine() { }

	// --==<core_methods>==--
	void CoreEngine::Run()
	{
		Init();
		if (!m_bIsRunning) { return; }
		if (m_States.empty()) { Quit(); }

		auto fnRunning = [this]()->void {
			//CMD::CmdEngine::Get().Run();
			GLIB::GraphEngine::Get().Run();
			while (m_bIsRunning) { Update(); }
			GLIB::GraphEngine::Get().Quit();
			GLIB::GraphEngine::Get().GetRunThread().join();
			//CMD::CmdEngine::Get().Quit();
			//CMD::CmdEngine::Get().GetRunThread().join();
			Quit();
		};
		m_thrRun = Thread(fnRunning);
	}
	bool CoreEngine::Init()
	{
		if (m_bIsRunning) { return false; }
		m_Memory = MemArena(new Byte[1 << 18], 1 << 18);
	#if (defined NW_WINDOW)
		WindowInfo wInfo{ &m_strName[0], 1200, 800, true, nullptr };
		#if (NW_WINDOW_GLFW & NW_WINDOW_GLFW)
		wInfo.wapiType = WAPI_GLFW;
		#endif
		AppWindow::Create(wInfo, m_pWindow);
		if (!m_pWindow->Init()) { m_pWindow->OnQuit(); return false; }
	#endif	// NW_WINDOW
		m_pWindow->SetEventCallback([this](AEvent& rEvt)->void { return OnEvent(rEvt); });
		DataSys::OnInit();

		if (!GLIB::GraphEngine::Get().Init()) { return false; }
		//if (!CMD::CmdEngine::Get().Init()) { return false; }
		
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

		GLIB::GraphEngine::Get().Quit();
		//CMD::CmdEngine::Get().Quit();

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
		
		GLIB::GraphEngine::Get().Update();
		//CMD::CmdEngine::Get().Update();

		m_pWindow->Update();
	}
	void CoreEngine::OnEvent(AEvent& rEvt)
	{
		if (rEvt.IsInCategory(EC_MOUSE)) {
			MouseEvent* pmEvt = static_cast<MouseEvent*>(&rEvt);
			switch (pmEvt->evType) {
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
			for (auto& itState : m_States) { if (rEvt.bIsHandled) return; itState->OnEvent(*pmEvt); }
		}
		else if (rEvt.IsInCategory(EC_KEYBOARD)) {
			KeyboardEvent* pkEvt = static_cast<KeyboardEvent*>(&rEvt);
			switch (pkEvt->evType) {
			case ET_KEY_RELEASE:
				IOSys::s_Keyboard.bsKeys[pkEvt->unKeyCode].bNew = false;
				switch (pkEvt->unKeyCode) {
				case NW_KEY_ESCAPE_27:
					StopRunning();
					rEvt.bIsHandled = true;
				case NW_KEY_M_77:
					IOSys::SetCursorIMode(IOSys::s_Mouse.iMode == IM_CURSOR_NORMAL ? IM_CURSOR_DISABLED : IM_CURSOR_NORMAL);
					break;
				default: break;
				}
				break;
			case ET_KEY_PRESS:
				IOSys::s_Keyboard.bsKeys[pkEvt->unKeyCode].bNew = true;
				switch (pkEvt->unKeyCode) {
				case NW_KEY_ESCAPE_27:
				default: break;
				}
				break;
			case ET_KEY_CHAR:
				break;
			}
			for (auto& itState : m_States) { if (rEvt.bIsHandled) { return; } itState->OnEvent(*pkEvt); }
		}
		else if (rEvt.IsInCategory(EC_WINDOW)) {
			WindowEvent* pwEvt = static_cast<WindowEvent*>(&rEvt);
			switch (pwEvt->evType) {
			case ET_WINDOW_RESIZE:
				break;
			case ET_WINDOW_MOVE:
				break;
			case ET_WINDOW_FOCUS:
				break;
			case ET_WINDOW_CLOSE:
				StopRunning();
				rEvt.bIsHandled = true;
				break;
			}
			for (auto& itState : m_States) { if (rEvt.bIsHandled) { return; } itState->OnEvent(*pwEvt); }
		}
		GLIB::GraphEngine::Get().OnEvent(rEvt);
	}
	// --==</core_methods>==--
}