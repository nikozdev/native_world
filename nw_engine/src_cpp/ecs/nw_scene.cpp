#include <nw_pch.hpp>
#include "nw_scene.h"

#include <glib/control/nw_draw_engine.h>
#include <glib/vision/nw_gcamera.h>
#include <glib/gcontext/nw_framebuf.h>

#include <core/nw_core_engine.h>
#include <sys/nw_data_sys.h>
#include <sys/nw_log_sys.h>
#include <sys/nw_mem_sys.h>

#include <lua.hpp>

namespace NW
{
	Scene::Scene() :
        m_pGCamera(nullptr), m_pFrameBuf(nullptr)
    {
        m_EntIdStack.push(1);

        FrameBufInfo fbInfo;
        fbInfo.unWidth = 800;
        fbInfo.unHeight = 600;
        fbInfo.unSamples = 1;
        m_pFrameBuf = AFrameBuf::Create("fmb_scene", fbInfo);
    }
	Scene::~Scene() {
        if (m_pFrameBuf != nullptr) { MemSys::DelT<AFrameBuf>(m_pFrameBuf); }
        m_Ents.clear();
        m_ACmps.clear();
    }

    // --setters
    void Scene::SetGCamera(GCamera* pGCamera) { m_pGCamera = pGCamera; }
    void Scene::SetViewport(const V4f& xywhViewport) {
        m_xywhViewport = xywhViewport;
        V2f whSize = { xywhViewport.z - xywhViewport.x, xywhViewport.w - xywhViewport.y };
        m_pFrameBuf->SetSizeWH(whSize.x, whSize.y);
    }

    AEntity* Scene::CreateEntity() {
        UInt32 unId = 0;
        if (m_EntIdStack.size() == 1) { unId = m_EntIdStack.top(); m_EntIdStack.top()++; }
        else if (m_EntIdStack.size() > 1) { unId = m_EntIdStack.top(); m_EntIdStack.pop(); }
        m_Ents[unId] = AEntity(unId);
        m_OverEnts[unId] = &m_Ents[unId];
        return m_OverEnts[unId];
    }
    void Scene::DestroyEntity(const char* strName) {
        Ents::iterator& itEnt = std::find_if(m_Ents.begin(), m_Ents.end(),
            [=](std::pair<const UInt32, AEntity>& rEnt)->bool { return strcmp(rEnt.second.GetName(), strName) == 0; });
        if (itEnt == m_Ents.end()) { return; };
        OnDestroyEntity(itEnt);
    }
    void Scene::DestroyEntity(AEntity* pEnt) {
        if (pEnt == nullptr) { return; }
        Ents::iterator itEnt = m_Ents.begin();
        while (itEnt++ != m_Ents.end()) { if (&itEnt->second == pEnt) { OnDestroyEntity(itEnt); return; } }
    }
    void Scene::DestroyEntity(UInt32 unId) {
        Ents::iterator& itEnt = m_Ents.find(unId);
        if (itEnt == m_Ents.end()) { return; }
        OnDestroyEntity(itEnt);
    }

    // --==<core_methods>==--
    void Scene::Update()
    {
        while (!m_DestroyEnts.empty()) {
            m_OverEnts.erase(m_DestroyEnts.back()->second.GetId());
            m_EntIdStack.push(m_DestroyEnts.back()->second.GetId());
            m_Ents.erase(m_DestroyEnts.back());
            m_DestroyEnts.pop_back();
        }
        
        DrawEngine::GetState("des_scene").pGCamera = m_pGCamera;
        DrawEngine::GetState("des_scene").pFrameBuf = m_pFrameBuf;
        DrawEngine::GetState("des_scene").xywhViewport = m_xywhViewport;
        DrawEngine::GetState("des_scene").unDrawOrder = 0;
        DrawEngine::GetState("des_scene").DPrimitive = PT_TRIANGLES;
        
        for (auto pCmp : m_ACmps) {
            if (!pCmp->IsEnabled()) continue;
            pCmp->OnUpdate();
        }

    }
	// --==</core_methods>==--

    // --==<data_methods>==--
	bool Scene::SaveF(const char* strFPath)
	{   // Very temporaty: here is too much string processing, but I simply need some way to serialize a scene
        std::stringstream strStream;
        strStream <<
            "scene_data = {"
            ",\n    entities = {";
        auto itEnt = m_Ents.begin();
        while (itEnt != m_Ents.end()) {
        }
        strStream << "\n    }"; // Entities are done
        strStream << "\n}";   // Scene is done
        String& strFile = strStream.str();
        strStream = std::stringstream();
        return DataSys::SaveF_string(strFPath, strFile.c_str(), strFile.size() * sizeof(char));
	}
	bool Scene::LoadF(const char* strFPath)
	{
        String strFile = "", strLine = "";
        Int32 nBegl = 0, nC = 0, nEndl = 0;
        if (!DataSys::LoadF_string(strFPath, strFile)) { return false; }

        auto setNewLine = [&]()->void {
            nBegl = nEndl, nEndl = strFile.find('\n', nEndl + 1); nC = 0;
            strLine = strFile.substr(nBegl, nEndl - nBegl);
        };
        setNewLine();
        while (nEndl < strFile.size() && nEndl > 0) {
            if ((nC = strLine.find("name = \"")) != -1) {
            } else if ((nC = strLine.find("entities = {")) != -1) {
                setNewLine();
                while ((nC = strLine.find("entity_", nC)) != -1) {
                    AEntity* pEnt = CreateEntity();
                    setNewLine();
                }
            }
            setNewLine();
        }
        return true;
	}
	// --==</data_methods>==--
}