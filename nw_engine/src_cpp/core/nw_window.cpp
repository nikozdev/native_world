#include <nw_pch.hpp>
#include "nw_window.h"

#include <sys/nw_log_sys.h>

#if (defined NW_WINDOW)
#include <core/nw_core_engine.h>
namespace NW
{
	AWindow::AWindow(const WindowInfo& rwInfo) : m_Info(rwInfo) { }
	// --==<core_methods>==--
	AWindow* AWindow::Create(const WindowInfo& rWindowInfo)
	{
		AWindow* pWindow = nullptr;
		switch (CoreEngine::Get().GetWApiType()) {
	#if (NW_WINDOW & NW_WINDOW_GLFW)
		case WApiTypes::WAPI_GLFW: pWindow = NewT<WindowOgl>(CoreEngine::Get().GetMemory(), rWindowInfo); break;
	#endif	// NW_WINDOW
		default: NWL_ERR("Window type is undefined"); break;
		}
		return pWindow;
	}
	// --==</core_methods>==--
}

#endif // NW_WINDOW
#if (NW_WINDOW & NW_WINDOW_GLFW)
#include <glfw/glfw3.h>
namespace NW
{
	static bool s_GLFWinit = false;

	WindowOgl::WindowOgl(const WindowInfo& rwInfo) :
		AWindow(rwInfo),
		m_pNative(nullptr)
	{
		m_Info.strTitle = rwInfo.strTitle;
		m_Info.unWidth = rwInfo.unWidth;
		m_Info.unHeight = rwInfo.unHeight;
		m_Info.nOpacity = rwInfo.nOpacity;
		m_Info.WApiType = WAPI_GLFW;
	}
	WindowOgl::~WindowOgl() { }

	// --setters
	void WindowOgl::SetTitle(const char* strTitle) {
		m_Info.strTitle = strTitle;
		glfwSetWindowTitle(m_pNative, strTitle);
	}
	void WindowOgl::SetVSync(bool enabled) {
		if (enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);
		m_Info.bVSync = enabled;
	}
	void WindowOgl::SetEventCallback(const EventCallback& fnEvCallback) { m_Info.fnEvCallback = fnEvCallback; }
	void WindowOgl::SetIcon(UByte* pData, UInt16 unWidth, UInt16 unHeight) {
		m_pIcon->pixels = pData;
		m_pIcon->width = static_cast<Int32>(unWidth);
		m_pIcon->height = static_cast<Int32>(unHeight);
		glfwSetWindowIcon(m_pNative, 1, m_pIcon);
	}
	void WindowOgl::SetOpacity(float nOpacity) {
		nOpacity = nOpacity > 1.0f ? 1.0f : nOpacity < 0.1f ? 0.1f : nOpacity;
		m_Info.nOpacity = nOpacity;
		glfwSetWindowOpacity(m_pNative, nOpacity);
	}

	// --==<core_methods>==--
	bool WindowOgl::Init()
	{
		if (!s_GLFWinit) { s_GLFWinit = (glfwInit() == GLFW_TRUE); }
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Set window pointer
		m_pNative = glfwCreateWindow(static_cast<Int32>(m_Info.unWidth), static_cast<Int32>(m_Info.unHeight),
			&m_Info.strTitle[0], nullptr, nullptr);
		m_pIcon = CoreEngine::Get().NewT<GLFWimage>();
		
		UByte WhiteIcon[] = { 255, 255, 255, 255 };
		SetIcon(&WhiteIcon[0], 1, 1);
		SetVSync(m_Info.bVSync);

		glfwMakeContextCurrent(m_pNative);

		glfwSetWindowUserPointer(m_pNative, &m_Info);
		glfwSetCursorPosCallback(m_pNative, CbMouseCoord);
		glfwSetScrollCallback(m_pNative, CbMouseScroll);
		glfwSetMouseButtonCallback(m_pNative, CbMouseButton);
		glfwSetKeyCallback(m_pNative, CbKeyboard);
		glfwSetCharCallback(m_pNative, CbKeyboardChar);
		glfwSetWindowCloseCallback(m_pNative, CbWindowClose);
		glfwSetFramebufferSizeCallback(m_pNative, CbWindowSize);
		glfwSetWindowFocusCallback(m_pNative, CbWindowFocus);
		glfwSetErrorCallback(CbError);

		m_Info.strApiVer = glfwGetVersionString();
		std::cout << m_Info;

		return true;
	}
	void WindowOgl::OnQuit()
	{
		glfwSetWindowShouldClose(m_pNative, GL_TRUE);
		glfwDestroyWindow(m_pNative);
		CoreEngine::Get().DelT<GLFWimage>(m_pIcon);
	}

