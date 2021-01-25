#include <nw_pch.hpp>
#include "glib/core/nw_gengine.h"

#include <glib/core/nw_gapi.h>

#include <glib/gcontext/nw_framebuf.h>

#include <glib/vision/nw_gcamera.h>
#include <glib/nw_texture.h>
#include <glib/vision/nw_shader.h>
#include <glib/vision/nw_gmaterial.h>

#include <sys/nw_data_sys.h>

namespace NW
{
	GEngine::GEngine() :
		m_thrRun(Thread()), m_Memory(MemArena(nullptr, 0)), m_bIsRunning(false),
		m_pGApi(RefOwner<AGApi>()),
		m_DInfo(GEngineInfo()) { }
	GEngine::~GEngine() { }

	// --setters
	GLayer* GEngine::AddLayer(const char* strName) {
		if (GetLayer(strName) != nullptr) { return nullptr; }
		m_GLayers.push_back(GLayer(strName));
		return &m_GLayers.back();
	}
	void GEngine::RmvLayer(const char* strName) {
		Layers::iterator& itDest = std::find_if(m_GLayers.begin(), m_GLayers.end(),
			[=](GLayer& rObj)->bool {return rObj.strName == strName; });
		if (itDest != m_GLayers.end()) { return; }
		m_GLayers.erase(itDest);
	}
	// --==<core_methods>==--
	bool GEngine::Init(Size szMemory)
	{
		m_bIsRunning = true;
		m_Memory = MemArena(new Byte[szMemory], szMemory);
	#if (defined NW_GRAPHICS)
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		m_pGApi.MakeRef<GApiOgl>(GetMemory());
		m_pGApi->SetClearColor(0.1f, 0.3f, 0.3f);
	#endif
	#endif	// NW_GRAPHICS

		AddLayer("gel_default");
		m_GLayer = m_GLayers.begin();
		
		return true;
	}
	void GEngine::Quit()
	{
		if (!m_bIsRunning) { return; }
		m_bIsRunning = false;

		m_GLayers.clear();

		delete[] m_Memory.GetDataBeg();
	}
	void GEngine::Run() {
		m_thrRun = std::thread([this]()->void { while (m_bIsRunning) { Update(); } });
	}
	void GEngine::Update()
	{
		m_DInfo.Reset();

		std::sort(m_GLayers.begin(), m_GLayers.end());
		for (m_GLayer = m_GLayers.begin(); m_GLayer != m_GLayers.end(); m_GLayer++) {
			m_GLayer->OnDraw(GetGApi());
			m_DInfo.szShd = m_DInfo.szShd + m_GLayer->szShdData;
			m_DInfo.szVtx = m_DInfo.szVtx + m_GLayer->szVtxData;
			m_DInfo.szIdx = m_DInfo.szIdx + m_GLayer->szIdxData;
			m_DInfo.unDrawCalls += m_GLayer->unDrawCalls;
		}
		m_GLayer = m_GLayers.begin();
	}
	// --==</core_methods>==--
}

/// OnDraw (AShape)
	/// Description:
	/// -- Shapes have a vertex data, indices and bound AGMaterial
	/// -- GEngine collects all the Vertex and Index data into some arrays from given object
	/// -- AGMaterial of the object provides GEngine's AGMaterial with textures
	/// -- Every new texture from new AGMaterial will be bound to the current GMtl
	/// -- In the EndDraw it will bind all owned textures and will set their samplerIDs to the own shader
	/// -- There is also default white texture which will be used if we don not have other textures