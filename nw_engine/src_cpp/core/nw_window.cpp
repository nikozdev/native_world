#include <nw_pch.hpp>
#include "nw_window.h"

#if (defined NW_WAPI)
#include <core/nw_engine.h>
#include <resource.h>
#if (NW_WAPI & NW_WAPI_GLFW)
#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>
namespace NW
{
	CoreWindow::CoreWindow(const WindowInfo& rwInfo) :
		m_wInfo(rwInfo), m_pNative(nullptr),
		m_pIcon(nullptr) { }
	CoreWindow::~CoreWindow() {}
	
	// --setters
	void CoreWindow::SetTitle(const char* strTitle) {
		m_wInfo.strTitle = strTitle;
		glfwSetWindowTitle(m_pNative, strTitle);
	}
	void CoreWindow::SetVSync(bool enabled) {
		if (enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);
		m_wInfo.bVSync = enabled;
	}
	void CoreWindow::SetOpacity(float nOpacity) {
		nOpacity = nOpacity > 1.0f ? 1.0f : nOpacity < 0.1f ? 0.1f : nOpacity;
		m_wInfo.nOpacity = nOpacity;
		glfwSetWindowOpacity(m_pNative, nOpacity);
	}
	void CoreWindow::SetIcon(const ImageInfo& rInfo) {
		m_pIcon->pixels = rInfo.pClrData;
		m_pIcon->width = rInfo.nWidth;
		m_pIcon->height = rInfo.nHeight;
		glfwSetWindowIcon(m_pNative, 1, m_pIcon);
	}
	void CoreWindow::SetEventCallback(const EventCallback& fnOnEvent) { m_wInfo.fnOnEvent = fnOnEvent; }
	void CoreWindow::SetKeyboardMode(KeyboardModes kbdMode) {
		switch (kbdMode) {
		case KBD_LOCK: break;
		case KBD_STICK: break;
		default: break;
		}
	}
	void CoreWindow::SetCursorMode(CursorModes crsMode) {
		switch (crsMode) {
		case CRS_CAPTURED: glfwSetInputMode(); break;
		case CRS_HIDDEN: break;
		default: break;
		}
	}
	// --==<core_methods>==--
	bool CoreWindow::Init()
	{
		NWL_ASSERT(glfwInit() == GLFW_TRUE, "glfw is not initialized!");
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		// Set window pointer
		m_pNative = glfwCreateWindow(static_cast<Int32>(m_wInfo.unWidth), static_cast<Int32>(m_wInfo.unHeight),
			&m_wInfo.strTitle[0], nullptr, nullptr);
		m_pIcon = CoreEngine::Get().NewT<GLFWimage>();
		
		UByte WhiteIcon[] = { 255, 255, 255, 255 };
		SetIcon(ImageInfo{ &WhiteIcon[0], 1, 1 });
		SetVSync(m_wInfo.bVSync);

		glfwMakeContextCurrent(m_pNative);

		glfwSetWindowUserPointer(m_pNative, &m_wInfo);
		glfwSetCursorPosCallback(m_pNative, CbMouseCoord);
		glfwSetScrollCallback(m_pNative, CbMouseScroll);
		glfwSetMouseButtonCallback(m_pNative, CbMouseButton);
		glfwSetKeyCallback(m_pNative, CbKeyboard);
		glfwSetCharCallback(m_pNative, CbKeyboardChar);
		glfwSetWindowCloseCallback(m_pNative, CbWindowClose);
		glfwSetFramebufferSizeCallback(m_pNative, CbWindowSize);
		glfwSetWindowFocusCallback(m_pNative, CbWindowFocus);
		glfwSetErrorCallback(CbError);

		m_wInfo.strApiVer = glfwGetVersionString();
		std::cout << m_wInfo;

		return true;
	}
	void CoreWindow::OnQuit()
	{
		glfwSetWindowShouldClose(m_pNative, GL_TRUE);
		glfwDestroyWindow(m_pNative);
		CoreEngine::Get().DelT<GLFWimage>(m_pIcon);
		glfwTerminate();
	}

