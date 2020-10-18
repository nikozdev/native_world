#ifndef CHEER_NIK_APP_H
#define CHEER_NIK_APP_H

#include <cn_core.hpp>
#include <cn/window/WinWindow.h>

namespace CN
{
	//class __declspec(dllexport) Application;
	class CN_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	private:
		Window* m_wnd;
		bool m_isRunning;
	};

	/// Has to be defined in client - function for entry point
	Application* cnCreateApp();
}

#endif