	void WindowOgl::Update()
	{
		glfwSwapBuffers(m_pNative);
		glfwPollEvents();
	}
	// --==</core_methods>==--

	// --==<callback_methods>==--
    // --input_callbacks
    void WindowOgl::CbMouseCoord(GLFWwindow* pWindow, double xCrd, double yCrd)
    {
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        MouseEvent mEvt = MouseEvent(ET_MOUSE_MOVE, xCrd, yCrd);
        rWindowInfo.fnEvCallback(mEvt);
    }
    void WindowOgl::CbMouseScroll(GLFWwindow* pWindow, double xScrollDelta, double yScrollDelta)
    {
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        MouseEvent mEvt = MouseEvent(ET_MOUSE_SCROLL, xScrollDelta, yScrollDelta);
        rWindowInfo.fnEvCallback(mEvt);
    }
    void WindowOgl::CbMouseButton(GLFWwindow* pWindow, Int32 nButton, Int32 nAction, Int32 nMode)
    {   // If the mouse event is gotten - set true/false in the keylist
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        if (nAction == GLFW_PRESS) { rWindowInfo.fnEvCallback(MouseEvent(ET_MOUSE_PRESS, nButton)); }
        else if (nAction == GLFW_RELEASE) { rWindowInfo.fnEvCallback(MouseEvent(ET_MOUSE_RELEASE, nButton)); }
    }
    void WindowOgl::CbKeyboard(GLFWwindow* pWindow, Int32 nKeyCode, Int32 nScanCode, Int32 nAction, Int32 nMode)
    {   // If the key event is gotten - set true/false in the keylist
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        if (nAction == GLFW_PRESS) { rWindowInfo.fnEvCallback(KeyboardEvent(ET_KEY_PRESS, nKeyCode)); }
        else if (nAction == GLFW_RELEASE) { rWindowInfo.fnEvCallback(KeyboardEvent(ET_KEY_RELEASE, nKeyCode));}
		if (nKeyCode == GLFW_KEY_F11) { glfwMaximizeWindow(glfwGetCurrentContext()); }
    }
    void WindowOgl::CbKeyboardChar(GLFWwindow* pWindow, UInt32 unChar)
    {
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        KeyboardEvent kEvt(ET_KEY_CHAR, unChar);

        rWindowInfo.fnEvCallback(kEvt);
    }
    // --window_callbacks
    void WindowOgl::CbWindowClose(GLFWwindow* pWindow)
    {
        WindowEvent wEvt = WindowEvent(ET_WINDOW_CLOSE);
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));

        glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
        rWindowInfo.fnEvCallback(wEvt);
    }
    void WindowOgl::CbWindowSize(GLFWwindow* pWindow, Int32 nWidth, Int32 nHeight)
    {
        WindowEvent wEvt = WindowEvent(ET_WINDOW_RESIZE, nWidth, nHeight);
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        rWindowInfo.unWidth = nWidth; rWindowInfo.unHeight = nHeight;

        glfwSetWindowSize(pWindow, nWidth, nHeight);
        rWindowInfo.fnEvCallback(wEvt);
    }
    void WindowOgl::CbWindowFocus(GLFWwindow* pWindow, Int32 nFocus)
    {
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        WindowEvent wEvt(ET_WINDOW_FOCUS, nFocus);

        rWindowInfo.fnEvCallback(wEvt);
    }
    // Other callbacks
    void WindowOgl::CbError(Int32 errId, const char* errMsg) { LogSys::WriteErrStr(errId, "{str}", errMsg); }
	// --==</callback_methods>==--
}
#endif // NW_WINDOW