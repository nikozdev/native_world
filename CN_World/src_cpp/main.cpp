#include <CNWorld.h>
#include <CheerNik.h>

namespace CN
{
	Application* cnCreateApp()
	{
		return new CNW::CNWorld();
	}
}