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
		m_pIcon(nullptr)
	{
		m_wInfo.strTitle = rwInfo.strTitle;
		m_wInfo.unWidth = rwInfo.unWidth;
		m_wInfo.unHeight = rwInfo.unHeight;
		m_wInfo.nOpacity = rwInfo.nOpacity;
	}
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
	void CoreWindow::SetEventCallback(const EventCallback& fnOnEvent) { m_wInfo.fnOnEvent = fnOnEvent; }
	void CoreWindow::SetIcon(const ImageInfo& rInfo) {
		m_pIcon->pixels = rInfo.pClrData;
		m_pIcon->width = rInfo.nWidth;
		m_pIcon->height = rInfo.nHeight;
		glfwSetWindowIcon(m_pNative, 1, m_pIcon);
	}
	void CoreWindow::SetOpacity(float nOpacity) {
		nOpacity = nOpacity > 1.0f ? 1.0f : nOpacity < 0.1f ? 0.1f : nOpacity;
		m_wInfo.nOpacity = nOpacity;
		glfwSetWindowOpacity(m_pNative, nOpacity);
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
		glfwSwapBuffers(m_pNative);
		glfwPollEvents();
	}
	// --==</core_methods>==--

	// --==<callback_methods>==--
    // --input_callbacks
    void CoreWindow::CbMouseCoord(GLFWwindow* pWindow, double xCrd, double yCrd)
    {
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        MouseEvent mEvt = MouseEvent(ET_MOUSE_MOVE, xCrd, yCrd);
        rWindowInfo.fnOnEvent(mEvt);
    }
    void CoreWindow::CbMouseScroll(GLFWwindow* pWindow, double xScrollDelta, double yScrollDelta)
    {
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        MouseEvent mEvt = MouseEvent(ET_MOUSE_SCROLL, xScrollDelta, yScrollDelta);
        rWindowInfo.fnOnEvent(mEvt);
    }
    void CoreWindow::CbMouseButton(GLFWwindow* pWindow, Int32 nButton, Int32 nAction, Int32 nMode)
    {   // If the mouse event is gotten - set true/false in the keylist
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        if (nAction == GLFW_PRESS) { rWindowInfo.fnOnEvent(MouseEvent(ET_MOUSE_PRESS, nButton)); }
        else if (nAction == GLFW_RELEASE) { rWindowInfo.fnOnEvent(MouseEvent(ET_MOUSE_RELEASE, nButton)); }
    }
    void CoreWindow::CbKeyboard(GLFWwindow* pWindow, Int32 nKeyCode, Int32 nScanCode, Int32 nAction, Int32 nMode)
    {   // If the key event is gotten - set true/false in the keylist
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        if (nAction == GLFW_PRESS) { rWindowInfo.fnOnEvent(KeyboardEvent(ET_KEY_PRESS, nKeyCode)); }
        else if (nAction == GLFW_RELEASE) { rWindowInfo.fnOnEvent(KeyboardEvent(ET_KEY_RELEASE, nKeyCode));}
		if (nKeyCode == GLFW_KEY_F11) { glfwMaximizeWindow(glfwGetCurrentContext()); }
    }
    void CoreWindow::CbKeyboardChar(GLFWwindow* pWindow, UInt32 unChar)
    {
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        KeyboardEvent kEvt(ET_KEY_CHAR, unChar);

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
		m_wInfo(rwInfo), m_pNative(nullptr),
		m_devContext(nullptr), m_oglContext(nullptr),
		m_wndClass{ 0 }, m_wMsg{ 0 },
		m_pxfDesc{ 0 },
		m_pntStruct{ 0 }
	{
		m_wInfo.strTitle = rwInfo.strTitle;
		m_wInfo.unWidth = rwInfo.unWidth;
		m_wInfo.unHeight = rwInfo.unHeight;
		m_wInfo.nOpacity = rwInfo.nOpacity;
		m_wInfo.fnOnEvent = [](AEvent& rEvt)->void { return; };
	}
	CoreWindow::~CoreWindow() {}

	// --setters
	void CoreWindow::SetTitle(const char* strTitle) {
		m_wInfo.strTitle = strTitle;
		SetWindowTextA(m_pNative, strTitle);
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

	// --==<core_methods>==--
	bool CoreWindow::Init()
	{
		if (m_wndClass.hInstance != nullptr) { return false; }

		{
			auto strClsName = L"nw_core_window";
			m_wndClass.lpszClassName = strClsName;
			m_wndClass.lpszMenuName = NULL;
			m_wndClass.lpfnWndProc = MsgProcInit;
			m_wndClass.style = CS_OWNDC;
			m_wndClass.hInstance = GetModuleHandle(nullptr);
			m_wndClass.hIcon = LoadIcon(m_wndClass.hInstance, MAKEINTRESOURCE(NW_ICON0));
			m_wndClass.hIconSm = LoadIcon(m_wndClass.hInstance, MAKEINTRESOURCE(NW_ICON0));
			m_wndClass.hCursor = LoadCursor(m_wndClass.hInstance, MAKEINTRESOURCE(NW_CURSOR0));
			m_wndClass.hbrBackground = NULL;
			m_wndClass.cbClsExtra = 0;
			m_wndClass.cbWndExtra = 0;
			m_wndClass.cbSize = sizeof(WNDCLASSEX);
			if (!RegisterClassEx(&m_wndClass)) { return false; }

			RECT wndRect = { 100, 100, 100 + GetWidth(), 100 + GetHeight() };
			AdjustWindowRect(&wndRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
			auto strWndName = reinterpret_cast<const wchar_t*>(GetTitle());
			m_pNative = CreateWindowEx(0,
				strClsName, strWndName,
				WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
				CW_USEDEFAULT, CW_USEDEFAULT, GetWidth(), GetHeight(),
				NULL, NULL,
				m_wndClass.hInstance, this);
			if (m_pNative == nullptr) { return false; }
		}
		{
			m_devContext = GetDC(m_pNative);
			m_pxfDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			m_pxfDesc.nVersion = 1;
			m_pxfDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
			m_pxfDesc.iPixelType = PFD_TYPE_RGBA;
			m_pxfDesc.cColorBits = 32;
			Int32 nPxFormat = ChoosePixelFormat(m_devContext, &m_pxfDesc);
			if (nPxFormat == 0) { return false; }
			if (!SetPixelFormat(m_devContext, nPxFormat, &m_pxfDesc)) { return false; }
			DescribePixelFormat(m_devContext, nPxFormat, m_pxfDesc.nSize, &m_pxfDesc);
			
			m_oglContext = wglCreateContext(m_devContext);
			wglMakeCurrent(m_devContext, m_oglContext);
			ShowWindow(m_pNative, SW_SHOWDEFAULT);
		}

		SetTitle(GetTitle());
		std::cout << m_wInfo;

		return true;
	}
	void CoreWindow::OnQuit()
	{
		if (m_wndClass.hInstance == nullptr) { return; }
		
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_oglContext);
		ReleaseDC(m_pNative, m_devContext);
		
		UnregisterClass(m_wndClass.lpszClassName, m_wndClass.hInstance);
		DestroyWindow(m_pNative);
	}

	void CoreWindow::Update()
	{
		// if there is false - we don't have a message
		if (PeekMessage(&m_wMsg, m_pNative, NULL, NULL, PM_NOREMOVE)) {
			// if there is false - we've got a quit
			if (GetMessage(&m_wMsg, m_pNative, NULL, NULL)) {
				TranslateMessage(&m_wMsg);
				DispatchMessage(&m_wMsg);
			}
			else { CoreEngine::Get().StopRunning(); return; }
		}
		SwapBuffers(m_devContext);
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
			BeginPaint(m_pNative, &m_pntStruct);
			EndPaint(m_pNative, &m_pntStruct);
			return 0;
			break;
		}

		case WM_MOUSEMOVE:
		{
			const POINTS xyCrd = MAKEPOINTS(lParam);
			m_wInfo.fnOnEvent(MouseEvent(ET_MOUSE_MOVE, xyCrd.x, xyCrd.y));
			break;
		}
		case WM_LBUTTONDOWN:
		{
			m_wInfo.fnOnEvent(MouseEvent(ET_MOUSE_PRESS, wParam));
			break;
		}

		case WM_SYSKEYDOWN: case WM_KEYDOWN:
			m_wInfo.fnOnEvent(KeyboardEvent(ET_KEY_PRESS, wParam));
			break;
		case WM_SYSKEYUP: case WM_KEYUP:
			m_wInfo.fnOnEvent(KeyboardEvent(ET_KEY_RELEASE, wParam));
			break;
		case WM_CHAR:
			m_wInfo.fnOnEvent(KeyboardEvent(ET_KEY_CHAR, wParam));
			break;

		case WM_SIZE:
			m_wInfo.fnOnEvent(WindowEvent(ET_WINDOW_RESIZE, LOWORD(lParam), HIWORD(lParam)));
			PostMessage(m_pNative, WM_PAINT, 0, 0);
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