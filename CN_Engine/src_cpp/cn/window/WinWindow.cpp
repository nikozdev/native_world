#include <cn_pch.hpp>

#include <cn/Window/WinWindow.h>

#include <extern/GLEW/glew.h>
#include <extern/GLFW/glfw3.h>

#include <events/ev_EvSys.h>
#include <events/ev_Event.h>
#include <events/ev_KeyEvent.h>
#include <events/ev_WndEvent.h>
#include <events/ev_MouseEvent.h>

#include <extern/imgui/imgui.h>
#include <extern/imgui/imgui_impl_glfw.h>
#include <extern/imgui/imgui_impl_opengl3.h>

namespace CN
{
	extern bool s_wndWork;
	/// We need to know whether glfw is initialized or not
	static bool s_GLFWinit = false;
	static bool s_GLEWinit = false;
}
// Base Window class definitions
namespace CN
{
	Window* Window::createWnd(const WndDescript& descript)
	{
		return new WinWindow(descript);
	}
}
// Windows platform Window
namespace CN
{
	WinWindow::WinWindow(const WndDescript& descript)
	{
		init(descript);
		EV::EvSys::init(m_wnd);

		printf("CN::WIN_WND::CONSTRUCTOR: The window %s\nhas been created.\n",
			m_data.Title.c_str());
	}
	WinWindow::~WinWindow()
	{
		closeWindow();
		shutDown();

		printf("CN::WIN_WND::DESTRUCTOR: The window %s\nhas been destroyed.\n",
			m_data.Title.c_str());
	}

	void WinWindow::setVSync(bool enabled)
	{
		if (enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);
	m_data.vSync = enabled;
	}

	// Core functions
	void WinWindow::onDraw()
	{
		glfwSwapBuffers(m_wnd);
	}

	void WinWindow::onUpdate()
	{
		EV::EvSys::pollEvents();
	}

	void WinWindow::closeWindow()
	{
		glfwSetWindowShouldClose(m_wnd, GL_TRUE);
	}

	void WinWindow::init(const WndDescript& descript)
	{
		m_data.Title = descript.Title;
		m_data.Width = descript.Width;
		m_data.Height = descript.Height;
		printf("CN::WIN_WINDOW::INIT: %s::%dx%d\n",
			m_data.Title.c_str(), m_data.Width, m_data.Height);

		// init glfw and configure the window
		if (!s_GLFWinit)
		{
			CN_ASSERT(glfwInit() == GLFW_TRUE, "Could not initialize GLFW");
			s_GLFWinit = true;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		// Set window pointer
		m_wnd = glfwCreateWindow((int)m_data.Width, (int)m_data.Height,
			m_data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_wnd);

		if (!s_GLEWinit)
		{
			CN_ASSERT(glewInit() == GLEW_OK, "Couldn't initialize GLEW");
			s_GLEWinit = true;
			glewExperimental = GL_TRUE;
		}
		setVSync(true);
		// Bind own data pointer to window. It allows to get this pointer in callback
		glfwSetWindowUserPointer(m_wnd, &m_data);
	}
	void WinWindow::shutDown()
	{
		glfwDestroyWindow(m_wnd);
		if (s_GLFWinit) glfwTerminate();
	}
}