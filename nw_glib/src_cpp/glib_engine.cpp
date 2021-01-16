#include <glib_pch.hpp>
#include "glib_engine.h"

#include <glib_api.h>

#include <glib_window.h>
#include <glib_framebuf.h>

#include <glib_camera.h>
#include <glib_shader.h>
#include <glib_material.h>

#include <glib_texture.h>

namespace GLIB
{
	GEngine::GEngine() :
		m_bIsRunning(false),
		m_pGApi(nullptr), m_pWindow(nullptr),
		m_DInfo(GEngineInfo()) { }
	GEngine::~GEngine() {}

	// --setters
	GLayer* GEngine::AddLayer(const char* strName) {
		if (GetLayer(strName) != nullptr) { return nullptr; }
		m_GLayers.push_back(GLayer(strName));
		return &m_GLayers.back();
	}
	void GEngine::RmvLayer(const char* strName){
		Layers::iterator& itDest = FIND_BY_NAME(m_GLayers, GLayer&, strName, .GetName);
		if (itDest != m_GLayers.end()) { return; }
		m_GLayers.erase(itDest);
	}
	void GEngine::ChangeLayerOrder(const char* strName, bool bPushUp) {
		Layers::iterator itLayer = std::find_if(m_GLayers.begin(), m_GLayers.end(),
			[=](GLayer& rObj)->bool {return strcmp(&rObj.GetName()[0], strName) == 0; });
		Layers::iterator itLayerChanged = itLayer;
		if (bPushUp) { std::swap(itLayerChanged, ++itLayer); }
		else { std::swap(itLayerChanged, --itLayer); }
	}
	// --==<core_methods>==--
	bool GEngine::Init(WApiTypes WindowApiType, GApiTypes GraphicsApiType)
	{
		m_bIsRunning = true;

		WindowInfo WindowInfo{ "default", 1200, 1200 / 4 * 3, true, nullptr };
		m_pWindow = AWindow::Create(WindowInfo, WindowApiType);
		if (!m_pWindow->Init()) { m_pWindow->OnQuit(); return false; }
		
		m_pGApi = AGraphApi::Create(GraphicsApiType);
		m_pGApi->SetClearColor(0.1f, 0.2f, 0.3f);

		if (!m_bIsRunning) { Quit(); }

		AddLayer("del_default");
		AddLayer("del_lines");
		m_GLayer = m_GLayers.begin();

		return m_bIsRunning;
	}
	void GEngine::Quit()
	{
		if (!m_bIsRunning) { return; }
		m_bIsRunning = false;
		
		m_GLayers.clear();
		delete m_pGApi;
		m_pWindow->OnQuit();
		delete m_pWindow;
	}
	void GEngine::Update()
	{
		m_pWindow->Update();

		m_DInfo.Reset();
		
		for (m_GLayer = m_GLayers.begin(); m_GLayer != m_GLayers.end(); m_GLayer++) {
			m_GLayer->Update();
		}
		m_GLayer = m_GLayers.begin();
	}
	// --==</core_methods>==--

	// --==<drawing>==--
	void GEngine::DrawCall(DrawTools& rDTools)
	{
		rDTools.pGMtl->Enable();

		rDTools.pShdBuf->SetSubData(rDTools.szShdData, &rDTools.pShdData[0]);
		rDTools.pVtxBuf->SetSubData(rDTools.szVtxData, &rDTools.pVtxData[0]);
		rDTools.pIdxBuf->SetSubData(rDTools.szIdxData, &rDTools.pIdxData[0]);

		rDTools.pShdBuf->SetLayout(rDTools.pGMtl->GetShader()->GetShdLayout());
		rDTools.pVtxBuf->SetLayout(rDTools.pGMtl->GetShader()->GetVertexLayout());
		rDTools.pVtxBuf->Bind();
		rDTools.pIdxBuf->Bind();
		m_pGApi->DrawIndexed(rDTools.szIdxData / sizeof(UInt32));
		rDTools.pIdxBuf->Unbind();
		rDTools.pVtxBuf->Unbind();
		rDTools.pShdBuf->Unbind();

		rDTools.pGMtl->Disable();

		m_DInfo.szShd = m_DInfo.szShd + rDTools.szShdData;
		m_DInfo.szVtx = m_DInfo.szVtx + rDTools.szVtxData;
		m_DInfo.szIdx = m_DInfo.szIdx + rDTools.szIdxData;
		if (m_DInfo.unTex < rDTools.unTexCount) { m_DInfo.unTex = rDTools.unTexCount; }
		m_DInfo.unDrawCalls++;
	}
	// --==<drawing>==--
}

/// OnDraw (AShape)
	/// Description:
	/// -- Shapes have a vertex data, indices and bound AGMaterial
	/// -- GEngine collects all the Vertex and Index data into some arrays from given object
	/// -- AGMaterial of the object provides GEngine's AGMaterial with textures
	/// -- Every new texture from new AGMaterial will be bound to the current GMtl
	/// -- In the EndDraw it will bind all owned textures and will set their samplerIDs to the own shader
	/// -- There is also default white texture which will be used if we don not have other textures