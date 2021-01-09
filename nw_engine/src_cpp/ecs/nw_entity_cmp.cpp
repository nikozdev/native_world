#include "nw_pch.hpp"
#include "nw_entity_cmp.h"

#include <ecs/nw_scene.h>
#include <ecs/nw_entity.h>

namespace NW
{
	// --==<AbstractEntityCmp>==--
	AEntityCmp::AEntityCmp(AEntity& rEntity, const CmpTypeId& TypeIndex) :
		m_pEntity(&rEntity),
		m_TypeIndex(TypeIndex), m_bIsEnabled(true)
	{
		m_pEntity->AddAComponent(this);
		Scene::Get().AddAComponent(this);
	}
	AEntityCmp::AEntityCmp(AEntityCmp& rCpy) :
		m_pEntity(rCpy.m_pEntity),
		m_TypeIndex(rCpy.m_TypeIndex), m_bIsEnabled(rCpy.m_bIsEnabled)
	{
		Scene::Get().AddAComponent(this);
	}
	AEntityCmp::~AEntityCmp()
	{
		Scene::Get().RemoveAComponent(this);
	}
	// --==</AbstractEntityCmp>==--
}