#ifndef CHEER_NIK_APP_H
#define CHEER_NIK_APP_H

#include <cn_core.hpp>

namespace CN
{
	//class __declspec(dllexport) Application;
	class CHN_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	/// Has to be defined in client - function for entry point
	Application* cnCreateApp();
}

#endif