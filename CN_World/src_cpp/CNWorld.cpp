#include <CNWorld.h>
#include <CheerNik.h>

namespace CNW
{
	CNWorld::CNWorld() : Application()
	{
	}
	CNWorld::~CNWorld()
	{
	}
}

// Define Application Creation for Entry Point of the Engine
namespace CN
{
	Application* cnCreateApp()
	{
		MemoryMaster::printMem();
		return new CNW::CNWorld();
	}
}