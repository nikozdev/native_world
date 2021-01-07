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
	class NW_API AEntity : public ADataRes
	{
		using CmpTypeId = std::type_index;
		using ACmps = HashMap<CmpTypeId, AEntityCmp*>;
		using SubEnts = DArray<AEntity*>;
		friend class Scene;
	public:
		AEntity();
		explicit AEntity(const AEntity& rEnt);
		virtual ~AEntity();

		// --getters
		inline AEntity* GetOverEnt() { return m_pOverEnt; }
		inline SubEnts& GetSubEnts() { return m_SubEnts; }
		inline ACmps& GetAComponents() { return m_ACmps; }
		inline AEntity* GetSubEnt(UInt32 unId) {
			SubEnts::iterator itSubEnt = FIND_BY_FUNC(m_SubEnts, AEntity*, unId, ->GetId);
			return itSubEnt == m_SubEnts.end() ? nullptr : *itSubEnt;
		}
		inline AEntityCmp* GetAComponent(UInt32 unId) {
			ACmps::iterator itCmp = std::find_if(m_ACmps.begin(), m_ACmps.end(),
				[=](std::pair<const CmpTypeId, AEntityCmp*>& rACmp)->bool {return rACmp.second->GetCmpId() == unId; });
			return itCmp == m_ACmps.end() ? nullptr : itCmp->second;
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
		void RemoveAComponent(UInt32 unId);
		template<class CmpType, typename ...Args>
		inline CmpType* AddComponent(Args ...Arguments) {
			if (HasComponent<CmpType>()) return nullptr;
			CmpType* pCmp = MemSys::NewT<CmpType>(*this, std::forward<Args>(Arguments)...);
			return pCmp;
		}
		template<class CmpType> inline void RemoveComponent() { RemoveAComponent(CmpTypeId(typeid(CmpType))); }
		virtual void SetName(const char* strName) override;
		// -- Predicates
		inline bool IsEnabled() { return m_bIsEnabled; }
		inline bool HasSubEnt(UInt32 unId) { SubEnts::iterator itEnt = FIND_BY_FUNC(m_SubEnts, AEntity*, unId, ->GetId);  return itEnt != m_SubEnts.end(); }
		inline bool HasSubEnt(AEntity* pEntity) { return (pEntity->GetOverEnt() == this); }
		inline bool HasAComponent(UInt32 unId) { return GetAComponent(unId) != nullptr; }
		inline bool HasAComponent(const CmpTypeId& TypeIndex) { return m_ACmps.find(TypeIndex) != m_ACmps.end(); }
		template <class CmpType> inline bool HasComponent() { return HasAComponent(CmpTypeId(typeid(CmpType))); }

		// --data_methods
		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;
	private:	// Implementation Attributes
		bool m_bIsEnabled;

		AEntity* m_pOverEnt;
		SubEnts m_SubEnts;

		ACmps m_ACmps;
	};
}
#endif	// ECS_AENTITY_H