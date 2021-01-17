#ifndef ECS_SCENE_H
#define ECS_SCENE_H

#include <ecs/nw_entity.h>

#include <nwlib/nw_singleton.h>

#include <nw_glib_decl.hpp>

namespace NW
{
	/// Scene Class
	class NW_API Scene : public ASingleton<Scene>
	{
		using Ents = HashMap<UInt32, AEntity>;
		using RefEnts = HashMap<UInt32, AEntity*>;
		using DestroyEnts = DArray<Ents::iterator>;
		using ACmps = DArray<AEntityCmp*>;
		friend class ASingleton <Scene>;
		friend class AEntity;
	private:
		Scene();
		Scene(Scene& rScn) = delete;
		void operator=(Scene& rScn) = delete;
	public:
		~Scene();

		// --getters

		// --setters
		void AddEnt(AEntity* pEnt);
		void RmvEnt(UInt32 unId);
		// --core_methods
		void Update();
	private:
		RefEnts m_Ents;
	};
}

#endif // ECS_SCENE_H