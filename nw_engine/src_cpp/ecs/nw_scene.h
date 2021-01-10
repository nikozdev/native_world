#ifndef ECS_SCENE_H
#define ECS_SCENE_H

#include <ecs/nw_entity.h>

#include <ecs/nw_transform_cmp.h>
#include <ecs/nw_graphics_cmp.h>
#include <ecs/nw_script_cmp.h>
#include <ecs/nw_physics_cmp.h>
#include <ecs/nw_collider_cmp.h>

#include <lib/nw_singleton.h>
#include <core/nw_data_res.h>

#include <sys/nw_mem_sys.h>

namespace NW
{
	/// Scene Class
	/// Interface:
	/// -> Create a scene -> Set cameras and entities
	/// -> Give this scene to the renderer as an argument for BeginRender
	/// Description:
	/// -- Scenes are used for rendering data
	/// -- Renderer takes the scene data in the beginning of the frame
	/// -- Renderer sets all cameras, lights and objects in the scene
	/// -- Renderer draws it's stuff with scene data
	/// ==> Scene data - configurations of rendering
	/// ==> Scene is not an owner of camera.
	/// ==> CameraLad handles cameras and can set them as a scene pointers
	/// -- Since 25.12.2020 scene is a singleton. I don't need more than one scene
	/// -- If I need to store some scenes for a game - I can save them in files
	/// -- But actual processed scene in the actual memory is a singleton
	class NW_API Scene : public ASingleton<Scene>
	{
		using Ents = HashMap<UInt32, AEntity>;
		using RefEnts = HashMap<UInt32, AEntity*>;
		using DestroyEnts = DArray<Ents::iterator>;
		using ACmps = DArray<AEntityCmp*>;
		using IdStack = DStack<UInt32>;
		friend class ASingleton <Scene>;
		friend class AEntity;
	private:
		Scene();
		Scene(Scene& rScn) = delete;
		void operator=(Scene& rScn) = delete;
	public:
		~Scene();

		// --getters
		inline GCamera* GetGCamera() { return m_pGCamera; }
		inline AFrameBuf* GetFrameBuf() { return m_pFrameBuf; }
		inline V4f GetViewport() { return m_xywhViewport; }
		inline GraphState* GetGraphState() { return m_pGState; }

		inline Ents& GetEnts() { return m_Ents; }
		inline RefEnts& GetOverEnts() { return m_OverEnts; }
		inline AEntity* GetEntity(UInt32 unId) { return (m_Ents.find(unId) == m_Ents.end()) ? nullptr : &m_Ents[unId]; }
		inline AEntity* GetEntity(const char* strName) {
			Ents::iterator itEnt = std::find_if(m_Ents.begin(), m_Ents.end(),
				[=](std::pair<const UInt32, AEntity>& rEnt)->bool {return strcmp(rEnt.second.GetName(), strName) == 0; });
			return itEnt == m_Ents.end() ? nullptr : &itEnt->second;
		}
		// --setters
		void SetGCamera(GCamera* pGCamera);
		void SetViewport(const V4f& xywhViewport);

		AEntity* CreateEntity();
		void DestroyEntity(const char* strName);
		void DestroyEntity(AEntity* pEnt);
		void DestroyEntity(UInt32 unId);

		void AddAComponent(AEntityCmp* pCmp) { if (pCmp == nullptr) { return; } m_ACmps.push_back(pCmp); }
		void RemoveAComponent(AEntityCmp* pCmp) {
			auto& itCmp = std::find(m_ACmps.begin(), m_ACmps.end(), pCmp);
			if (itCmp == m_ACmps.end()) { return; } m_ACmps.erase(itCmp);
		}
		// --core_methods
		void Update();
		// --data_methods
		bool SaveF(const char* strFPath);
		bool LoadF(const char* strFPath);
	private:
		Ents m_Ents;
		RefEnts m_OverEnts;
		DestroyEnts m_DestroyEnts;
		IdStack m_EntIdStack;

		ACmps m_ACmps;

		GCamera* m_pGCamera;
		AFrameBuf* m_pFrameBuf;
		V4f m_xywhViewport = V4f{ 0.0f, 0.0f, 800.0f, 600.0f };
		GraphState* m_pGState;
	private:
		inline void OnDestroyEntity(const Ents::iterator& itEnt) {
			for (auto& itDestEnt : m_DestroyEnts) { if (itDestEnt == itEnt) return; }
			m_DestroyEnts.push_back(itEnt);
		}
	};
}

#endif // ECS_SCENE_H