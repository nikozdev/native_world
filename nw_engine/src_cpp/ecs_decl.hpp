#ifndef ECS_DECL_HPP
#define ECS_DECL_HPP

#include <nw_core.hpp>

namespace NW
{
	// --entities
	class NW_API AEntity;
	// --components
	class NW_API AEntityCmp;
	class NW_API ATransformCmp;
	class NW_API AGraphicsCmp;
	class NW_API ACollider2dCmp;
	class NW_API Physics2dCmp;
	class NW_API LuaScriptCmp;
	// --systems
	class NW_API Scene;
}

#endif	// ECS_DECL_HPP