#include <nwg_pch.hpp>

#include <native_world.hpp>
#include <nwg_game_state.h>

int main(int nArgs, char* strArgs[])
{
	try {
		NW::CoreEngine* pGameEngine = &NW::CoreEngine::Get();
		if (!pGameEngine->Init()) return -1;
		pGameEngine->AddState(NW::MemSys::NewT<NWG::GameState>());
		pGameEngine->Run();
	} catch (std::exception ext){
		NW_ERR(ext.what());
	}

	return 0;
}