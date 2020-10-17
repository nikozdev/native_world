#ifndef CN_WORLD_H
#define CN_WORLD_H

#include <cn_Application.h>

#pragma comment (lib, "CN_Engine.lib")

// Predeclarations
namespace CN
{
	class CHN_API Application;
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