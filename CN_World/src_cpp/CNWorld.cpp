#include <CNWorld.h>
#include <CheerNik.h>

namespace CNW
{
	CNWorld::CNWorld() : Application()
	{
	}
	CNWorld::~CNWorld()
	{
		CN::MemoryMaster::printMem();
	}
}

// Define Application Creation for Entry Point of the Engine
namespace CN
{
	Application* cnCreateApp()
	{
		return new CNW::CNWorld();
	}
}