#include <nw_pch.hpp>
#include "nw_window.h"

#include <core/nw_engine.h>
#include <resource.h>
#include <gui/impl/imgui_win.h>

namespace NW
{
	WindowInfo::WindowInfo(const char* cstTitle, UInt16 unWidth, UInt16 unHeight, Bit bVSync) :
		strTitle(cstTitle),
		nX(0), nY(0), nW(unWidth), nH(unHeight),
		nAspectRatio((Float32)unWidth / (Float32)unHeight) { }
	// --operators
	OutStream& WindowInfo::operator<<(OutStream& rStream) const {
		return rStream <<
			"--==<window_info>==--" << std::endl <<
			"title: " << &strTitle[0] << std::endl <<
			"coordx/coordy: " << nX << "/" << nY << std::endl <<
			"width/height: " << nW << "/" << nH << std::endl <<
			"api_version: " << &strApiVer[0] << std::endl <<
			"--==</window_info>==--" << std::endl;
	}
	InStream& WindowInfo::operator>>(InStream& rStream) {
		return rStream >>
			strTitle >> strApiVer >>
			nX >> nY >> nW >> nH >>
			nAspectRatio >>
			nOpacity >>
			bIsHovered >> bIsFocused >> bIsEnabled;
	}
}
#if (defined NW_PLATFORM_WINDOWS)
namespace NW
{
	CoreWindow::CoreWindow(const WindowInfo& rwInfo) :
		m_Info(rwInfo), m_pNative{ 0 },
		m_wndClass{ 0 }, m_wMsg{ 0 },
		m_pntStruct{ 0 }
	{
		// Register a window class to create a window. ModuleHandle is the current app
		auto strClsName = L"nw_core_window";
		m_wndClass.lpszClassName = strClsName;
		m_wndClass.lpszMenuName = NULL;
		m_wndClass.lpfnWndProc = MsgProcInit;
		m_wndClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
		m_wndClass.hInstance = GetModuleHandle(NULL);
		m_wndClass.hIcon = LoadIcon(m_wndClass.hInstance, MAKEINTRESOURCE(NW_ICON1));
		m_wndClass.hIconSm = LoadIcon(m_wndClass.hInstance, MAKEINTRESOURCE(NW_ICON1));
		m_wndClass.hCursor = LoadCursor(m_wndClass.hInstance, MAKEINTRESOURCE(NW_CURSOR0));
		m_wndClass.hbrBackground = NULL;
		m_wndClass.cbClsExtra = 0;
		m_wndClass.cbWndExtra = 0;
		m_wndClass.cbSize = sizeof(WNDCLASSEX);
		if (!RegisterClassEx(&m_wndClass)) { throw Exception("window class is not registred"); return; }

		DWORD wStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_VISIBLE;
		RECT wndRect = { 100, 100, 100 + GetSizeW(), 100 + GetSizeH() };
		AdjustWindowRect(&wndRect, wStyle, FALSE);

		auto strWndName = reinterpret_cast<const wchar_t*>(GetTitle());
		m_pNative = CreateWindowEx(0, strClsName, strWndName,
			wStyle,
			CW_USEDEFAULT, CW_USEDEFAULT, GetSizeW(), GetSizeH(),
			NULL, NULL, m_wndClass.hInstance, this);
		if (m_pNative == nullptr) { throw Exception("window class is not registred"); return; }
		ShowWindow(m_pNative, SW_SHOWDEFAULT);
		UpdateWindow(m_pNative);

		SetTitle(GetTitle());
		std::cout << m_Info;
	}
	CoreWindow::~CoreWindow()
	{
		// Get rid of the windows window and it's class
		DestroyWindow(m_pNative);
		m_pNative = { 0 };
		UnregisterClass(m_wndClass.lpszClassName, m_wndClass.hInstance);
		m_wndClass = { 0 };
	}
	// --setters
	void CoreWindow::SetTitle(const char* strTitle) {
		m_Info.strTitle = strTitle;
		SetWindowTextA(m_pNative, strTitle);
	}
	void CoreWindow::SetEventCallback(const EventCallback& fnOnEvent) { m_Info.fnOnEvent = fnOnEvent; }
	void CoreWindow::SetIcon(const ImageInfo& rInfo) { }
	void CoreWindow::SetOpacity(float nOpacity) {
		nOpacity = nOpacity > 1.0f ? 1.0f : nOpacity < 0.1f ? 0.1f : nOpacity;
		m_Info.nOpacity = nOpacity;
		UInt64 nStyle = GetWindowLongW(m_pNative, GWL_EXSTYLE);
		if (nOpacity < 1.0f) {	// get window style and add "layered attribute" to it
			nStyle |= WS_EX_LAYERED;
			SetWindowLongW(m_pNative, GWL_EXSTYLE, nStyle);
			SetLayeredWindowAttributes(m_pNative, 0u, static_cast<UByte>(255 * nOpacity), LWA_ALPHA);
		}
		else {  // get rid of transparency
			nStyle &= ~WS_EX_LAYERED;	// "&~0b0010" == get all bits except this one
			SetWindowLongW(m_pNative, GWL_EXSTYLE, nStyle);
		}
	}
	void CoreWindow::SetFocused(bool bFocus) {
		if (m_Info.bIsFocused == bFocus) { return; }
		SetFocus(m_pNative);
	}
	void CoreWindow::SetEnabled(bool bEnable) {
		if (m_Info.bIsEnabled == bEnable) { return; }
		m_Info.bIsEnabled = bEnable;
		EnableWindow(m_pNative, IsEnabled());
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
		case CRS_CAPTURED: {
			SetCapture(m_pNative);
			RECT wRect = { GetCoordX(), GetCoordY(), GetCoordX() + GetSizeW(), GetCoordY() + GetSizeH() };
			ClipCursor(&wRect);
			ShowCursor(FALSE);
			break;
		}
		case CRS_HIDDEN: {
			ShowCursor(FALSE);
			break;
		}
		default: {
			ShowCursor(TRUE);
			ClipCursor(NULL);
			ReleaseCapture();
			break;
		}
		}
	}
	// --==<core_methods>==--
	void CoreWindow::Update()
	{
		// if the message queue is empty - thread can be blocked by GetMessage()
		// if there is 0 - we've got a quit; can be also -1
		if (m_wMsg.message != WM_QUIT) {
			// if there is false - we don't have a message
			if (PeekMessage(&m_wMsg, m_pNative, 0u, 0u, PM_REMOVE)) {
				TranslateMessage(&m_wMsg);	// make WM_CHAR/WM_SYSCHAR messages
				DispatchMessage(&m_wMsg);	// send all the messages into window procedure
			}
		}
		Char strTitle[128];
		sprintf(strTitle, "%s|fps:%3.2d|", GetTitle(), static_cast<Int32>(TimeSys::GetFPS()));
		SetWindowTextA(m_pNative, strTitle);
	}
	// --==</core_methods>==--
	
	// --==<callback_methods>==--
	LRESULT __stdcall CoreWindow::MsgProcInit(HWND hWnd, UINT unMsg, WPARAM wParam, LPARAM lParam) {
		if (unMsg != WM_NCCREATE) { return DefWindowProc(hWnd, unMsg, wParam, lParam); }
		
		CREATESTRUCT* pcrtStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		CoreWindow* pWindow = reinterpret_cast<CoreWindow*>(pcrtStruct->lpCreateParams);
		
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(StaticMsgProc));
		
		return StaticMsgProc(hWnd, unMsg, wParam, lParam);
	}
	LRESULT __stdcall CoreWindow::StaticMsgProc(HWND hWnd, UINT unMsg, WPARAM wParam, LPARAM lParam) {
		CoreWindow* pWindow = reinterpret_cast<CoreWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		
		return pWindow->MsgProc(hWnd, unMsg, wParam, lParam);
	}
	LRESULT __stdcall CoreWindow::MsgProc(HWND hWnd, UINT unMsg, WPARAM wParam, LPARAM lParam) {
		if (GUI::Win32MsgProc(hWnd, unMsg, wParam, lParam)) { return true; }

		switch (unMsg) {
		case WM_CREATE: {
			return 0l;
		}
		case WM_DESTROY: {
			return 0l;
		}
		case WM_QUIT: {
			return 0l;
		}
		case WM_PAINT: {
			return 0l;
		}
		case WM_ERASEBKGND: {
			return 0l;
		}

		case WM_MOUSEMOVE: {
			const POINTS xyCrd = MAKEPOINTS(lParam);
			m_Info.fnOnEvent(CursorEvent(EVT_CURSOR_MOVE, xyCrd.x, xyCrd.y));
			return 0l;
		}
		case WM_MOUSEHWHEEL: {
			m_Info.fnOnEvent(CursorEvent(EVT_CURSOR_SCROLL, GET_WHEEL_DELTA_WPARAM(wParam) / 100.0f, 0.0));
			return 0l;
		}
		case WM_MOUSEWHEEL: {
			m_Info.fnOnEvent(CursorEvent(EVT_CURSOR_SCROLL, 0.0, GET_WHEEL_DELTA_WPARAM(wParam) / 100.0f));
			return 0l;
		}
		case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
		case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
		{
			UInt8 nButton = 0u;
			if (unMsg == WM_LBUTTONDOWN || unMsg == WM_LBUTTONDBLCLK) { nButton = 0u; }
			if (unMsg == WM_RBUTTONDOWN || unMsg == WM_RBUTTONDBLCLK) { nButton = 1u; }
			if (unMsg == WM_MBUTTONDOWN || unMsg == WM_MBUTTONDBLCLK) { nButton = 2u; }
			if (unMsg == WM_XBUTTONDOWN || unMsg == WM_XBUTTONDBLCLK) { nButton = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3u : 4u; }
			if (GetCapture() == nullptr) { SetCapture(m_pNative); }
			m_Info.fnOnEvent(CursorEvent(EVT_CURSOR_PRESS, static_cast<CursorCodes>(nButton)));
			return 0l;
		}
		case WM_LBUTTONUP: case WM_RBUTTONUP: case WM_MBUTTONUP: case WM_XBUTTONUP: {
			UInt8 nButton = 0u;
			if (unMsg == WM_LBUTTONUP) { nButton = 0u; }
			if (unMsg == WM_RBUTTONUP) { nButton = 1u; }
			if (unMsg == WM_MBUTTONUP) { nButton = 2u; }
			if (unMsg == WM_XBUTTONUP) {nButton = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3u : 4u; }
			m_Info.fnOnEvent(CursorEvent(EVT_CURSOR_RELEASE, static_cast<CursorCodes>(nButton)));
			if (GetCapture() == m_pNative) { ReleaseCapture(); }
			return 0l;
		}

		case WM_KEYDOWN: case WM_SYSKEYDOWN: {
			m_Info.fnOnEvent(KeyboardEvent(EVT_KEYBOARD_PRESS, static_cast<KeyCodes>(wParam)));
			return 0l;
		}
		case WM_KEYUP: case WM_SYSKEYUP: {
			m_Info.fnOnEvent(KeyboardEvent(EVT_KEYBOARD_RELEASE, static_cast<KeyCodes>(wParam)));
			return 0l;
		}
		case WM_CHAR: {
			m_Info.fnOnEvent(KeyboardEvent(EVT_KEYBOARD_CHAR, static_cast<KeyCodes>(wParam)));
			return 0l;
		}

		case WM_SIZE: {
			m_Info.nW = LOWORD(lParam);
			m_Info.nH = HIWORD(lParam);
			//SetWindowPos(m_pNative, NULL, 0, 0, GetSizeW(), GetSizeH(), SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
			m_Info.fnOnEvent(WindowEvent(EVT_WINDOW_RESIZE, m_Info.nW, m_Info.nH));
			PostMessage(m_pNative, WM_PAINT, 0, 0);
			return 0;
		}
		case WM_MOVE: {
			m_Info.nX = LOWORD(lParam);
			m_Info.nY = HIWORD(lParam);
			//SetWindowPos(m_pNative, NULL, GetCoordX(), GetCoordY(), 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
			m_Info.fnOnEvent(WindowEvent(EVT_WINDOW_MOVE, m_Info.nX, m_Info.nY));
			PostMessage(m_pNative, WM_PAINT, 0, 0);
			return 0;
		}
		case WM_SETFOCUS: {		// wParam is the last window was focused, lParam is not used
			m_Info.fnOnEvent(WindowEvent(EVT_WINDOW_FOCUS));
			m_Info.bIsFocused = true;
			return 0;
		}
		case WM_KILLFOCUS: {	// wParam is the next window will be focused, lParam is not used
			m_Info.bIsFocused = false;
			return 0;
		}
		case WM_CLOSE: {

			m_Info.fnOnEvent(WindowEvent(EVT_WINDOW_CLOSE));
			PostQuitMessage(0);
			return 0;
		}
		default: DefWindowProc(hWnd, unMsg, wParam, lParam); break;
		}
		return DefWindowProc(hWnd, unMsg, wParam, lParam);
	}
	// --==</callback_methods>==--
}
#endif // NW_PLATFORM