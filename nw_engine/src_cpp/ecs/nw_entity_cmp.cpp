#include "nw_pch.hpp"
#include "nw_entity_cmp.h"

#include <ecs/nw_entity.h>

#include <sys/nw_data_sys.h>

namespace NW
{
	// --==<AbstractEntityCmp>==--
	AEntityCmp::AEntityCmp(AEntity& rEntity, const TypeInfo& rTypeInfo) :
		ADataRes(&(String(&rTypeInfo.name()[10]) + std::to_string(rEntity.GetId()))[0]),
		m_TypeInfo(rTypeInfo),
		m_pEntity(&rEntity),
		m_bIsEnabled(true)
	{
		DataSys::AddDataRes<AEntityCmp>(this);
		m_pEntity->m_ACmps[GetTypeInfo()] = this;
	}
	AEntityCmp::AEntityCmp(const AEntityCmp& rCpy) :
		ADataRes(rCpy),
		m_TypeInfo(rCpy.m_TypeInfo),
		m_pEntity(rCpy.m_pEntity), m_bIsEnabled(rCpy.m_bIsEnabled)
	{
		DataSys::AddDataRes<AEntityCmp>(this);
		m_pEntity->m_ACmps[GetTypeInfo()] = this;
	}
	AEntityCmp::~AEntityCmp()
	{
		m_pEntity->m_ACmps.erase(GetTypeInfo());
		DataSys::RmvDataRes<AEntityCmp>(GetId());
	}
	// --==</AbstractEntityCmp>==--
}