#ifndef ECS_AENTITY_H
#define ECS_AENTITY_H

#include <ecs/nw_entity_cmp.h>

#include <core/nw_data_res.h>

#include <ecs_decl.hpp>

namespace NW
{
	/// Abstract Entity class
	/// Description
	/// -- The main unit of the scene
	/// -- Owns component references of component system
	class NW_API AEntity
	{
		using CmpTypeId = std::type_index;
		using ACmps = HashMap<CmpTypeId, AEntityCmp*>;
		using RefEnts = HashMap<UInt32, AEntity*>;
	public:
		AEntity();
		AEntity(UInt32 unId);
		explicit AEntity(const AEntity& rEnt);
		virtual ~AEntity();

		// --getters
		inline UInt32 GetId() { return m_unId; }
		inline const char* GetName() { return &m_strName[0]; }

		inline AEntity* GetOverEnt() { return m_pOverEnt; }
		inline RefEnts& GetSubEnts() { return m_SubEnts; }
		inline ACmps& GetAComponents() { return m_ACmps; }
		inline AEntity* GetSubEnt(UInt32 unId) { return m_SubEnts.find(unId) == m_SubEnts.end() ? nullptr : m_SubEnts[unId]; }
		template<class CmpType>
		inline CmpType* GetAComponent(CmpTypeId TypeIndex) {
			ACmps::iterator itCmp = m_ACmps.find(TypeIndex);
			return itCmp == m_ACmps.end() ? nullptr : static_cast<CmpType*>(itCmp->second);
		}
		template<class CmpType>
		inline CmpType* GetComponent() {
			ACmps::iterator itCmp = m_ACmps.find(CmpTypeId(typeid(CmpType)));
			return itCmp == m_ACmps.end() ? nullptr : dynamic_cast<CmpType*>(itCmp->second);
		}
		// --setters
		void SetOverEnt(AEntity* pOverEnt);
		void SetEnabled(bool bIsEnabled);
		void AddSubEnt(AEntity* pSubEnt);
		void RemoveSubEnt(UInt32 unId);
		void AddAComponent(AEntityCmp* pCmp);
		void RemoveAComponent(CmpTypeId TypeIndex);
		void DestroyAComponent(CmpTypeId TypeIndex);
		template<class CmpType, typename ...Args>
		inline CmpType* CreateComponent(Args ...Arguments) {
			if (HasComponent<CmpType>()) return nullptr;
			return MemSys::NewT<CmpType>(*this, std::forward<Args>(Arguments)...);
		}
		template<class CmpType> inline void DestroyComponent() { DestroyAComponent(CmpTypeId(typeid(CmpType))); }
		void SetName(const char* strName);
		// -- Predicates
		inline bool IsEnabled() { return m_bIsEnabled; }
		inline bool HasSubEnt(UInt32 unId) { return m_SubEnts.find(unId) != m_SubEnts.end(); }
		inline bool HasSubEnt(AEntity* pEntity) { return (pEntity->GetOverEnt() == this); }
		inline bool HasAComponent(CmpTypeId TypeIndex) { return m_ACmps.find(TypeIndex) != m_ACmps.end(); }
		template <class CmpType> inline bool HasComponent() { return m_ACmps.find(CmpTypeId(typeid(CmpType))) != m_ACmps.end(); }
	private:
		UInt32 m_unId;
		String m_strName;
		bool m_bIsEnabled;
		bool m_bIsDestroyed;

		AEntity* m_pOverEnt;
		RefEnts m_SubEnts;

		ACmps m_ACmps;
	};
}
#endif	// ECS_AENTITY_H