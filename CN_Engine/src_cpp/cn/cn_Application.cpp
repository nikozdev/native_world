#include <cn_pch.hpp>
#include <cn/cn_Application.h>
#include <cn/cn_MemoryMaster.h>

// Application class
namespace CN
{
	// Constructor&Destructor
	Application::Application()
	{
		m_wnd = Window::createWnd();
		m_isRunning = true;
		printf("\n--------<CN::APP::CONSTRUCTOR>--------\n");
		printf("\tApplication has been created\n");
		CN::MemoryMaster::printMem();
	}
	Application::~Application()
	{
		if (m_wnd) delete m_wnd;
		printf("\n--------<CN::APP::DESTRUCTOR>--------\n");
		printf("\tCN::APP::DESTRUCTOR: Application has been destroyed.\n");
		CN::MemoryMaster::printMem();
	}

	void Application::run()
	{
		m_isRunning = true;
		while (m_isRunning)
		{
			glClearColor(0.0f, 0.3f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
#ifdef USE_EVSYS
			if (EV::EvSys::isPressed_key(GLFW_KEY_ESCAPE))
			{
				m_wnd->closeWindow();
				m_isRunning = false;
			}
#endif
			m_wnd->onUpdate();
		}
	}
}