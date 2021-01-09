#include <nw_pch.hpp>
#include "nw_entity.h"

#include <ecs/nw_scene.h>

#include <lua/nw_lua_vm.h>

#include <sys/nw_data_sys.h>
#include <sys/nw_mem_sys.h>
#include <sys/nw_log_sys.h>

namespace NW
{
	AEntity::AEntity() :
		m_unId(0), m_strName("ent_"), m_bIsEnabled(true), m_bIsDestroyed(false),
		m_pOverEnt(nullptr) { m_strName += std::to_string(m_unId); }
	AEntity::AEntity(UInt32 unId) :
		m_unId(unId), m_strName("ent_"), m_bIsEnabled(true), m_bIsDestroyed(false),
		m_pOverEnt(nullptr) { m_strName += std::to_string(m_unId); }
	AEntity::AEntity(const AEntity& rEnt) :
		m_unId(rEnt.m_unId), m_strName(rEnt.m_strName), m_bIsDestroyed(false),
		m_bIsEnabled(rEnt.m_bIsEnabled),
		m_pOverEnt(nullptr), m_SubEnts(rEnt.m_SubEnts),
		m_ACmps(rEnt.m_ACmps) { if (rEnt.m_pOverEnt) { rEnt.m_pOverEnt->AddSubEnt(this); } }
	AEntity::~AEntity() {
		m_bIsDestroyed = true;
		if (m_pOverEnt != nullptr) m_pOverEnt->RemoveSubEnt(m_unId);
		while (!m_SubEnts.empty()) {
			UInt32 unId = m_SubEnts.begin()->second->GetId();
			auto& rEnts = Scene::Get().m_Ents;
			rEnts.erase(rEnts.find(unId));
		}
		while (!m_ACmps.empty()) { MemSys::DelT<AEntityCmp>(m_ACmps.begin()->second); m_ACmps.erase(m_ACmps.begin()); }
	}

	// --getters
	// --setters
	void AEntity::SetName(const char* strName) {
		bool bRepeat = Scene::Get().GetEntity(strName) != nullptr;
		m_strName = strName;
		while (bRepeat) {
			bRepeat = Scene::Get().GetEntity(&(m_strName + "_")[0]) != nullptr;
			m_strName += "_";
		}

		if (LuaScriptCmp* pLSCmp = GetComponent<LuaScriptCmp>()) {
			pLSCmp->GetScript()->SetName(&(m_strName + std::to_string(GetId()))[0]);
		}
	}
	void AEntity::SetEnabled(bool bIsEnabled) {
		m_bIsEnabled = bIsEnabled;
		for (auto itEnt : m_SubEnts) itEnt.second->SetEnabled(bIsEnabled);
		for (auto itCmp : m_ACmps) itCmp.second->SetEnabled(bIsEnabled);
		
		if (LuaScriptCmp* pLSCmp = GetComponent<LuaScriptCmp>()) {
			if (bIsEnabled) { pLSCmp->OnEnable(); }
			else { pLSCmp->OnDisable(); }
		}
	}

	void AEntity::SetOverEnt(AEntity* pOverEnt) {
		auto& rOverEnts = Scene::Get().m_OverEnts;
		if (m_pOverEnt == nullptr) {
			if (pOverEnt == nullptr) { }
			else { rOverEnts.erase(rOverEnts.find(m_unId)); }
		}
		else {
			if (pOverEnt == nullptr) {
				Scene::Get().m_OverEnts[m_unId] = this;
			}
			else { }
		}
		m_pOverEnt = pOverEnt;
	}
	void AEntity::AddSubEnt(AEntity* pSubEnt) {
		if (pSubEnt == nullptr || pSubEnt == this ||
			m_SubEnts.find(pSubEnt->GetId()) != m_SubEnts.end()) { return; }
		m_SubEnts[pSubEnt->GetId()] = pSubEnt;
		pSubEnt->SetOverEnt(this);
	}
	void AEntity::RemoveSubEnt(UInt32 unId) {
		RefEnts::iterator itEnt = m_SubEnts.find(unId);
		if (itEnt == m_SubEnts.end()) { return; }
		if (!itEnt->second->m_bIsDestroyed) { itEnt->second->SetOverEnt(nullptr); }
		itEnt->second->m_pOverEnt = nullptr;
		m_SubEnts.erase(itEnt);
	}
	void AEntity::AddAComponent(AEntityCmp* pCmp) {
		if (HasAComponent(pCmp->GetTypeId())) { return; }
		m_ACmps[pCmp->GetTypeId()] = pCmp;
	}
	void AEntity::RemoveAComponent(CmpTypeId TypeIndex) {
		ACmps::iterator& itCmp = m_ACmps.find(TypeIndex);
		if (itCmp == m_ACmps.end()) { return; }
		MemSys::DelT<AEntityCmp>(itCmp->second);
		m_ACmps.erase(itCmp);
	}
	void AEntity::DestroyAComponent(CmpTypeId TypeIndex) {
		ACmps::iterator itCmp = m_ACmps.find(TypeIndex);
		if (itCmp == m_ACmps.end()) return;
		MemSys::DelT<AEntityCmp>(itCmp->second);
		m_ACmps.erase(itCmp);
	}
}