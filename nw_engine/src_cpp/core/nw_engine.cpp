#include <nw_pch.hpp>
#include <core/nw_engine.h>
namespace NW
{
	CoreEngine::CoreEngine(const char* strName) :
		m_strName(&strName[0]),
		m_bIsRunning(false), m_thrRun(Thread()), m_mtxRun(Mutex()),
		m_Graphics(RefKeeper<GfxEngine>()),
		m_Window(RefKeeper<CoreWindow>()),
		m_Keyboard(Keyboard()), m_Cursor(Cursor())
	{
		MemSys::OnInit(1 << 23);
		DataSys::OnInit();

		TimeSys::OnInit();
		LogSys::OnInit();

		EntSys::OnInit();
		CmpSys::OnInit();
	}
	CoreEngine::~CoreEngine()
	{
		m_Graphics.Reset();
		m_Window.Reset();
		m_States.clear();

		EntSys::OnQuit();
		CmpSys::OnQuit();

		LogSys::OnQuit();
		TimeSys::OnQuit();

		DataSys::OnQuit();
		MemSys::OnQuit();
	}
	// --setters
	void CoreEngine::AddState(RefKeeper<State>& rState) { m_States.push_back(rState); }
	void CoreEngine::RmvState(UInt8 nIdx) { if (m_States.size() <= nIdx) { return; } m_States.erase(m_States.begin() + nIdx); }
	void CoreEngine::StopRunning() { m_bIsRunning = false; }
	// --==<core_methods>==--
	bool CoreEngine::Init()
	{
		if (m_Window.IsValid() || m_Graphics.IsValid()) { return false; }
		if (!m_bIsRunning) { m_bIsRunning = true; }

		m_Window.MakeRef<CoreWindow>(WindowInfo{ &m_strName[0], 1200, 800, true });
		m_Window->SetEventCallback([this](AEvent& rEvt)->void { return this->OnEvent(rEvt); });

		GfxEngine::Create(m_Graphics, m_Window->GetNative());
		m_Graphics->SetViewport(0, 0, m_Window->GetSizeW(), m_Window->GetSizeH());
		m_Graphics->SetVSync(true);

		for (auto& itState : m_States) { if (!itState->Init()) { return false; } }
		
		return true;
	}
	void CoreEngine::Quit()
	{
		if (!m_Window.IsValid() || !m_Graphics.IsValid()) { return; }
		if (!m_bIsRunning) { m_bIsRunning = true; }
		
		for (auto& itState : m_States) { itState->Quit(); }

		m_Graphics.Reset();
		m_Window.Reset();
	}
	void CoreEngine::Run()
	{
		m_bIsRunning = true;
		auto fnRunning = [this]()->void {
			if (!Init()) { return; }
			while (m_bIsRunning) { Update(); }
			Quit();
		};
		m_thrRun = Thread(fnRunning);
	}
	void CoreEngine::Update()
	{
		m_mtxRun.lock();
		
		for (auto& itState : m_States) { itState->Update(); }
		
		LogSys::Update();
		TimeSys::Update();

		m_Window->Update();
		m_Graphics->Update();

		m_mtxRun.unlock();
	}
	void CoreEngine::OnEvent(AEvent& rEvt)
	{
		if (rEvt.IsInCategory(EVC_CURSOR)) {
			CursorEvent* pmEvt = static_cast<CursorEvent*>(&rEvt);
			switch (pmEvt->evType) {
			case EVT_CURSOR_MOVE: {
				m_Cursor.xMoveDelta = pmEvt->nX - m_Cursor.xMove;
				m_Cursor.yMoveDelta = pmEvt->nY - m_Cursor.yMove;
				m_Cursor.xMove = pmEvt->nX;
				m_Cursor.yMove = pmEvt->nY;
				if (m_Cursor.GetHeld(pmEvt->cButton)) {
					m_Cursor.Buttons[pmEvt->cButton].xHeldDelta = m_Cursor.xMove - m_Cursor.Buttons[pmEvt->cButton].xHeld;
					m_Cursor.Buttons[pmEvt->cButton].yHeldDelta = m_Cursor.yMove - m_Cursor.Buttons[pmEvt->cButton].yHeld;
				}
				break;
			}
			case EVT_CURSOR_SCROLL: m_Cursor.xScrollDelta = pmEvt->nX;
				m_Cursor.yScrollDelta = pmEvt->nY;
				break;
			case EVT_CURSOR_PRESS: {
				auto& rBtn = m_Cursor.Buttons[pmEvt->cButton];
				rBtn.bState = rBtn.bState == BS_PRESSED ? BS_HELD : BS_PRESSED;
				rBtn.xHeld = m_Cursor.xMove;
				rBtn.yHeld = m_Cursor.yMove;
				break;
			}
			case EVT_CURSOR_RELEASE: {
				auto& rBtn = m_Cursor.Buttons[pmEvt->cButton];
				rBtn.bState = BS_RELEASED;
				rBtn.xHeldDelta = 0.0;
				rBtn.yHeldDelta = 0.0;
				break;
			}
			default: break;
			}
			for (auto& itState : m_States) { if (rEvt.bIsHandled) return; itState->OnEvent(*pmEvt); }
		}
		else if (rEvt.IsInCategory(EVC_KEYBOARD)) {
			KeyboardEvent* pkEvt = static_cast<KeyboardEvent*>(&rEvt);
			switch (pkEvt->evType) {
			case EVT_KEYBOARD_PRESS: {
				auto& rKey = m_Keyboard.Keys[pkEvt->keyCode];
				if (rKey.bState == BS_PRESSED) {
					rKey.bState = BS_PRESSED;
					rKey.unRepeats = 1;
				}
				else {
					rKey.bState = BS_HELD;
					rKey.unRepeats++;
				}
				break;
			}
			case EVT_KEYBOARD_RELEASE: {
				auto& rKey = m_Keyboard.Keys[pkEvt->keyCode];
				rKey.bState = BS_RELEASED;
				rKey.unRepeats = 0;
				switch (pkEvt->keyCode) {
				case KC_V:
					if (m_Keyboard.GetHeld(KC_LCTRL)) { m_Graphics->SetVSync(!m_Graphics->IsVSync()); }
					break;
				case KC_ESCAPE:
					StopRunning();
					rEvt.bIsHandled = true;
					break;
				case KC_M:
					m_Cursor.crsMode = m_Cursor.crsMode == CRS_DEFAULT ? CRS_CAPTURED : CRS_DEFAULT;
					m_Window->SetCursorMode(m_Cursor.crsMode);
					break;
				default: break;
				}
				break;
			}
			case EVT_KEYBOARD_CHAR: {
				break;
			}
			}
			for (auto& itState : m_States) { if (rEvt.bIsHandled) { return; } itState->OnEvent(*pkEvt); }
		}
		else if (rEvt.IsInCategory(EVC_WINDOW)) {
			WindowEvent* pwEvt = static_cast<WindowEvent*>(&rEvt);
			switch (pwEvt->evType) {
			case EVT_WINDOW_RESIZE: {
				break;
			}
			case EVT_WINDOW_MOVE: {
				break;
			}
			case EVT_WINDOW_FOCUS: {
				break;
			}
			case EVT_WINDOW_DEFOCUS: {
				break;
			}
			case EVT_WINDOW_CLOSE: {
				StopRunning();
				rEvt.bIsHandled = true;
				break;
			}
			}
			for (auto& itState : m_States) { if (rEvt.bIsHandled) { return; } itState->OnEvent(*pwEvt); }
		}
	}

	String CoreEngine::DialogLoad(const char* strFilter) { return DataSys::DialogLoad(strFilter, m_Window->GetNative()); }
	String CoreEngine::DialogSave(const char* strFilter) { return DataSys::DialogSave(strFilter, m_Window->GetNative()); }
	// --==</core_methods>==--
}