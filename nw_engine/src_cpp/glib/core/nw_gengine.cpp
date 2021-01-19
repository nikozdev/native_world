#include <nw_pch.hpp>
#include "glib/core/nw_gengine.h"

#include <glib/core/nw_gapi.h>

#include <glib/gcontext/nw_window.h>
#include <glib/gcontext/nw_framebuf.h>

#include <glib/vision/nw_gcamera.h>
#include <glib/nw_texture.h>
#include <glib/vision/nw_shader.h>
#include <glib/vision/nw_gmaterial.h>

#include <nwlib/nw_event.h>

#include <sys/nw_data_sys.h>
#include <sys/nw_mem_sys.h>

namespace NW
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
	void GEngine::RmvLayer(const char* strName) {
		Layers::iterator& itDest = std::find_if(m_GLayers.begin(), m_GLayers.end(),
			[=](GLayer& rObj)->bool {return rObj.strName == strName; });
		if (itDest != m_GLayers.end()) { return; }
		m_GLayers.erase(itDest);
	}
	// --==<core_methods>==--
	bool GEngine::Init(WApiTypes WindowApiType, GApiTypes GraphicsApiType)
	{
		m_bIsRunning = true;

		WindowInfo WindowInfo{ "graphics_engine", 1200, 1200 / 4 * 3, true, nullptr };
		m_pWindow.reset(AWindow::Create(WindowInfo, WindowApiType));
		if (!m_pWindow->Init()) { m_pWindow->OnQuit(); return false; }

		m_pGApi.reset(AGApi::Create(GraphicsApiType));
		m_pGApi->SetClearColor(0.1f, 0.3f, 0.3f);

		if (true) {
			ImageInfo imgInfo;
			if (DataSys::LoadF_image("D:/dev/native_world/nw_engine/data/graphics/ico/nw_bat.png", &imgInfo)) {
				m_pWindow->SetIcon(imgInfo.ClrData, imgInfo.nWidth, imgInfo.nHeight);
				MemSys::DelTArr<UByte>(imgInfo.ClrData, imgInfo.nWidth * imgInfo.nHeight * imgInfo.nChannels);
			}
		}
		AddLayer("gel_default");
		m_GLayer = m_GLayers.begin();
		
		if (!m_bIsRunning) { Quit(); }
		return true;
	}
	void GEngine::Quit()
	{
		if (!m_bIsRunning) { return; }
		m_bIsRunning = false;

		m_GLayers.clear();
		m_pWindow->OnQuit();
	}
	void GEngine::Run() {
		m_thrRun = std::thread([this]()->void { while (m_bIsRunning) { Update(); } });
	}
	void GEngine::Update()
	{
		m_pWindow->Update();

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