#include <nw_pch.hpp>
#include "nw_graph_engine.h"

#include <glib/control/nw_graph_api.h>

#include <glib/gcontext/nw_gcontext.h>
#include <glib/gcontext/nw_window.h>
#include <glib/gcontext/nw_framebuf.h>

#include <glib/vision/nw_gcamera.h>
#include <glib/vision/nw_gmaterial.h>
#include <glib/vision/nw_shader.h>

#include <glib/render/nw_texture.h>
#include <glib/render/nw_drawable.h>

#include <sys/nw_mem_sys.h>

namespace NW
{
	struct NW_API DrawTools {
	public:
		// --vertex_data
		UByte* pVtxData = nullptr;
		UByte* pVtxIter = nullptr;
		UInt32 unVtxData = 0;
		Size szVtxData = 0;
		// --index_data
		UInt32* pIdxIter = nullptr;
		UInt32* pIdxData = nullptr;
		UInt32 unIdxData = 0;
		Size szIdxData = 0;
		// --shader_data
		UByte* pShdData = nullptr;
		UByte* pShdIter = nullptr;
		Size szShdData = 0;
		// --texture_data
		ATexture* pTextures[NW_MAX_TEXTURES];
		UInt8 unTexCount = 0;
		// --objects
		GMaterial* pGMtl = nullptr;
		AVertexBuf* pVtxBuf = nullptr;
		AIndexBuf* pIdxBuf = nullptr;
		AShaderBuf* pShdBuf = nullptr;
	public:
		inline void ResetData() {
			pVtxIter = &pVtxData[0];
			szVtxData = unVtxData = 0;
			pIdxIter = &pIdxData[0];
			szIdxData = unIdxData = 0;
			pShdIter = &pShdData[0];
			szShdData = 0;
			for (UInt8 txi = 0; txi < unTexCount; txi++) { pTextures[txi] = nullptr; }
			unTexCount = 0;
		}
	};
	static DrawTools m_DTools = { 0 };
}

namespace NW
{
	GraphEngine::GraphEngine() :
		m_bIsRunning(false), m_State(m_States.begin()),
		m_pGApi(nullptr),
		m_DInfo(GraphEngineInfo()) { }
	GraphEngine::~GraphEngine() {}

	// --setters
	void GraphEngine::SetMaxVtxSize(Size szMaxVtx) {
		MemSys::DelTArr<UByte>(m_DTools.pVtxData, m_DInfo.szMaxVtx);
		m_DInfo.szMaxVtx = szMaxVtx;
		m_DTools.pVtxData = nullptr;
		m_DTools.pVtxBuf->SetData(szMaxVtx, nullptr);
		if (szMaxVtx > 0) { m_DTools.pVtxData = MemSys::NewTArr<UByte>(szMaxVtx); }
	}
	void GraphEngine::SetMaxIdxSize(Size szMaxIdx) {
		MemSys::DelTArr(m_DTools.pIdxData, m_DInfo.szMaxIdx / sizeof(UInt32));
		m_DTools.pIdxData = nullptr;
		m_DInfo.szMaxIdx = szMaxIdx;
		m_DTools.pIdxBuf->SetData(szMaxIdx, nullptr);
		if (szMaxIdx > 0) { m_DTools.pIdxData = MemSys::NewTArr<UInt32>(szMaxIdx); }
	}
	void GraphEngine::SetMaxShdSize(Size szMaxShd) {
		MemSys::DelTArr(m_DTools.pShdData, m_DInfo.szMaxShd);
		m_DTools.pShdData = nullptr;
		m_DInfo.szMaxShd = szMaxShd;
		m_DTools.pShdBuf->SetData(szMaxShd, nullptr);
		if (szMaxShd > 0) { m_DTools.pShdData = MemSys::NewTArr<UByte>(szMaxShd); }
	}
	void GraphEngine::SetMaxTexCount(UInt8 unMaxTex) {
		if (unMaxTex > NW_MAX_TEXTURES || unMaxTex < NW_MIN_TEXTURES) { NW_ERR("We can not set this amount of slots"); return; }
		m_DInfo.unMaxTex = unMaxTex;
	}

