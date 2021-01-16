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
		using ACmps = HashMap<TypeInfo, AEntityCmp*>;
		using Ents = HashMap<UInt32, AEntity>;
		using RefEnts = HashMap<UInt32, AEntity*>;
		friend class AEntityCmp;
	public:
		AEntity();
		AEntity(const AEntity& rCpy);
		virtual ~AEntity();

		// --getters
		inline AEntity* GetOverEnt() { return m_pOverEnt; }
		inline RefEnts& GetSubEnts() { return m_SubEnts; }
		inline ACmps& GetACmps(const TypeInfo& rTypeInfo);
		inline AEntityCmp* GetACmp(const TypeInfo& rTypeInfo);
		template <class CmpType> inline CmpType* GetCmp();
		// --setters
		void SetEnabled(bool bIsEnabled);
		void SetName(const char* strName);
		void SetOverEnt(AEntity* pEnt);
		void AddSubEnt(AEntity* pEnt);
		void RmvSubEnt(UInt32 unId);
		// --predicates
		inline bool IsEnabled() { return m_bIsEnabled; }
		inline bool HasACmp(const TypeInfo& rTypeInfo) { return GetACmp(rTypeInfo) != nullptr; }
		template <class CmpType>
		inline bool HasCmp() { return GetCmp<CmpType>() != nullptr; }

		// --core_methods
		inline void AddACmp(AEntityCmp* pCmp);
		inline void RmvACmp(const TypeInfo& rTypeInfo);
		template <class CmpType, typename ...Args>
		inline CmpType* AddCmp(Args... Arguments);
		template <class CmpType>
		inline void RmvCmp();
		// --data_methods
		virtual bool LoadF(const char* strName) override { return true; }
		virtual bool SaveF(const char* strName) override { return true; }
	private:
		bool m_bIsEnabled;
		ACmps m_ACmps;
		AEntity* m_pOverEnt;
		RefEnts m_SubEnts;
	};
	inline void AEntity::AddACmp(AEntityCmp* pCmp) {
		if (pCmp == nullptr) { return; }
		if (HasACmp(pCmp->GetTypeInfo())) { return; }
		m_ACmps[pCmp->GetTypeInfo()] = pCmp;
	}
	inline void AEntity::RmvACmp(const TypeInfo& rTypeInfo) { m_ACmps.erase(rTypeInfo); }

	inline AEntityCmp* AEntity::GetACmp(const TypeInfo& rTypeInfo) {
		return m_ACmps.find(rTypeInfo) == m_ACmps.end() ? nullptr : m_ACmps[rTypeInfo];
	}
	template <class CmpType>
	inline CmpType* AEntity::GetCmp() {
		return m_ACmps.find(TypeInfo(typeid(CmpType))) == m_ACmps.end() ?
			nullptr : static_cast<CmpType*>(m_ACmps[TypeInfo(typeid(CmpType))]);
	}
	template <class CmpType, typename ...Args>
	inline CmpType* AEntity::AddCmp(Args... Arguments) {
		if (HasCmp<CmpType>()) { return nullptr; }
		CmpType* pCmp = MemSys::NewT<CmpType>(*this, std::forward<Args>()...);
		m_ACmps[pCmp->GetTypeInfo()] = pCmp;
		return pCmp;
	}
	template <class CmpType>
	inline void AEntity::RmvCmp() {
		ACmps::iterator& itCmp = m_ACmps.find(TypeInfo(typeid(CmpType)));
		if (itCmp == m_ACmps.end()) { return; }
		MemSys::DelT<CmpType>(itCmp->second);
		m_ACmps.erase(itCmp);
	}
}
#endif	// ECS_AENTITY_H