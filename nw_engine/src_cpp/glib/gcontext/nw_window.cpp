#include <nw_pch.hpp>
#include "nw_window.h"

#include <sys/nw_mem_sys.h>

#if (defined NW_WINDOW)
namespace NW
{
	AWindow* AWindow::Create(const WindowInfo& rWindowInfo, WApiTypes WApiType)
	{
		AWindow* pWindow = nullptr;
		switch (WApiType) {
	#if (NW_WINDOW & NW_WINDOW_CONSOLE)
		case WApiTypes::WAPI_COUT:
			pWindow = MemSys::NewT<WindowCout>(rWindowInfo);
			break;
	#endif	// NW_WINDOW
	#if (NW_WINDOW & NW_WINDOW_GLFW)
		case WApiTypes::WAPI_GLFW:
			pWindow = MemSys::NewT<WindowOgl>(rWindowInfo);
			break;
	#endif	// NW_WINDOW
		default:
			NW_ERR("Window type is undefined");
			break;
		}
		return pWindow;
	}
}

#endif // NW_WINDOW
#if (NW_WINDOW & NW_WINDOW_GLFW)
#include <glfw/glfw3.h>
namespace NW
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
		printf("NW::WIN_WINDOW::INIT: %s::%dx%d\n",
			&m_WindowInfo.strTitle[0], m_WindowInfo.unWidth, m_WindowInfo.unHeight);
	}
	WindowOgl::~WindowOgl()
	{
	}

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

	// --==<Core Functions>==--
	bool WindowOgl::Init()
	{
		// init glfw and configure the window
		if (!s_GLFWinit) {
			s_GLFWinit = glfwInit() == GLFW_TRUE;
			NW_ASSERT(s_GLFWinit, "Could not initialize GLFW");
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Set window pointer
		m_pNative = glfwCreateWindow(static_cast<Int32>(m_WindowInfo.unWidth), static_cast<Int32>(m_WindowInfo.unHeight),
			&m_WindowInfo.strTitle[0], nullptr, nullptr);

		m_pGContext.reset(MemSys::NewT<GContextOgl>(m_pNative));
		m_pGContext->OnInit();

		SetVSync(m_WindowInfo.bVSync);
		// Bind own data pointer to window. It allows to get this pointer in callback
		glfwSetWindowUserPointer(m_pNative, &m_WindowInfo);

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
	// --==</Core Functions>==--
}
#endif // NW_WINDOW
#if (NW_WINDOW & NW_WINDOW_CONSOLE)

#include <glib/cout/window/cout_GContext.h>
#include <glib/cout/render/cout_FrameBuf.h>

#include <cn/events/cn_EvSys.h>

#include <cn/library/cn_MemSys.h>

namespace NW
{
	CoutWindow::CoutWindow(const WindowInfo& windowInfo) :
		AWindow(),
		m_rGContext(MemSys::NewT<GContextCout>(this)),
		m_windowInfo(windowInfo), m_xywhRect{ 0, 0, 1, 1 }
	{
	}
	CoutWindow::~CoutWindow()
	{
		OnQuit();
	}

	// --setters
	void CoutWindow::SetVSync(bool enabled)
	{
		m_windowInfo.bVSync = enabled;
	}

	// --core_methods
	bool CoutWindow::OnInit()
	{
		m_hCout = m_hOrigCout = GetStdHandle(STD_OUTPUT_HANDLE);
		m_hCin = GetStdHandle(STD_INPUT_HANDLE);

		if (m_hCout == INVALID_HANDLE_VALUE || m_hCin == INVALID_HANDLE_VALUE)
			return false;

		if (!SetConsoleWindowInfo(m_hCout, TRUE, &m_xywhRect))
			return false;
		m_xywhRect = { 0, 0, (Int16)m_windowInfo.usiWidth - 1, (Int16)m_windowInfo.usiHeight - 1 };

		if (!SetConsoleScreenBufferSize(m_hCout, { (Int16)m_windowInfo.usiWidth,
			(Int16)m_windowInfo.usiHeight }) ||
			!SetConsoleActiveScreenBuffer(m_hCout))
			return false;

		m_rGContext->SetPxSize({ 8, 16 });
		if (!m_rGContext->OnInit())
			return false;

		if (!SetConsoleWindowInfo(m_hCout, TRUE, &m_xywhRect))
			return false;

		if (!SetConsoleMode(m_hCin, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
			return false;

		char title[128];
		sprintf_s(title, 128, "%s::FPS: %f\0", m_windowInfo.strTitle.c_str(), 0.0f);
		SetConsoleTitleA(title);

		return true;
	}
	void CoutWindow::OnQuit()
	{
		m_rGContext->OnQuit();
		SetConsoleActiveScreenBuffer(m_hOrigCout);
		system("\a");
		MemSys::Get().MemoryLog();
		system("pause");
	}

	void CoutWindow::OnUpdate()
	{
		char title[128];
		sprintf_s(title, 128, "%s::FPS: %2f\0", m_windowInfo.strTitle.c_str(), EvSys::s_fTimeDelta);
		SetConsoleTitleA(title);

		m_rGContext->SwapBuffers();
	}
}
#endif // NW_WINDOW