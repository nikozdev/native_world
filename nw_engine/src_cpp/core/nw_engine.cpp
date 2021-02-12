#include <nw_pch.hpp>
#include <core/nw_engine.h>

namespace NW
{
	CoreEngine::CoreEngine() :
		AEngine(),
		m_strName("nw_engine"),
		m_pWindow(RefKeeper<CoreWindow>()),
		m_Keyboard(Keyboard()), m_Cursor(Cursor()) { }
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
		//m_thrRun = Thread(fnRunning);
		fnRunning();
	}
	bool CoreEngine::Init()
	{
		if (m_bIsRunning) { return false; }
		m_bIsRunning = true;
		
		MemSys::OnInit(1 << 23);
		CmpSys::OnInit();
		EntSys::OnInit();
	
		m_pWindow.MakeRef<CoreWindow>(WindowInfo{ &m_strName[0], 1200, 800, true });
		if (!m_pWindow->Init()) { m_pWindow->OnQuit(); return false; }
		m_pWindow->SetEventCallback([this](AEvent& rEvt)->void { return this->OnEvent(rEvt); });
		
		m_pGfx.MakeRef<GfxApi, CoreWindow&>(*m_pWindow);
		if (!m_pGfx->Init()) { return false; }

		for (auto& itState : m_States) { if (!itState->Init()) { NWL_ERR("State is not initialized"); Quit(); return false; } }
		
		return m_bIsRunning;
	}
	void CoreEngine::Quit()
	{
		if (!m_bIsRunning && m_pWindow.GetRef() == nullptr) { return; }
		m_bIsRunning = false;

		for (auto& itState : m_States) { itState->OnQuit(); }
		m_States.clear();

		m_pGfx->OnQuit();
		m_pGfx.Reset();
		m_pWindow->OnQuit();
		m_pWindow.Reset();
		
		EntSys::OnQuit();
		CmpSys::OnQuit();
		DataSys::GetStorage().clear();
		MemSys::OnQuit();
	}

	void CoreEngine::Update()
	{
		for (auto& itState : m_States) { itState->Update(); }
	
		TimeSys::Update();
		
		m_pGfx->Update();
		m_pWindow->Update();
	}
	void CoreEngine::OnEvent(AEvent& rEvt)
	{
		if (rEvt.IsInCategory(EC_CURSOR)) {
			CursorEvent* pmEvt = static_cast<CursorEvent*>(&rEvt);
			switch (pmEvt->evType) {
			case ET_CURSOR_MOVE:
				m_Cursor.xMove = pmEvt->nX;
				m_Cursor.yMove = pmEvt->nY;
				m_Cursor.xMoveDelta = pmEvt->nX - m_Cursor.xMove;
				m_Cursor.yMoveDelta = pmEvt->nY - m_Cursor.yMove;
				if (m_Cursor.GetHeld(pmEvt->cButton)) {
					m_Cursor.Buttons[pmEvt->cButton].xHeldDelta = m_Cursor.xMove - m_Cursor.Buttons[pmEvt->cButton].xHeld;
					m_Cursor.Buttons[pmEvt->cButton].yHeldDelta = m_Cursor.yMove - m_Cursor.Buttons[pmEvt->cButton].yHeld;
				}
				break;
			case ET_CURSOR_SCROLL:
				m_Cursor.xScrollDelta = pmEvt->nX;
				m_Cursor.yScrollDelta = pmEvt->nY;
				break;
			case ET_CURSOR_PRESS:
			{
				auto& rBtn = m_Cursor.Buttons[pmEvt->cButton];
				rBtn.bState = rBtn.bState == BS_PRESSED ? BS_HELD : BS_PRESSED;
				rBtn.xHeld = m_Cursor.xMove;
				rBtn.yHeld = m_Cursor.yMove;
				break;
			}
			case ET_CURSOR_RELEASE:
			{
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
		else if (rEvt.IsInCategory(EC_KEYBOARD)) {
			KeyboardEvent* pkEvt = static_cast<KeyboardEvent*>(&rEvt);
			switch (pkEvt->evType) {
			case ET_KEYBOARD_PRESS:
			{
				auto& rKey = m_Keyboard.Keys[pkEvt->keyCode];
				if (rKey.bState == BS_PRESSED) {
					rKey.bState = BS_PRESSED;
					rKey.unRepeats = 1;
				}
				else {
					rKey.bState = BS_HELD;
					rKey.unRepeats++;
				}
				switch (pkEvt->keyCode) {
				case KC_ESCAPE:
				default: break;
				}
				break;
			}
			case ET_KEYBOARD_RELEASE:
			{
				auto& rKey = m_Keyboard.Keys[pkEvt->keyCode];
				rKey.bState = BS_RELEASED;
				rKey.unRepeats = 0;
				switch (pkEvt->keyCode) {
				case KC_ESCAPE:
					StopRunning();
					rEvt.bIsHandled = true;
					break;
				case KC_M:
					m_Cursor.crsMode = m_Cursor.crsMode == CRS_DEFAULT ? CRS_CAPTURED : CRS_DEFAULT;
					m_pWindow->SetCursorMode(m_Cursor.crsMode);
					break;
				default: break;
				}
				break;
			}
			case ET_KEYBOARD_CHAR:
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
			case ET_WINDOW_DEFOCUS:
				break;
			case ET_WINDOW_CLOSE:
				StopRunning();
				rEvt.bIsHandled = true;
				break;
			}
			for (auto& itState : m_States) { if (rEvt.bIsHandled) { return; } itState->OnEvent(*pwEvt); }
		}
	}

	String CoreEngine::FDialogLoad(const char* strFilter) { return DataSys::FDialogLoad(strFilter, m_pWindow->GetNative()); }
	String CoreEngine::FDialogSave(const char* strFilter) { return DataSys::FDialogSave(strFilter, m_pWindow->GetNative()); }
	// --==</core_methods>==--
}