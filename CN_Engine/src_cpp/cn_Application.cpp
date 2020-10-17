#include "headers/cn_pch.hpp"
#include <cn_Application.h>

namespace CN
{
	Application::Application()
	{
		printf("CN::APP::CONSTRUCTOR: Application has been created\n");
	}
	Application::~Application()
	{
		printf("CN::APP::DESTRUCTOR: Application has been destroyed.\n");
	}

	void Application::run()
	{
		printf("CN::APP::DESTRUCTOR: Application has been ran.\n");
	}
}