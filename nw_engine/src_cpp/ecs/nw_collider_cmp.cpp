#include <nw_pch.hpp>
#include "nw_collider_cmp.h"

namespace NW
{
	// --==<Collider2dRectCmp>==--
	Collider2dRectCmp::Collider2dRectCmp(AEntity& rEntity) :
		ACollider2dCmp(rEntity)
	{
	}

	// --core_methods
	void Collider2dRectCmp::OnUpdate()
	{
	}
	// --==</Collider2dRectCmp>==--
	
	// --==<Collider2dPolyCmp>==--
	Collider2dPolyCmp::Collider2dPolyCmp(AEntity& rEntity) :
		ACollider2dCmp(rEntity)
	{
	}

	// --core_methods
	void Collider2dPolyCmp::OnUpdate()
	{
	}
	// --==</Collider2dPolyCmp>==--
}