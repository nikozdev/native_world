#ifndef CHEER_NIK_APP_H
#define CHEER_NIK_APP_H

#include <cn_core.hpp>
#include <cn_pch.hpp>
#include <cn/window/WinWindow.h>

#include <gl/gl_declarations.hpp>

// Predeclarations
namespace CN
{
	namespace EV
	{
		class CN_API Event;
		class CN_API EvDis;
		class CN_API EvSys;
		class CN_API WndCloseEvent;
		class CN_API WndResizeEvent;
		class CN_API MouseButtonEvent;
		class CN_API KeyPressedEvent;
		class CN_API KeyReleasedEvent;
	}
}
// Application
namespace CN
{
	/// Application core class
	/// --Contains application layer function (run->while(running))
	/// --Has OnEvent
	class CN_API Application
	{
	public:
		// Constructor&Destructor
		Application();
		virtual ~Application();
		
		// Core functions
		void run();
		void update();
		void render();
		/// Get window events and create dispatcher for them
		void onEvent(EV::Event& newEvent);
	private:
		// CN variables
		Window* m_wnd;
		// GL variables
		GL::Renderer* m_renderer;
		
		// Other fields
		bool m_isRunning;
		// Time: current frame, last frame and the difference between them
		float m_currTime, m_lastTime, m_deltaTime;
	private: // Functions
		bool onWndClosed(EV::WndCloseEvent ev);

		bool onKeyPressed(EV::KeyPressedEvent ev);
	};

	/// Has to be defined in client - function for entry point
	Application* cnCreateApp();
}

#endif