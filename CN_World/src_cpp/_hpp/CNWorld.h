#ifndef CNW_WORLD_H
#define CNW_WORLD_H

#include <cn/cn_Application.h>

// Predeclarations
namespace CN
{
	class CN_API Application;
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