	// --==<core_methods>==--
	bool GraphEngine::Init(GApiTypes GraphicsApiType)
	{
		m_bIsRunning = true;

		m_pGApi = AGraphApi::Create(GraphicsApiType);

		m_DTools.pVtxBuf = AVertexBuf::Create(0);
		m_DTools.pIdxBuf = AIndexBuf::Create(0);
		m_DTools.pShdBuf = AShaderBuf::Create(0);

		SetMaxVtxSize(2 << 8);
		SetMaxIdxSize(m_DInfo.szMaxVtx << 2);
		SetMaxShdSize(2 << 8);
		SetMaxTexCount(NW_MAX_TEXTURES);

		m_pGApi->SetClearColor(Pixel{ 64, 64, 96 });

		//if (m_States.empty()) { AddState(MemSys::NewT<GraphState>("ges_default")); }
		if (!m_bIsRunning) { OnQuit(); }

		return m_bIsRunning;
	}
	void GraphEngine::OnQuit()
	{
		m_bIsRunning = false;

		while (!m_States.empty()) {
			MemSys::DelT<GraphState>(m_States.begin()->second);
			m_States.erase(m_States.begin());
		}

		SetMaxVtxSize(0);
		SetMaxIdxSize(0);
		SetMaxShdSize(0);
		SetMaxTexCount(1);
		
		if (m_DTools.pVtxBuf != nullptr) { MemSys::DelT<AVertexBuf>(m_DTools.pVtxBuf); m_DTools.pVtxBuf = nullptr; }
		if (m_DTools.pIdxBuf != nullptr) { MemSys::DelT<AIndexBuf>(m_DTools.pIdxBuf); m_DTools.pIdxBuf = nullptr; }
		if (m_DTools.pShdBuf != nullptr) { MemSys::DelT<AShaderBuf>(m_DTools.pShdBuf); m_DTools.pShdBuf = nullptr; }
		
		m_States.clear();

		MemSys::DelT<AGraphApi>(m_pGApi);
	}
	void GraphEngine::Update()
	{
		if (!m_bIsRunning) { return; }
		m_DInfo.Reset();
		m_DTools.ResetData();

		DArray<GraphState*> StatesOrder;
		StatesOrder.reserve(16);

		for (auto& itState : m_States) {
			StatesOrder.push_back(itState.second);
		}
		std::sort(StatesOrder.begin(), StatesOrder.end());

		for (auto& itDState : StatesOrder) {
			const V4f& xywhViewport = itDState->xywhViewport;

			itDState->pGCamera->nAspectRatio = (xywhViewport.z - xywhViewport.x) / (xywhViewport.w - xywhViewport.y);

			GraphEngine::Get().GetGApi()->SetViewport(xywhViewport.x, xywhViewport.y, xywhViewport.z, xywhViewport.w);

			if (itDState->Config.DPrimitive != m_pGApi->GetPrimitiveType()) { m_pGApi->SetPrimitiveType(itDState->Config.DPrimitive); }
			m_pGApi->SetModes(itDState->Blending.bEnable, PM_BLEND);
			if (itDState->Blending.bEnable) { m_pGApi->SetBlendFunc(itDState->Blending.FactorSrc, itDState->Blending.FactorDest); }
			if (itDState->DepthTest.bEnable) { }
			m_pGApi->SetModes(itDState->DepthTest.bEnable, PM_DEPTH_TEST);
			
			m_pGApi->SetPixelSize(itDState->Config.nPixelSize);
			m_pGApi->SetLineWidth(itDState->Config.nLineWidth);

			DrawSceneData DSData;
			DSData.m4Proj = itDState->pGCamera->GetProjMatrix();
			DSData.m4View = itDState->pGCamera->GetViewMatrix();
			UploadSceneData(&DSData);

			itDState->pFrameBuf->Bind();
			itDState->pFrameBuf->Clear(FB_COLOR | FB_DEPTH | FB_STENCIL);

			for (auto& itDData : itDState->m_DData) {
				m_DTools.pGMtl = itDData.first;
				std::sort(itDData.second.begin(), itDData.second.end());
				for (auto& pDrb : itDData.second) { UploadDrawData(pDrb); }
				DrawCall();
			}
			itDState->pFrameBuf->Unbind();
			itDState->Reset();
			StatesOrder.pop_back();

			m_DTools.ResetData();
		}
	}
	// --==</core_methods>==--

