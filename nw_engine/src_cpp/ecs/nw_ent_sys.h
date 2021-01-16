#ifndef NW_ENTITY_SYSTEM_H
#define NW_ENTITY_SYSTEM_H

#include <ecs/nw_entity.h>
#include <ecs/nw_entity_cmp.h>
#include <ecs/nw_transform_cmp.h>
#include <ecs/nw_graphics_cmp.h>
#include <ecs/nw_script_cmp.h>
#include <ecs/nw_physics_cmp.h>

namespace NW
{
	/// EntitySystem static class
	class NW_API EntSys
	{
		using ACmps = HashMap<UInt32, AEntityCmp*>;
		using Ents = HashMap<UInt32, ACmps>;
		template <class CmpType> using Cmps = HashMap<UInt32, CmpType>;
	public:
		// --getters
		inline Ents& GetEnts() { return s_Ents; }
		inline AEntity* GetEnt(UInt32 unId) { s_Ents.find(unId) == s_Ents.end() ? nullptr : &s_Ents[unId]; }
		inline ACmps GetACmps() { return s_ACmps; }
		inline ACmps GetACmp() { return s_ACmps; }
		template <class CmpType> inline Cmps<CmpType>& GetCmps() { static Cmps<CmpType> s_Cmps; return s_Cmps; }
		// --setters
		AEntity* AddEnt();
		void RmvEnt(UInt32 unId);
		// --core_methods
		bool OnInit();
		void OnQuit();

	private:
		static Ents s_Ents;
		static ACmps s_ACmps;
		static IdStack s_CmpIdStack;
	};
}

#endif	// NW_ENTITY_SYSTEM_H