#include <nw_pch.hpp>
#include <core/nw_engine.h>

#if (defined NW_WAPI)
	#if (NW_WAPI & NW_GLFW)
		#include <glfw/glfw3.h>
		#include <glfw/glfw3native.h>
	#endif
#endif	// NW_WAPI

namespace NW
{
	CoreEngine::CoreEngine() :
		AEngine(),
		m_strName("nw_engine"),
		m_pWindow(RefKeeper<CoreWindow>()) { }
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
		MemSys::OnInit(1 << 23);
		CmpSys::OnInit();
		EntSys::OnInit();
		m_Memory = MemArena(MemSys::GetMemory().Alloc(1 << 18), 1 << 18);
	
		m_pWindow.MakeRef<CoreWindow>(WindowInfo{ &m_strName[0], 1200, 800, true, nullptr });
		if (!m_pWindow->Init()) { m_pWindow->OnQuit(); return false; }
		m_pWindow->SetEventCallback([this](AEvent& rEvt)->void { return this->OnEvent(rEvt); });
		
		m_pGfx.MakeRef<GfxApi>();
		if (!m_pGfx->Init()) { return false; }

		for (auto& itState : m_States) { if (!itState->Init()) { NWL_ERR("State is not initialized"); Quit(); return false; } }

		return (m_bIsRunning = true);
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
		
		MemSys::GetMemory().Dealloc(m_Memory.GetDataBeg(), m_Memory.GetDataSize());
		m_Memory = MemArena(nullptr, 0);
		EntSys::OnQuit();
		CmpSys::OnQuit();
		DataSys::GetStorage().clear();
		MemSys::OnQuit();
	}

	void CoreEngine::Update()
	{
		for (auto& itState : m_States) { itState->Update(); }
	
		IOSys::Update();
		TimeSys::Update();
		
		m_pGfx->Update();
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
	}

#if (NW_WAPI & NW_WAPI_GLFW)
	String CoreEngine::FDialogLoad(const char* strFilter) { return DataSys::FDialogLoad(strFilter, ); }
	String CoreEngine::FDialogSave(const char* strFilter) { return DataSys::FDialogSave(strFilter, ); }
#endif
#if (NW_WAPI & NW_WAPI_WIN)
	String CoreEngine::FDialogLoad(const char* strFilter) { return DataSys::FDialogLoad(strFilter, reinterpret_cast<HWND>(m_pWindow->GetNative())); }
	String CoreEngine::FDialogSave(const char* strFilter) { return DataSys::FDialogSave(strFilter, reinterpret_cast<HWND>(m_pWindow->GetNative())); }
#endif
	// --==</core_methods>==--
}