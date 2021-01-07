#include <nw_pch.hpp>
#include <ecs/nw_scene.h>

namespace NW
{
	// --==<Physics2dCmp>==--
	Physics2dCmp::Physics2dCmp(AEntity& rEntity) :
		AEntityCmp(rEntity, std::type_index(typeid(Physics2dCmp))),
		m_pCollider2d(nullptr) { }

	// --core_methods
	void Physics2dCmp::OnUpdate()
	{
	}
	// --==</Physics2dCmp>==--
}