	void CoreWindow::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_pNative);
	}
	// --==</core_methods>==--

	// --==<callback_methods>==--
    // --input_callbacks
    void CoreWindow::CbMouseCoord(GLFWwindow* pWindow, double xCrd, double yCrd)
    {
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        CursorEvent mEvt = CursorEvent(ET_CURSOR_MOVE, xCrd, yCrd);
        rWindowInfo.fnOnEvent(mEvt);
    }
    void CoreWindow::CbMouseScroll(GLFWwindow* pWindow, double xScrollDelta, double yScrollDelta)
    {
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        CursorEvent mEvt = CursorEvent(ET_CURSOR_SCROLL, xScrollDelta, yScrollDelta);
        rWindowInfo.fnOnEvent(mEvt);
    }
    void CoreWindow::CbMouseButton(GLFWwindow* pWindow, Int32 nButton, Int32 nAction, Int32 nMode)
    {   // If the mouse event is gotten - set true/false in the keylist
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        if (nAction == GLFW_PRESS) { rWindowInfo.fnOnEvent(CursorEvent(ET_CURSOR_PRESS, nButton)); }
        else if (nAction == GLFW_RELEASE) { rWindowInfo.fnOnEvent(CursorEvent(ET_CURSOR_RELEASE, nButton)); }
    }
    void CoreWindow::CbKeyboard(GLFWwindow* pWindow, Int32 nKeyCode, Int32 nScanCode, Int32 nAction, Int32 nMode)
    {   // If the key event is gotten - set true/false in the keylist
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        if (nAction == GLFW_PRESS) { rWindowInfo.fnOnEvent(KeyboardEvent(ET_KEYBOARD_PRESS, nKeyCode)); }
        else if (nAction == GLFW_RELEASE) { rWindowInfo.fnOnEvent(KeyboardEvent(ET_KEYBOARD_RELEASE, nKeyCode));}
		if (nKeyCode == GLFW_KEY_F11) { glfwMaximizeWindow(glfwGetCurrentContext()); }
    }
    void CoreWindow::CbKeyboardChar(GLFWwindow* pWindow, UInt32 unChar)
    {
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        KeyboardEvent kEvt(ET_KEYBOARD_CHAR, unChar);

        rWindowInfo.fnOnEvent(kEvt);
    }
    // --window_callbacks
    void CoreWindow::CbWindowClose(GLFWwindow* pWindow)
    {
        WindowEvent wEvt = WindowEvent(ET_WINDOW_CLOSE);
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));

        glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
        rWindowInfo.fnOnEvent(wEvt);
    }
    void CoreWindow::CbWindowSize(GLFWwindow* pWindow, Int32 nWidth, Int32 nHeight)
    {
        WindowEvent wEvt = WindowEvent(ET_WINDOW_RESIZE, nWidth, nHeight);
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        rWindowInfo.unWidth = nWidth; rWindowInfo.unHeight = nHeight;

        glfwSetWindowSize(pWindow, nWidth, nHeight);
        rWindowInfo.fnOnEvent(wEvt);
    }
    void CoreWindow::CbWindowFocus(GLFWwindow* pWindow, Int32 nFocus)
    {
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        WindowEvent wEvt(ET_WINDOW_FOCUS, nFocus);

        rWindowInfo.fnOnEvent(wEvt);
    }
    // Other callbacks
    void CoreWindow::CbError(Int32 errId, const char* errMsg) { printf("GLFW_ERROR::ID_%d: %s\n", errId, errMsg); }
	// --==</callback_methods>==--
}
#endif
#if (NW_WAPI & NW_WAPI_WIN)
namespace NW
{
	CoreWindow::CoreWindow(const WindowInfo& rwInfo) :
		m_wInfo(rwInfo),
		m_Native{ 0 }, m_Context{ 0 },
		m_wndClass{ 0 }, m_wMsg{ 0 },
		m_pntStruct{ 0 } { }
	CoreWindow::~CoreWindow() { OnQuit(); }

