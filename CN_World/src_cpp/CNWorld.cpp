#include <CNWorld.h>

namespace CN
{
	Application* cnCreateApp()
	{
		return new CNW::CNWorld();
	}
}

namespace CNW
{
	CNWorld::CNWorld() : Application()
	{
	}
	CNWorld::~CNWorld()
	{
	}
}