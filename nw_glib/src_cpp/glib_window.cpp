#include <glib_pch.hpp>
#include "glib_window.h"

#include <glib_event.h>

#if (defined GLIB_WINDOW)
namespace GLIB
{
	AWindow* AWindow::Create(const WindowInfo& rWindowInfo, WApiTypes WApiType)
	{
		AWindow* pWindow = nullptr;
		switch (WApiType) {
	#if (GLIB_WINDOW & GLIB_WINDOW_GLFW)
		case WApiTypes::WAPI_GLFW: pWindow = new WindowOgl(rWindowInfo); break;
	#endif	// GLIB_WINDOW
		default: GLIB_ERR("Window type is undefined"); break;
		}
		return pWindow;
	}
}

#endif // GLIB_WINDOW
#if (GLIB_WINDOW & GLIB_WINDOW_GLFW)
#include <glfw/glfw3.h>
namespace GLIB
{
	static bool s_GLFWinit = false;

	WindowOgl::WindowOgl(const WindowInfo& rWindowInfo) :
		AWindow(),
		m_pNative(nullptr),
		m_pGContext(nullptr)
	{
		m_WindowInfo.strTitle = rWindowInfo.strTitle;
		m_WindowInfo.unWidth = rWindowInfo.unWidth;
		m_WindowInfo.unHeight = rWindowInfo.unHeight;
		m_WindowInfo.WApiType = WAPI_GLFW;
		printf("NW::WINDOW_OGL::INIT: %s::%dx%d\n",
			&m_WindowInfo.strTitle[0], m_WindowInfo.unWidth, m_WindowInfo.unHeight);
	}
	WindowOgl::~WindowOgl() { }

	// --setters
	void WindowOgl::SetTitle(const char* strTitle) {
		m_WindowInfo.strTitle = strTitle;
		glfwSetWindowTitle(m_pNative, strTitle);
	}
	void WindowOgl::SetVSync(bool enabled) {
		if (enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);
		m_WindowInfo.bVSync = enabled;
	}

	// --==<core_methods>==--
	bool WindowOgl::Init()
	{
		// init glfw and configure the window
		if (!s_GLFWinit) {
			s_GLFWinit = glfwInit() == GLFW_TRUE;
			GLIB_ASSERT(s_GLFWinit, "Could not initialize GLFW");
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Set window pointer
		m_pNative = glfwCreateWindow(static_cast<Int32>(m_WindowInfo.unWidth), static_cast<Int32>(m_WindowInfo.unHeight),
			&m_WindowInfo.strTitle[0], nullptr, nullptr);

		m_pGContext.reset(new GContextOgl(m_pNative));
		m_pGContext->OnInit();

		SetVSync(m_WindowInfo.bVSync);
		// Bind own data pointer to window. It allows to get this pointer in callback
		glfwSetWindowUserPointer(m_pNative, &m_WindowInfo);

		glfwSetCursorPosCallback(m_pNative, CbMouseCoord);
		glfwSetScrollCallback(m_pNative, CbMouseScroll);
		glfwSetMouseButtonCallback(m_pNative, CbMouseButton);
		glfwSetKeyCallback(m_pNative, CbKeyboard);
		glfwSetCharCallback(m_pNative, CbKeyboardChar);
		glfwSetWindowCloseCallback(m_pNative, CbWindowClose);
		glfwSetFramebufferSizeCallback(m_pNative, CbWindowSize);
		glfwSetWindowFocusCallback(m_pNative, CbWindowFocus);
		glfwSetErrorCallback(CbError);

		return true;
	}
	void WindowOgl::OnQuit()
	{
		glfwSetWindowShouldClose(m_pNative, GL_TRUE);
		glfwDestroyWindow(m_pNative);
		m_pGContext->OnQuit();
	}

	void WindowOgl::Update()
	{
		m_pGContext->SwapBuffers();
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
        else if (nAction == GLFW_RELEASE) { rWindowInfo.fnEvCallback(KeyboardEvent(ET_KEY_RELEASE, nKeyCode)); }
    }
    void WindowOgl::CbKeyboardChar(GLFWwindow* pWindow, UInt32 unChar)
    {
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        KeyboardEvent kEvt(ET_KEY_TYPE, unChar);

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
    void WindowOgl::CbError(Int32 errId, const char* errMsg) { GLIB_ERR(errMsg); }
	// --==</callback_methods>==--
}
#endif // GLIB_WINDOW