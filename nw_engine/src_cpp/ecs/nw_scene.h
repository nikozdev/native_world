#ifndef ECS_SCENE_H
#define ECS_SCENE_H

#include <ecs/nw_entity.h>

#include <ecs/nw_transform_cmp.h>
#include <ecs/nw_graphics_cmp.h>
#include <ecs/nw_script_cmp.h>
#include <ecs/nw_physics_cmp.h>
#include <ecs/nw_collider_cmp.h>

#include <core/nw_singleton.h>
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
		using Ents = List2<AEntity>;
		using RefEnts = DArray<AEntity*>;
		using DestroyEnts = DArray<Ents::iterator>;
		using CmpTypeId = std::type_index;
		using ACmps = DArray<AEntityCmp*>;
		friend class ASingleton <Scene>;
	private:
		Scene();
	public:
		Scene(Scene& rScn) = delete;
		void operator=(Scene& rScn) = delete;
		~Scene();

		// --getters
		inline const char* GetName() const { return &m_strName[0]; }
		inline GCamera* GetGCamera() { return m_pGCamera; }
		inline AFrameBuf* GetFrameBuf() { return m_pFrameBuf; }
		inline const V4f& GetViewport() const { return m_xywhViewport; }
		// --setters
		inline void SetName(const char* strName) { m_strName = strName; }
		void SetGCamera(GCamera* pGCamera);
		void SetViewport(const V4f& xywhViewport);

		// --core_methods
		void Update();
		// -- Entities
		inline Ents& GetEnts() { return m_Ents; }
		inline AEntity* GetEntity(UInt32 unId) {
			Ents::iterator itEnt = FIND_BY_FUNC(m_Ents, AEntity&, unId, .GetId);
			return itEnt == m_Ents.end() ? nullptr : &*itEnt;
		}
		inline AEntity* GetEntity(const char* strName) {
			Ents::iterator itEnt = std::find_if(m_Ents.begin(), m_Ents.end(),
				[=](AEntity& rEnt)->bool {return strcmp(rEnt.GetName(), strName) == 0; });
			return itEnt == m_Ents.end() ? nullptr : &*itEnt;
		}
		inline AEntity* CreateEntity() {
			m_Ents.push_back(AEntity());
			return &*(--m_Ents.end());
		}
		inline void OnDestroyEntity(const Ents::iterator& itEnt) {
			auto itDestEnt = std::find(m_DestroyEnts.begin(), m_DestroyEnts.end(), itEnt);
			if (itDestEnt == m_DestroyEnts.end()) m_DestroyEnts.push_back(itEnt);
		}
		inline void DestroyEntity(const char* strName) {
			Ents::iterator itEnt = std::find_if(m_Ents.begin(), m_Ents.end(),
				[=](AEntity& rEnt)->bool {return strcmp(rEnt.GetName(), strName) == 0; });
			if (itEnt == m_Ents.end()) { return; };
			//m_Ents.erase(itEnt);
			OnDestroyEntity(itEnt);
		}
		inline void DestroyEntity(AEntity* pEnt) {
			if (pEnt == nullptr) { return; }
			Ents::iterator itEnt = m_Ents.begin();
			while (itEnt != m_Ents.end()) {
				if (&*itEnt == pEnt) { OnDestroyEntity(itEnt); return; }
				else { itEnt++; }
			}
		}
		inline void DestroyEntity(UInt32 unId) {
			Ents::iterator itEnt = FIND_BY_FUNC(m_Ents, AEntity&, unId, .GetId);
			if (itEnt == m_Ents.end()) { return; };
			// m_Ents.erase(itEnt);
			OnDestroyEntity(itEnt);
		}
		// -- AComponents
		inline ACmps& GetAComponents() { return m_ACmps; }
		inline AEntityCmp* GetAComponent(UInt32 unId) {
			ACmps::iterator& itCmp = FIND_BY_FUNC(m_ACmps, AEntityCmp*, unId, ->GetCmpId);
			return itCmp == m_ACmps.end() ? nullptr : *itCmp;
		}
		inline void AddAComponent(AEntityCmp* pCmp) {
			if (pCmp == nullptr) return;
			if (HasAComponent(pCmp->GetCmpId())) return;
			m_ACmps.push_back(pCmp);
		}
		inline void RemoveAComponent(UInt32 unId) {
			ACmps::iterator& itCmp = FIND_BY_FUNC(m_ACmps, AEntityCmp*, unId, ->GetCmpId);
			if (itCmp == m_ACmps.end()) return;
			if ((*itCmp)->GetCmpId() != unId) return;
			m_ACmps.erase(itCmp);
		}
		inline bool HasAComponent(UInt32 unId) { return GetAComponent(unId) != nullptr; }
		// --data_methods
		bool SaveF(const char* strFPath);
		bool LoadF(const char* strFPath);
	private:
		String m_strName;
		
		Ents m_Ents;
		DestroyEnts m_DestroyEnts;

		ACmps m_ACmps;

		GCamera* m_pGCamera;
		AFrameBuf* m_pFrameBuf;
		V4f m_xywhViewport = V4f{ 0.0f, 0.0f, 800.0f, 600.0f };
	};
}

#endif // ECS_SCENE_H