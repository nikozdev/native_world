#include <nwg_pch.hpp>

#include <native_world.hpp>

int main(int nArgs, char* strArgs[])
{
	try {
		NW::CoreEngine* pGameEngine = &NW::CoreEngine::Get();
		if (!pGameEngine->Init()) return -1;
		pGameEngine->Run();
	} catch (std::exception ext){
		NW_ERR(ext.what());
	}

	return 0;
}