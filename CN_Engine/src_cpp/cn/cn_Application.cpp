#include <cn_pch.hpp>
#include <cn/cn_Application.h>
#include <cn/cn_MemoryMaster.h>
#include <gl/gl_Renderer.h>

#include <events/ev_EvSys.h>
#include <events/ev_KeyEvent.h>
#include <events/ev_MouseEvent.h>
#include <events/ev_WndEvent.h>

#include <math/cn_math.h>

#include <extern/GLFW/glfw3.h>

// Application class
namespace CN
{
/// This macro allows to bind any function to some class as a lambda expression
/// So, we can give a function as an argument
#define BIND_EVENT_FN(func)std::bind(&func, this, std::placeholders::_1)

	// Constructor&Destructor
	Application::Application() :
		m_currTime(0.0f), m_deltaTime(0.0f), m_lastTime(0.0f)
	{
		m_wnd = Window::createWnd();
		m_wnd->setEventCallback(BIND_EVENT_FN(Application::onEvent));

		m_renderer = new GL::Renderer();
		
		m_isRunning = true;

		printf("\n--------<CN::APP::CONSTRUCTOR>--------\n");
		CN_LOG("Application has been created");
		CN::MemoryMaster::printMem();
	}
	Application::~Application()
	{
		if (m_wnd) delete m_wnd;
		if (m_renderer) delete m_renderer;

		printf("\n--------<CN::APP::DESTRUCTOR>--------\n");
		CN_LOG("Application has been destroyed");
		CN::MemoryMaster::printMem();
	}

	// Core functions
	void Application::run()
	{
		m_isRunning = true;
		while (m_isRunning)
		{
			render();
			update();
		}
	}

	void Application::update()
	{
		m_currTime = glfwGetTime();
		m_deltaTime = m_currTime - m_lastTime;
		m_lastTime = m_currTime;
		m_wnd->onUpdate();
	}

	void Application::render()
	{
		m_renderer->clear();
		m_renderer->draw();
		m_wnd->onDraw();
	}

	// Event Functions
	void Application::onEvent(EV::Event& newEvent)
	{
		EV::EvDis dispatcher(newEvent);
		dispatcher.Dispatch<EV::WndCloseEvent>(BIND_EVENT_FN(Application::onWndClosed));

		dispatcher.Dispatch<EV::KeyPressedEvent>(BIND_EVENT_FN(Application::onKeyPressed));
	}
	bool Application::onWndClosed(EV::WndCloseEvent ev)
	{
		m_isRunning = false;
		return true;
	}
	bool Application::onKeyPressed(EV::KeyPressedEvent ev)
	{
		switch (ev.getKeyCode())
		{
		case GLFW_KEY_0:
			break;
		case GLFW_KEY_1:
			break;
		case GLFW_KEY_2:
			break;
		case GLFW_KEY_3:
			break;
		case GLFW_KEY_4:
			break;
		case GLFW_KEY_5:
			break;
		case GLFW_KEY_R:
			CN_LOG("R is pressed");
			break;
		case GLFW_KEY_ESCAPE:
			m_isRunning = false;
			break;
		default:
			CN_LOG(ev.getKeyCode());
			break;
		}
		return false;
	}
}