#include <nw_pch.hpp>
#include "nw_entity.h"

#include <ecs/nw_scene.h>

#include <lua/nw_lua_engine.h>

#include <sys/nw_data_sys.h>
#include <sys/nw_mem_sys.h>

namespace NW
{
	AEntity::AEntity() :
		ADataRes("ent_"),
		m_pOverEnt(nullptr),
		m_bIsEnabled(true)
	{
		m_strName += std::to_string(m_unId);
		DataSys::AddDataRes<AEntity>(this);
	}
	AEntity::AEntity(const AEntity& rCpy) :
		ADataRes(rCpy),
		m_bIsEnabled(rCpy.m_bIsEnabled),
		m_pOverEnt(rCpy.m_pOverEnt), m_SubEnts(rCpy.m_SubEnts),
		m_ACmps(rCpy.m_ACmps)
	{
		DataSys::AddDataRes<AEntity>(this);
	}
	AEntity::~AEntity()
	{
		if (m_pOverEnt != nullptr) { m_pOverEnt->m_SubEnts.erase(m_unId); }
		while (!m_ACmps.empty()) { MemSys::DelT<AEntityCmp>(m_ACmps.begin()->second); }
		while (!m_SubEnts.empty()) { MemSys::DelT<AEntity>(m_SubEnts.begin()->second); }
		DataSys::RmvDataRes<AEntity>(GetName());
	}

	// --getters
	// --setters
	void AEntity::SetName(const char* strName) { m_strName = strName; }
	void AEntity::SetEnabled(bool bIsEnabled) {
		m_bIsEnabled = bIsEnabled;
		for (auto itEnt : m_SubEnts) itEnt.second->SetEnabled(bIsEnabled);
		for (auto itCmp : m_ACmps) itCmp.second->SetEnabled(bIsEnabled);
	}
	void AEntity::SetOverEnt(AEntity* pEnt) {
		if (pEnt == nullptr) {
			if (m_pOverEnt == nullptr) {
			}
			else {
				m_pOverEnt->m_SubEnts.erase(m_unId);
			}
		}
		else {
			if (m_pOverEnt == nullptr) {
			}
			else {
				m_pOverEnt->m_SubEnts.erase(m_unId);
			}
		}
		m_pOverEnt = pEnt;
		pEnt->m_SubEnts[m_unId] = this;
	}
	void AEntity::AddSubEnt(AEntity* pEnt) {
		if (pEnt == nullptr) { return; }
		m_SubEnts[pEnt->m_unId] = pEnt;
		pEnt->m_pOverEnt = this;
	}
	void AEntity::RmvSubEnt(UInt32 unId) {
		RefEnts::iterator itSub = m_SubEnts.find(unId);
		if (itSub == m_SubEnts.end()) { return; }
		itSub->second->m_pOverEnt = nullptr;
		m_SubEnts.erase(unId);
	}
}