#include <CNWorld.h>

int main(int argc, char** argv)
{
	auto app = CN::cnCreateApp();
	app->run();
	delete app;
}