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
		ADataRes("ent_"),
		m_bIsEnabled(true),
		m_pOverEnt(nullptr)
	{
		m_strName += std::to_string(m_unId);
	}
	AEntity::AEntity(const AEntity& rEnt) :
		ADataRes(rEnt),
		m_bIsEnabled(rEnt.m_bIsEnabled),
		m_pOverEnt(nullptr), m_SubEnts(rEnt.m_SubEnts),
		m_ACmps(rEnt.m_ACmps)
	{
		if (rEnt.m_pOverEnt) { rEnt.m_pOverEnt->AddSubEnt(this); }
	}
	AEntity::~AEntity() {
		if (m_pOverEnt != nullptr) m_pOverEnt->RemoveSubEnt(m_unId);
		while (!m_SubEnts.empty()) { Scene::Get().DestroyEntity((*m_SubEnts.begin())->GetId()); }
		while (!m_ACmps.empty()) { MemSys::DelT<AEntityCmp>(m_ACmps.begin()->second); m_ACmps.erase(m_ACmps.begin()); }
	}

	// -- Getters
	// -- Setters
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
	void AEntity::SetOverEnt(AEntity* pOverEnt) {
		m_pOverEnt = pOverEnt;
	}
	void AEntity::SetEnabled(bool bIsEnabled) {
		m_bIsEnabled = bIsEnabled;
		for (auto itEnt : m_SubEnts) itEnt->SetEnabled(bIsEnabled);
		for (auto itCmp : m_ACmps) itCmp.second->SetEnabled(bIsEnabled);
		
		if (LuaScriptCmp* pLSCmp = GetComponent<LuaScriptCmp>()) {
			if (bIsEnabled) { pLSCmp->OnEnable(); }
			else { pLSCmp->OnDisable(); }
		}
	}

	void AEntity::AddSubEnt(AEntity* pSubEnt) {
		m_SubEnts.push_back(pSubEnt); pSubEnt->SetOverEnt(this);
	}
	void AEntity::RemoveSubEnt(UInt32 unId) {
		SubEnts::iterator itEnt = std::find_if(m_SubEnts.begin(), m_SubEnts.end(),
			[=](AEntity* pEnt)->bool {return pEnt->GetId() == unId; });
		if (itEnt != m_SubEnts.end()) { m_SubEnts.erase(itEnt); }
	}
	void AEntity::AddAComponent(AEntityCmp* pCmp) {
		if (pCmp == nullptr) return;
		if (HasAComponent(pCmp->GetTypeId())) return;
		m_ACmps[pCmp->GetTypeId()] = pCmp;
	}
	void AEntity::RemoveAComponent(CmpTypeId TypeIndex) {
		ACmps::iterator itCmp = m_ACmps.find(TypeIndex);
		if (itCmp == m_ACmps.end()) return;
		MemSys::DelT<AEntityCmp>(itCmp->second);
		m_ACmps.erase(itCmp);
	}
	void AEntity::RemoveAComponent(UInt32 unId) {
		for (ACmps::iterator itCmp = m_ACmps.begin(); itCmp != m_ACmps.end(); itCmp++) {
			if (itCmp->second->GetCmpId() == unId) {
				MemSys::DelT<AEntityCmp>(itCmp->second); m_ACmps.erase(itCmp);
			}
		}
	}
	// -- Data Methods
	bool AEntity::SaveF(const char* strFPath)
	{
		String strFile = "";
		return DataSys::SaveF_string(strFPath, &strFile[0], sizeof(char) * strFile.size());
	}
	bool AEntity::LoadF(const char* strFPath)
	{
		String strFile = "";
		if (DataSys::LoadF_string(strFPath, strFile) == false) { return false; }
		return true;
	}
	// ========<Core Methods>========
	// ========</Core Methods>========
}