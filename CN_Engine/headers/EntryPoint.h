#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

#ifdef CN_PLATFORM_WINDOWS

namespace CN
{
	extern Application* cnCreateApp();
}

int main(int argc, char* argv)
{
	auto app = CN::cnCreateApp();
	app->run();
	delete app;
}

#endif

#endif