#include "nw_pch.hpp"
#include "nw_glayer.h"

#include <glib/core/nw_gapi.h>
#include <glib/core/nw_gengine.h>
#include <glib/nw_gbuffer.h>
#include <glib/gcontext/nw_framebuf.h>
#include <glib/nw_texture.h>
#include <glib/vision/nw_shader.h>
#include <glib/vision/nw_gmaterial.h>

#include <sys/nw_data_sys.h>
#include <sys/nw_mem_sys.h>

namespace NW
{
	GLayer::GLayer(const char* strName) :
		strName(strName)
	{
        FrameBufInfo fbInfo;
        fbInfo.unWidth = xywhViewport.z - xywhViewport.x;
        fbInfo.unHeight = xywhViewport.w - xywhViewport.y;
        fbInfo.unSamples = 1;
		pFrameBuf.reset(AFrameBuf::Create(&("fmb_" + this->strName)[0], fbInfo));

		pVtxBuf.reset(AVertexBuf::Create(2 << 10));
		pIdxBuf.reset(AIndexBuf::Create(2 << 10));
		pShdBuf.reset(AShaderBuf::Create(2 << 10));

		pVtxData = MemSys::NewTArr<UByte>(pVtxBuf->GetDataSize());
		pIdxData = MemSys::NewTArr<UInt32>(pIdxBuf->GetDataSize());
		pShdData = MemSys::NewTArr<UByte>(pShdBuf->GetDataSize());
	}
	GLayer::GLayer(const GLayer& rCpy) : GLayer(&rCpy.strName[0]) { }
	GLayer::~GLayer() {
		MemSys::DelTArr<UByte>(pVtxData, pVtxBuf->GetDataSize());
		MemSys::DelTArr<UInt32>(pIdxData, pIdxBuf->GetDataSize());
		MemSys::DelTArr<UByte>(pShdData, pShdBuf->GetDataSize());
	}

	// --setters
	void GLayer::SetShader(AShader* pShader) {
		this->pShader = pShader;
		if (pShader == nullptr) { return; }
		pShdBuf->SetLayout(pShader->GetShdLayout());
		pVtxBuf->SetLayout(pShader->GetVtxLayout());
	}
	void GLayer::SetViewport(const V4f& xywhViewport) {
		this->xywhViewport = xywhViewport;
		V2f whSize = { xywhViewport.z - xywhViewport.x, xywhViewport.w - xywhViewport.y };
		pFrameBuf->SetSizeWH(whSize.x, whSize.y);
	}
	void GLayer::AddDrawData(const DrawObjectData& rDOData) {
		if (rDOData.pGMtl == nullptr) { return; }
		if (rDOData.pGMtl->GetShader() != pShader) { return; }
		Drawables[rDOData.pGMtl].push_back(rDOData);
	}
	void GLayer::OnDraw(AGApi* pGApi)
	{
		static auto fnDrawCall = [&](GMaterial* pGMtl)->void {
			pGMtl->Enable();
			pShdBuf->SetSubData(szShdData, pShdData);
			pVtxBuf->SetSubData(szVtxData, pVtxData);
			pIdxBuf->SetSubData(szIdxData, pIdxData);
			pShdBuf->Bind();
			pVtxBuf->Bind();
			pIdxBuf->Bind();
			pGApi->DrawIndexed(unIdxData);
			pIdxBuf->Unbind();
			pVtxBuf->Unbind();
			pShdBuf->Unbind();
			pGMtl->Disable();
			unDrawCalls++;
		};
		ResetData();
		Camera.nAspectRatio = (xywhViewport.z - xywhViewport.x) / (xywhViewport.w - xywhViewport.y);

		pGApi->SetViewport(xywhViewport.x, xywhViewport.y, xywhViewport.z, xywhViewport.w);

		if (DConfig.General.GPrimitive != pGApi->GetPrimitiveType()) { pGApi->SetPrimitiveType(DConfig.General.GPrimitive); }
		pGApi->SetPixelSize(DConfig.General.nPixelSize);
		pGApi->SetLineWidth(DConfig.General.nLineWidth);

		if (DConfig.Blending.bEnable) { pGApi->SetBlendFunc(DConfig.Blending.FactorSrc, DConfig.Blending.FactorDest); }
		pGApi->SetModes(DConfig.Blending.bEnable, PM_BLEND);
		if (DConfig.DepthTest.bEnable) {}
		pGApi->SetModes(DConfig.DepthTest.bEnable, PM_DEPTH_TEST);

		DSData.m4Proj = Camera.GetProjMatrix();
		DSData.m4View = Camera.GetViewMatrix();
		if (!UploadShdData(DSData)) { return; }

		pFrameBuf->Bind();
		pFrameBuf->Clear(FB_COLOR | FB_DEPTH | FB_STENCIL);
		for (auto& itDrbs : Drawables) {
			std::sort(itDrbs.second.begin(), itDrbs.second.end());
			for (auto& itDrb : itDrbs.second) { if (!UploadVtxData(itDrb)) { fnDrawCall(itDrbs.first); } }
			fnDrawCall(itDrbs.first);
		}
		pFrameBuf->Unbind();
		Drawables.clear();
	}
	// --core_methods
	bool GLayer::UploadVtxData(const DrawObjectData& rDOData)
	{
		Size szVtx = rDOData.GetVtxSize();
		Size szIdx = rDOData.GetIdxSize();
		UInt32 unVtx = szVtx / pVtxBuf->GetLayout().GetStride();
		UInt32 unIdx = rDOData.GetIdxCount();
		if (szVtxData + szVtx > pVtxBuf->GetDataSize() ||
			szIdxData + szIdx > pIdxBuf->GetDataSize()) { return false; }
		// vtx_data:
		memcpy(pVtxIter, rDOData.GetVtxData(), szVtx);
		pVtxIter += szVtx;
		// idx_data:
		for (UInt32 idi = 0; idi < unIdx; idi++) { *pIdxIter++ = rDOData.idxData[idi] + unVtxData; }
		
		szVtxData += szVtx;
		unVtxData += unVtx;
		szIdxData += szIdx;
		unIdxData += unIdx;
		return true;
	}
	bool GLayer::UploadShdData(const DrawSceneData& rDSData)
	{
		Size szData = rDSData.GetDataSize();
		if (szShdData + szData > pShdBuf->GetDataSize()) { return false; }
		// shd_data:
		memcpy(pShdIter, rDSData.GetData(), szData);
		pShdIter += szData;
		szShdData += szData;
		return true;
	}
}