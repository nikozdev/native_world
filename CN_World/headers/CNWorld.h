#ifndef CN_WORLD_H
#define CN_WORLD_H

#include <CheerNik.h>

// Predeclarations
namespace CN
{
	class CHN_API Application;
	extern Application* cnCreateApp();
}

namespace CNW
{
	class CNWorld : public CN::Application
	{
	public:
		//Constructors&Destructor
		CNWorld();
		~CNWorld();
	};
}

#endif