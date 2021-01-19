#ifndef ECS_SCENE_H
#define ECS_SCENE_H

#include <nwlib/nw_singleton.h>

#include <nw_glib_decl.hpp>

namespace NW
{
	/// Scene Class
	class NW_API Scene : public ASingleton<Scene>
	{
		friend class ASingleton <Scene>;
	private:
		Scene();
		Scene(Scene& rScn) = delete;
		void operator=(Scene& rScn) = delete;
	public:
		~Scene();

		// --getters

		// --setters
		// --core_methods
		void Update();
	private:
	};
}

#endif // ECS_SCENE_H