	// --==<drawing>==--
	inline void GraphEngine::UploadDrawData(ADrawable* pDrawable)
	{
		UInt32 szVtx = pDrawable->GetVDataSize();
		UInt32 unVtx = pDrawable->GetVDataCount();
		UInt32 szIdx = pDrawable->GetIDataSize();
		UInt32 unIdx = pDrawable->GetIDataCount();
		const UByte* pVData = reinterpret_cast<const UByte*>(pDrawable->GetVData());
		const UInt32* pIData = reinterpret_cast<const UInt32*>(pDrawable->GetIData());

		if (m_DTools.szVtxData + szVtx > m_DInfo.szMaxVtx ||
			m_DTools.szIdxData + szIdx > m_DInfo.szMaxIdx) { DrawCall(); }

	//TextureData:
	VertexData:
		pDrawable->UpdateVData();
		memcpy(m_DTools.pVtxIter, pVData, szVtx);
		m_DTools.pVtxIter += szVtx;
	IndexData:
		pDrawable->UpdateIData();
		memcpy(m_DTools.pIdxIter, pIData, szIdx);
		m_DTools.pIdxIter += szIdx;
	UpdateInfo:
		m_DTools.szVtxData += szVtx;
		m_DTools.unVtxData += unVtx;
		m_DTools.szIdxData += szIdx;
		m_DTools.unIdxData += unIdx;
	}
	inline void GraphEngine::UploadSceneData(DrawSceneData* pDSData)
	{
		Size szShd = pDSData->GetDataSize();
		const Byte* pShd = reinterpret_cast<const Byte*>(pDSData->GetData());

		if (m_DTools.szShdData + szShd > m_DInfo.szMaxShd) { DrawCall(); }
	ShaderData:
		memcpy(m_DTools.pShdData, pShd, szShd);
	UpdateInfo:
		m_DTools.szShdData += szShd;
	}
	inline void GraphEngine::DrawCall()
	{
		m_DTools.pGMtl->Enable();

		m_DTools.pShdBuf->SetSubData(m_DTools.szShdData, &m_DTools.pShdData[0]);
		m_DTools.pVtxBuf->SetSubData(m_DTools.szVtxData, &m_DTools.pVtxData[0]);
		m_DTools.pIdxBuf->SetSubData(m_DTools.szIdxData, &m_DTools.pIdxData[0]);

		m_DTools.pShdBuf->SetLayout(m_DTools.pGMtl->GetShader()->GetShdLayout());
		m_DTools.pVtxBuf->SetLayout(m_DTools.pGMtl->GetShader()->GetVertexLayout());
		m_DTools.pVtxBuf->Bind();
		m_DTools.pIdxBuf->Bind();
		m_pGApi->DrawIndexed(m_DTools.szIdxData / sizeof(UInt32));
		m_DTools.pIdxBuf->Unbind();
		m_DTools.pVtxBuf->Unbind();
		m_DTools.pShdBuf->Unbind();

		m_DTools.pGMtl->Disable();

		m_DInfo.unVtx = m_DInfo.unVtx + m_DTools.unVtxData;
		m_DInfo.unIdx = m_DInfo.unIdx + m_DTools.unIdxData;
		if (m_DInfo.unTex < m_DTools.pGMtl->GetTexCount()) { m_DInfo.unTex = m_DTools.pGMtl->GetTexCount(); }
		m_DInfo.unDrawCalls++;
		m_DTools.ResetData();
	}
	// --==<drawing>==--
}

/// OnDraw (AShape)
	/// Description:
	/// -- Shapes have a vertex data, indices and bound AGMaterial
	/// -- GraphEngine collects all the Vertex and Index data into some arrays from given object
	/// -- AGMaterial of the object provides GraphEngine's AGMaterial with textures
	/// -- Every new texture from new AGMaterial will be bound to the current GMtl
	/// -- In the EndDraw it will bind all owned textures and will set their samplerIDs to the own shader
	/// -- There is also default white texture which will be used if we don not have other textures