#include <nwg_pch.hpp>

#include <native_world.hpp>

int main()
{
	try {
		NW::Engine* pEngine = &NW::Engine::Get();
		if (!pEngine->Init()) return -1;
		pEngine->Run();
	} catch (std::exception ext){
		NW_ERR(ext.what());
	}

	return 0;
}