	// --setters
	void CoreWindow::SetTitle(const char* strTitle) {
		m_wInfo.strTitle = strTitle;
		SetWindowTextA(m_Native, strTitle);
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
		SetFocus(m_Native);
	}
	void CoreWindow::SetEnabled(bool bEnable) {
		if (m_wInfo.bIsEnabled == bEnable) { return; }
		m_wInfo.bIsEnabled = bEnable;
		EnableWindow(m_Native, IsEnabled());
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
	bool CoreWindow::Init()
	{
		if (m_wndClass.hInstance != nullptr) { return false; }

		// Register a window class to create a window. ModuleHandle is the current app
		if (!InitWindow()) { return false; }
		if (!InitContext()) { return false; }

		SetTitle(GetTitle());
		std::cout << m_wInfo;

		return true;
	}
	void CoreWindow::OnQuit()
	{
		if (m_wndClass.hInstance == nullptr) { return; }
		// Get rid of the windows window and it's class
		QuitContext();
		QuitWindow();
	}

	void CoreWindow::Update()
	{
		// if there is false - we don't have a message
		if (PeekMessage(&m_wMsg, m_Native, NULL, NULL, PM_NOREMOVE)) {
			// if there is false - we've got a quit
			if (GetMessage(&m_wMsg, m_Native, NULL, NULL)) {
				TranslateMessage(&m_wMsg);
				DispatchMessage(&m_wMsg);
			}
			else { CoreEngine::Get().StopRunning(); return; }
		}
#if (NW_GAPI & NW_GAPI_OGL)
		SwapBuffers(m_Context);
#endif
		Char strTitle[128];
		sprintf(strTitle, "%s|ups:%3.2f|", GetTitle(), 1.0f / TimeSys::GetDeltaS());
		SetWindowTextA(m_Native, strTitle);
	}
	// --==</core_methods>==--

	// --==<implementation_methods>==--
	inline bool CoreWindow::InitWindow() {
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
		if (!RegisterClassEx(&m_wndClass)) { return false; }

		RECT wndRect = { 100, 100, 100 + GetSizeW(), 100 + GetSizeH() };
		AdjustWindowRect(&wndRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		auto strWndName = reinterpret_cast<const wchar_t*>(GetTitle());
		m_Native.pHandle = CreateWindowEx(0, strClsName, strWndName,
			WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
			CW_USEDEFAULT, CW_USEDEFAULT, GetSizeW(), GetSizeH(),
			NULL, NULL, m_wndClass.hInstance, this);
		if (m_Native.pHandle == nullptr) { return false; }
		ShowWindow(m_Native, SW_SHOWDEFAULT);
		return true;
	}
	inline void CoreWindow::QuitWindow() {
		DestroyWindow(m_Native);
		m_Native = { 0 };
		UnregisterClass(m_wndClass.lpszClassName, m_wndClass.hInstance);
		m_wndClass = { 0 };
	}
	inline bool CoreWindow::InitContext() {
		PIXELFORMATDESCRIPTOR pxfDesc{ 0 };
		// Get device context, set it's pixel format and only then  make render context.
		// Get default device context. Only one DC can be used in a single thread at one time.
		m_Context.pHandle = GetWindowDC(GetNative());
		pxfDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pxfDesc.nVersion = 1;
		pxfDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
		pxfDesc.iPixelType = PFD_TYPE_RGBA;
		pxfDesc.iLayerType = PFD_MAIN_PLANE;
		pxfDesc.cColorBits = 24;
		pxfDesc.cRedBits = 0; pxfDesc.cGreenBits = 0; pxfDesc.cBlueBits = 0; pxfDesc.cAlphaBits = 0;
		pxfDesc.cRedShift = 0; pxfDesc.cGreenShift = 0; pxfDesc.cBlueShift = 0; pxfDesc.cAlphaShift = 0;
		pxfDesc.cAccumBits = 0;
		pxfDesc.cAccumRedBits = 0; pxfDesc.cAccumGreenBits = 0; pxfDesc.cAccumBlueBits = 0; pxfDesc.cAccumAlphaBits = 0;
		pxfDesc.cAuxBuffers = 0;
		pxfDesc.cDepthBits = 24;
		pxfDesc.cStencilBits = 8;
		pxfDesc.bReserved = 0;
		pxfDesc.dwVisibleMask = 0; pxfDesc.dwLayerMask = 0; pxfDesc.dwDamageMask = 0;
		// Get the best availabple pixel format for device context
		Int32 nPxFormat = ChoosePixelFormat(m_Context, &pxfDesc);
		if (nPxFormat == 0) { return false; }
		// Pixel format can be set to some window only once
		if (!SetPixelFormat(m_Context, nPxFormat, &pxfDesc)) { return false; }
		DescribePixelFormat(m_Context, nPxFormat, pxfDesc.nSize, &pxfDesc);
#if (NW_GAPI & NW_GAPI_OGL)
		// Create opengl context and associate that with the device context
		// It will be attached to the current frame and DC so,
		// this is only one current context we can use
		m_Context.pGfx = wglCreateContext(m_Context.pDevice);
		wglMakeCurrent(m_Context.pDevice, m_Context);
#endif
#if NW_GAPI & NW_GAPI_DX
#endif
		return true;
	}
	inline void CoreWindow::QuitContext() {
#if (NW_GAPI & NW_GAPI_OGL)
		// Break the connection between our thread and the rendering context
		wglMakeCurrent(NULL, NULL);
		// Release the associated DC and delete the rendering context
		ReleaseDC(m_Native, m_Context);
		// Before delete - we need to release that
		// DeleteDC(m_Context);	// delete only created device context
		// Before this call device context must be released or deleted
		wglDeleteContext(m_Context);
#endif
#if (NW_GAPI & NW_GAPI_DX)
		ReleaseDC(m_Native, m_Context);
#endif
		m_Context = { 0 };
	}
	// --==</implementation_methods>==--

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
			PostMessage(m_Native, WM_PAINT, 0, 0);
			break;
		case WM_MOVE:
			m_wInfo.nX = LOWORD(lParam);
			m_wInfo.nY = HIWORD(lParam);
			m_wInfo.fnOnEvent(WindowEvent(ET_WINDOW_MOVE, m_wInfo.nX, m_wInfo.nY));
			PostMessage(m_Native, WM_PAINT, 0, 0);
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
#endif
#endif // NW_WAPI