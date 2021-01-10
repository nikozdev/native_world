#ifndef NW_ENTITY_SYSTEM_H
#define NW_ENTITY_SYSTEM_H
#include <ecs/nw_entity.h>

/*
namespace NW
{
	/// EntSys static class
	class NW_API EntSys
	{
	public:
		using ACmps = AEntity::ACmps;
		using Ents = AEntity::Ents;
		using SubEnts = AEntity::SubEnts;
	public:
		// --getters
		static inline AEntity* GetEntity(UInt32 unId) { return s_Ents.find(unId) == s_Ents.end() ? nullptr : &s_Ents[unId]; }
		// --setters
		// --core_methods
		static bool OnInit();
		static void OnQuit();
		static inline AEntity* CreateEntity() { return nullptr; }
	private:
		static Ents s_Ents;
		static IdStack s_IdStack;
	};
}
*/
#endif	// NW_ENTITY_SYSTEM_H