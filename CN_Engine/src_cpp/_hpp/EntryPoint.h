#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H
/// This file has to be included into an application cpp.
/// In this dll project we only need to include it into engine header

#ifdef CN_PLATFORM_WINDOWS
namespace CN
{
	extern Application* cnCreateApp();
}
/// Entry point of any application
/// It can be included in cpp file of any project only once
int main(int argc, char* argv)
{
	auto app = CN::cnCreateApp();
	app->run();
	delete app;
}
#endif

#endif