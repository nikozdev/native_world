#include <nw_pch.hpp>
#include "nw_window.h"

#include <core/nw_engine.h>
#include <resource.h>

#if (defined NW_PLATFORM_WINDOWS)
namespace NW
{
	CoreWindow::CoreWindow(const WindowInfo& rwInfo) :
		m_wInfo(rwInfo), m_wNative{ 0 },
		m_wndClass{ 0 }, m_wMsg{ 0 },
		m_pntStruct{ 0 }
	{
		// Register a window class to create a window. ModuleHandle is the current app
		auto strClsName = L"nw_core_window";
		m_wndClass.lpszClassName = strClsName;
		m_wndClass.lpszMenuName = NULL;
		m_wndClass.lpfnWndProc = MsgProcInit;
		m_wndClass.style = CS_OWNDC;
		m_wndClass.hInstance = GetModuleHandle(NULL);
		m_wndClass.hIcon = LoadIcon(m_wndClass.hInstance, MAKEINTRESOURCE(NW_ICON0));
		m_wndClass.hIconSm = LoadIcon(m_wndClass.hInstance, MAKEINTRESOURCE(NW_ICON0));
		m_wndClass.hCursor = LoadCursor(m_wndClass.hInstance, MAKEINTRESOURCE(NW_CURSOR0));
		m_wndClass.hbrBackground = NULL;
		m_wndClass.cbClsExtra = 0;
		m_wndClass.cbWndExtra = 0;
		m_wndClass.cbSize = sizeof(WNDCLASSEX);
		if (!RegisterClassEx(&m_wndClass)) { NWL_ERR("Window class is not registered"); CoreEngine::Get().StopRunning(); return; }

		RECT wndRect = { 100, 100, 100 + GetSizeW(), 100 + GetSizeH() };
		AdjustWindowRect(&wndRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		auto strWndName = reinterpret_cast<const wchar_t*>(GetTitle());
		m_wNative.pHandle = CreateWindowEx(0, strClsName, strWndName,
			WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
			CW_USEDEFAULT, CW_USEDEFAULT, GetSizeW(), GetSizeH(),
			NULL, NULL, m_wndClass.hInstance, this);
		if (m_wNative == nullptr) { NWL_ERR("Window is not created"); CoreEngine::Get().StopRunning(); return; }
		ShowWindow(m_wNative, SW_SHOWDEFAULT);

		SetTitle(GetTitle());
		std::cout << m_wInfo;
	}
	CoreWindow::~CoreWindow()
	{
		// Get rid of the windows window and it's class
		DestroyWindow(m_wNative);
		m_wNative = { 0 };
		UnregisterClass(m_wndClass.lpszClassName, m_wndClass.hInstance);
		m_wndClass = { 0 };
	}

	// --setters
	void CoreWindow::SetTitle(const char* strTitle) {
		m_wInfo.strTitle = strTitle;
		SetWindowTextA(m_wNative, strTitle);
	}
	void CoreWindow::SetVSync(bool enabled) {
		m_wInfo.bVSync = enabled;
	}
	void CoreWindow::SetEventCallback(const EventCallback& fnOnEvent) { m_wInfo.fnOnEvent = fnOnEvent; }
	void CoreWindow::SetIcon(const ImageInfo& rInfo) { }
	void CoreWindow::SetOpacity(float nOpacity) {
		nOpacity = nOpacity > 1.0f ? 1.0f : nOpacity < 0.1f ? 0.1f : nOpacity;
		m_wInfo.nOpacity = nOpacity;
	}
	void CoreWindow::SetFocused(bool bFocus) {
		if (m_wInfo.bIsFocused == bFocus) { return; }
		SetFocus(m_wNative);
	}
	void CoreWindow::SetEnabled(bool bEnable) {
		if (m_wInfo.bIsEnabled == bEnable) { return; }
		m_wInfo.bIsEnabled = bEnable;
		EnableWindow(m_wNative, IsEnabled());
	}
	void CoreWindow::SetKeyboardMode(KeyboardModes kbdMode) {
		switch (kbdMode) {
		case KBD_LOCK: break;
		case KBD_STICK: break;
		default: break;
		}
	}
	void CoreWindow::SetCursorMode(CursorModes crsMode) {
		switch (crsMode) {
		case CRS_CAPTURED:
			// SetCapture(m_pNative);
			break;
		case CRS_HIDDEN: break;
		default: break;
		}
	}

	// --==<core_methods>==--
	void CoreWindow::Update()
	{
		// if there is false - we don't have a message
		if (PeekMessage(&m_wMsg, m_wNative, NULL, NULL, PM_NOREMOVE)) {
			// if there is false - we've got a quit
			if (GetMessage(&m_wMsg, m_wNative, NULL, NULL)) {
				TranslateMessage(&m_wMsg);
				DispatchMessage(&m_wMsg);
			}
			else { CoreEngine::Get().StopRunning(); return; }
		}
		
		Char strTitle[128];
		sprintf(strTitle, "%s|ups:%3.2f|", GetTitle(), 1.0f / TimeSys::GetDeltaS());
		SetWindowTextA(m_wNative, strTitle);
	}
	// --==</core_methods>==--

	// --==<callback_methods>==--
	inline LRESULT __stdcall CoreWindow::MsgProcInit(HWND hWnd, UINT unMsg, WPARAM wParam, LPARAM lParam) {
		if (unMsg != WM_NCCREATE) { return DefWindowProc(hWnd, unMsg, wParam, lParam); }
		
		CREATESTRUCT* pcrtStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		CoreWindow* pWindow = reinterpret_cast<CoreWindow*>(pcrtStruct->lpCreateParams);
		
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(StaticMsgProc));
		
		return StaticMsgProc(hWnd, unMsg, wParam, lParam);
	}
	inline LRESULT __stdcall CoreWindow::StaticMsgProc(HWND hWnd, UINT unMsg, WPARAM wParam, LPARAM lParam) {
		CoreWindow* pWindow = reinterpret_cast<CoreWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		
		return pWindow->MsgProc(hWnd, unMsg, wParam, lParam);
	}
	inline LRESULT __stdcall CoreWindow::MsgProc(HWND hWnd, UINT unMsg, WPARAM wParam, LPARAM lParam) {
		
		switch (unMsg) {
		case WM_CREATE:
			break;
		case WM_DESTROY:
			break;
		case WM_QUIT:
			break;
		case WM_PAINT:
		{
			return 0L;
			break;
		}
		case WM_ERASEBKGND:
			return 0L;
			break;

		case WM_MOUSEMOVE:
		{
			const POINTS xyCrd = MAKEPOINTS(lParam);
			if (IsCollidPointRect({ xyCrd.x, xyCrd.y }, { 0, 0 }, { GetSizeW(), GetSizeH() })) {
				//if (!IsHovered()) { SetCapture(m_pNative); m_wInfo.bIsHovered = true; }
				//else { m_wInfo.bIsHovered = false; }
			}
			else {
				if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON | MK_XBUTTON1 | MK_XBUTTON2)) {
					m_wInfo.fnOnEvent(CursorEvent(ET_CURSOR_MOVE, xyCrd.x, xyCrd.y));
				}
			}
			break;
		}
		case WM_MOUSEHWHEEL:
		{
			m_wInfo.fnOnEvent(CursorEvent(ET_CURSOR_SCROLL, GET_WHEEL_DELTA_WPARAM(wParam) / 500.0f, 0.0));
			break;
		}
		case WM_MOUSEWHEEL:
		{
			m_wInfo.fnOnEvent(CursorEvent(ET_CURSOR_SCROLL, 0.0, GET_WHEEL_DELTA_WPARAM(wParam) / 500.0f));
			break;
		}
		case WM_XBUTTONDOWN: case WM_RBUTTONDOWN: case WM_LBUTTONDOWN: case WM_MBUTTONDOWN:
		{
			m_wInfo.fnOnEvent(CursorEvent(ET_CURSOR_PRESS, static_cast<CursorCodes>(wParam)));
			break;
		}
		case WM_XBUTTONUP: case WM_RBUTTONUP: case WM_LBUTTONUP: case WM_MBUTTONUP:
		{
			m_wInfo.fnOnEvent(CursorEvent(ET_CURSOR_RELEASE, static_cast<CursorCodes>(wParam)));
			break;
		}
		case WM_LBUTTONDBLCLK:
			break;
		case WM_MBUTTONDBLCLK:
			break;

