#include <nw_pch.hpp>
#include <core/nw_engine.h>

namespace NW
{
	CoreEngine::CoreEngine() :
		AEngine(),
		m_strName("nw_engine"),
		m_pWindow(RefKeeper<CoreWindow>()), m_pGfx(RefKeeper<GfxEngine>()) { }
	CoreEngine::~CoreEngine() { }

	// --==<core_methods>==--
	void CoreEngine::Run()
	{
		m_bIsRunning = true;
		auto fnRunning = [this]()->void {
			try {
				if (!Init()) { return; }
				while (m_bIsRunning) { Update(); }
				Quit();
			}
			catch (AException& exc) {
				Quit();
				NWL_ERR(exc.GetStr());
				NWL_ERR(exc.GetStrType());
				throw CodeException("running loop has been crashed", NWL_ERR_NO_INIT, __FILE__, __LINE__);
			}
		};
		m_thrRun = Thread(fnRunning);
		//fnRunning();
	}
	bool CoreEngine::Init()
	{
		try {
			if (m_pGfx.IsValid() || m_pWindow.IsValid()) { return false; }
			if (!m_bIsRunning) { m_bIsRunning = true; }

			MemSys::OnInit(1 << 23);
			DataSys::OnInit();
			CmpSys::OnInit();
			EntSys::OnInit();

			m_pWindow.MakeRef<CoreWindow>(WindowInfo{ &m_strName[0], 1200, 800, true });
			m_pWindow->SetEventCallback([this](AEvent& rEvt)->void { return this->OnEvent(rEvt); });
			GfxEngine::Create(m_pGfx, m_pWindow->GetNative());

			for (auto& itState : m_States) {
				if (!itState->Init()) {
					throw CodeException("state is not initialized", NWL_ERR_NO_INIT, __FILE__, __LINE__);
				}
			}
		}
		catch (AException& exc) {
			Quit();
			NWL_ERR(exc.GetStr());
			throw CodeException("initialization has been failed", NWL_ERR_NO_INIT, __FILE__, __LINE__);
			return false;
		}
		return true;
	}
	void CoreEngine::Quit()
	{
		if (!m_pWindow.IsValid() || !m_pGfx.IsValid()) { return; }
		if (m_bIsRunning) { m_bIsRunning = false; }

		for (auto& itState : m_States) { itState->Quit(); }
		m_States.clear();

		m_pGfx.Reset();
		m_pWindow.Reset();
		
		EntSys::OnQuit();
		CmpSys::OnQuit();
		DataSys::OnQuit();
		MemSys::OnQuit();
	}

	void CoreEngine::Update()
	{
		for (auto& itState : m_States) { itState->Update(); }
	
		TimeSys::Update();
		
		m_pWindow->Update();
		m_pGfx->Update();
	}
	void CoreEngine::OnEvent(AEvent& rEvt)
	{
		if (rEvt.IsInCategory(EC_CURSOR)) {
			CursorEvent* pmEvt = static_cast<CursorEvent*>(&rEvt);
			switch (pmEvt->evType) {
			case ET_CURSOR_MOVE:
				m_crs.xMoveDelta = pmEvt->nX - m_crs.xMove;
				m_crs.yMoveDelta = pmEvt->nY - m_crs.yMove;
				m_crs.xMove = pmEvt->nX;
				m_crs.yMove = pmEvt->nY;
				if (m_crs.GetHeld(pmEvt->cButton)) {
					m_crs.Buttons[pmEvt->cButton].xHeldDelta = m_crs.xMove - m_crs.Buttons[pmEvt->cButton].xHeld;
					m_crs.Buttons[pmEvt->cButton].yHeldDelta = m_crs.yMove - m_crs.Buttons[pmEvt->cButton].yHeld;
				}
				break;
			case ET_CURSOR_SCROLL:
				m_crs.xScrollDelta = pmEvt->nX;
				m_crs.yScrollDelta = pmEvt->nY;
				break;
			case ET_CURSOR_PRESS:
			{
				auto& rBtn = m_crs.Buttons[pmEvt->cButton];
				rBtn.bState = rBtn.bState == BS_PRESSED ? BS_HELD : BS_PRESSED;
				rBtn.xHeld = m_crs.xMove;
				rBtn.yHeld = m_crs.yMove;
				break;
			}
			case ET_CURSOR_RELEASE:
			{
				auto& rBtn = m_crs.Buttons[pmEvt->cButton];
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
				auto& rKey = m_kbd.Keys[pkEvt->keyCode];
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
				auto& rKey = m_kbd.Keys[pkEvt->keyCode];
				rKey.bState = BS_RELEASED;
				rKey.unRepeats = 0;
				switch (pkEvt->keyCode) {
				case KC_ESCAPE:
					StopRunning();
					rEvt.bIsHandled = true;
					break;
				case KC_M:
					m_crs.crsMode = m_crs.crsMode == CRS_DEFAULT ? CRS_CAPTURED : CRS_DEFAULT;
					m_pWindow->SetCursorMode(m_crs.crsMode);
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