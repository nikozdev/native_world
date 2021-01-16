#include "nw_pch.hpp"
#include "nw_glayer.h"

#include <glib/core/nw_gengine.h>
#include <glib/nw_gbuffer.h>
#include <glib/gcontext/nw_framebuf.h>
#include <glib/vision/nw_shader.h>
#include <glib/vision/nw_gmaterial.h>

#include <sys/nw_data_sys.h>
#include <sys/nw_mem_sys.h>

namespace NW
{
	GLayer::GLayer(const char* strName) :
		m_strName(strName),
		m_xywhViewport{ 0, 0, 800, 600 },
		m_GCamera(GCamera()), m_pFrameBuf(nullptr),
		m_DTools(DrawTools())
	{
        FrameBufInfo fbInfo;
        fbInfo.unWidth = m_xywhViewport.z - m_xywhViewport.x;
        fbInfo.unHeight = m_xywhViewport.w - m_xywhViewport.y;
        fbInfo.unSamples = 1;
		m_pFrameBuf = AFrameBuf::Create(&("fmb_" + m_strName)[0], fbInfo);
		
		m_DTools.pVtxBuf = AVertexBuf::Create(2 << 10);
		m_DTools.pIdxBuf = AIndexBuf::Create(2 << 10);
		m_DTools.pShdBuf = AShaderBuf::Create(2 << 10);

		m_DTools.pVtxData = MemSys::NewTArr<UByte>(m_DTools.pVtxBuf->GetDataSize());
		m_DTools.pIdxData = MemSys::NewTArr<UInt32>(m_DTools.pIdxBuf->GetDataSize());
		m_DTools.pShdData = MemSys::NewTArr<UByte>(m_DTools.pShdBuf->GetDataSize());
	}
	GLayer::GLayer(const GLayer& rCpy) : GLayer(&rCpy.m_strName[0]) { }
	GLayer::~GLayer() {
		MemSys::DelTArr<UByte>(m_DTools.pVtxData, m_DTools.pVtxBuf->GetDataSize());
		MemSys::DelTArr<UInt32>(m_DTools.pIdxData, m_DTools.pIdxBuf->GetDataSize());
		MemSys::DelTArr<UByte>(m_DTools.pShdData, m_DTools.pShdBuf->GetDataSize());

		MemSys::DelT<AFrameBuf>(m_pFrameBuf);
		MemSys::DelT<AVertexBuf>(m_DTools.pVtxBuf);
		MemSys::DelT<AIndexBuf>(m_DTools.pIdxBuf);
		MemSys::DelT<AShaderBuf>(m_DTools.pShdBuf);
	}

	// --setters
	void GLayer::SetViewport(const V4f& xywhViewport) {
		m_xywhViewport = xywhViewport;
		V2f whSize = { xywhViewport.z - xywhViewport.x, xywhViewport.w - xywhViewport.y };
		m_pFrameBuf->SetSizeWH(whSize.x, whSize.y);
	}
	void GLayer::AddDrawData(const DrawObjectData& rDOData) { m_Drawables[rDOData.pDrawable->pGMtl].push_back(rDOData); }
	void GLayer::RmvDrawData(UInt32 unId) {
		Drawables::iterator itDOData = m_Drawables.begin();
		while (itDOData != m_Drawables.end()) {
			auto& itDrb = std::find_if(itDOData->second.begin(), itDOData->second.end(),
				[=](DrawObjectData& rDOData)->bool {return rDOData.unId == unId; });
			if (itDrb == itDOData->second.end()) { return; }
			itDOData->second.erase(itDrb);
		}
	}
	// --core_methods
	void GLayer::Update()
	{
		m_DTools.ResetData();
		m_GCamera.nAspectRatio = (m_xywhViewport.z - m_xywhViewport.x) / (m_xywhViewport.w - m_xywhViewport.y);

		AGApi* pGApi = GEngine::Get().GetGApi();
		pGApi->SetViewport(m_xywhViewport.x, m_xywhViewport.y, m_xywhViewport.z, m_xywhViewport.w);

		if (DConfig.General.GPrimitive != pGApi->GetPrimitiveType()) { pGApi->SetPrimitiveType(DConfig.General.GPrimitive); }
		pGApi->SetPixelSize(DConfig.General.nPixelSize);
		pGApi->SetLineWidth(DConfig.General.nLineWidth);

		if (DConfig.Blending.bEnable) { pGApi->SetBlendFunc(DConfig.Blending.FactorSrc, DConfig.Blending.FactorDest); }
		pGApi->SetModes(DConfig.Blending.bEnable, PM_BLEND);
		if (DConfig.DepthTest.bEnable) {}
		pGApi->SetModes(DConfig.DepthTest.bEnable, PM_DEPTH_TEST);


		DrawSceneData DSData;
		DSData.m4Proj = m_GCamera.GetProjMatrix();
		DSData.m4View = m_GCamera.GetViewMatrix();
		UploadShaderData(DSData.GetData(), DSData.GetDataSize());

		m_pFrameBuf->Bind();
		m_pFrameBuf->Clear(FB_COLOR | FB_DEPTH | FB_STENCIL);
		for (auto& itDrbs : m_Drawables) {
			m_DTools.pGMtl = itDrbs.first;
			std::sort(itDrbs.second.begin(), itDrbs.second.end());
			for (auto& itDrb : itDrbs.second) {
				itDrb.pDrawable->OnDraw();
			}
			GEngine::Get().DrawCall(m_DTools);
			m_DTools.ResetData();
		}
		m_pFrameBuf->Unbind();
	}

	void GLayer::UploadObjectData(const void* pVtx, Size szVtx, const UInt32* pIdx, Size szIdx)
	{
		if (m_DTools.szVtxData + szVtx > m_DTools.pVtxBuf->GetDataSize() ||
			m_DTools.szIdxData + szIdx > m_DTools.pIdxBuf->GetDataSize()) { return; }

		// vtx_data:
		memcpy(m_DTools.pVtxIter, pVtx, szVtx);
		m_DTools.pVtxIter += szVtx;
		m_DTools.szVtxData += szVtx;
		// idx_data:
		memcpy(m_DTools.pIdxIter, pIdx, szIdx);
		m_DTools.pIdxIter += szIdx;
		m_DTools.szIdxData += szIdx;
	}
	void GLayer::UploadShaderData(const void* pShd, Size szShd)
	{
		if (m_DTools.szShdData + szShd > m_DTools.pShdBuf->GetDataSize()) { return; }
		// shd_data
		memcpy(m_DTools.pShdData, pShd, szShd);
		m_DTools.pShdIter += szShd;
		m_DTools.szShdData += szShd;
	}
}