		case WM_SYSKEYDOWN: case WM_KEYDOWN:
			m_wInfo.fnOnEvent(KeyboardEvent(ET_KEYBOARD_PRESS, static_cast<KeyCodes>(wParam)));
			break;
		case WM_SYSKEYUP: case WM_KEYUP:
			m_wInfo.fnOnEvent(KeyboardEvent(ET_KEYBOARD_RELEASE, static_cast<KeyCodes>(wParam)));
			break;
		case WM_CHAR:
			m_wInfo.fnOnEvent(KeyboardEvent(ET_KEYBOARD_CHAR, static_cast<KeyCodes>(wParam)));
			break;

		case WM_SIZE:
			m_wInfo.nW = LOWORD(lParam);
			m_wInfo.nH = HIWORD(lParam);
			m_wInfo.fnOnEvent(WindowEvent(ET_WINDOW_RESIZE, m_wInfo.nW, m_wInfo.nH));
			PostMessage(m_wNative, WM_PAINT, 0, 0);
			break;
		case WM_MOVE:
			m_wInfo.nX = LOWORD(lParam);
			m_wInfo.nY = HIWORD(lParam);
			m_wInfo.fnOnEvent(WindowEvent(ET_WINDOW_MOVE, m_wInfo.nX, m_wInfo.nY));
			PostMessage(m_wNative, WM_PAINT, 0, 0);
			break;
		case WM_SETFOCUS:	// wParam is the last window was focused, lParam is not used
			m_wInfo.fnOnEvent(WindowEvent(ET_WINDOW_FOCUS));
			m_wInfo.bIsFocused = true;
			return 0L;
			break;
		case WM_KILLFOCUS:	// wParam is the next window will be focused, lParam is not used
			m_wInfo.bIsFocused = false;
			break;
		case WM_CLOSE:
			m_wInfo.fnOnEvent(WindowEvent(ET_WINDOW_CLOSE));
			PostQuitMessage(0);
			break;
		default: DefWindowProc(hWnd, unMsg, wParam, lParam); break;
		}
		return DefWindowProc(hWnd, unMsg, wParam, lParam);
	}
	// --==</callback_methods>==--
}
#endif // NW_PLATFORM