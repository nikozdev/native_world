#include <nw_pch.hpp>
#include "nw_draw_engine.h"

#include <gl/control/nw_gapi.h>

#include <gl/gcontext/nw_gcontext.h>
#include <gl/gcontext/nw_window.h>
#include <gl/gcontext/nw_framebuf.h>

#include <gl/vision/nw_gcamera.h>
#include <gl/vision/nw_gmaterial.h>
#include <gl/vision/nw_shader.h>

#include <gl/render/nw_texture.h>
#include <gl/render/nw_drawable.h>

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
	static DrawTools s_DTools = { 0 };
	static DrawEngine::States::iterator s_DState;

	AGraphicsApi* DrawEngine::s_pGApi(nullptr);
	DrawEngine::States DrawEngine::s_DStates;
	DrawEngineInfo DrawEngine::s_DInfo = { 0 };
	bool DrawEngine::s_bIsDrawing = false;
}

namespace NW
{
	// --setters
	void DrawEngine::SetMaxVtxSize(Size szMaxVtx) {
		MemSys::DelTArr<UByte>(s_DTools.pVtxData, s_DInfo.szMaxVtx);
		s_DInfo.szMaxVtx = szMaxVtx;
		s_DTools.pVtxData = nullptr;
		s_DTools.pVtxBuf->SetData(szMaxVtx, nullptr);
		if (szMaxVtx > 0) { s_DTools.pVtxData = MemSys::NewTArr<UByte>(szMaxVtx); }
	}
	void DrawEngine::SetMaxIdxSize(Size szMaxIdx) {
		MemSys::DelTArr(s_DTools.pIdxData, s_DInfo.szMaxIdx / sizeof(UInt32));
		s_DTools.pIdxData = nullptr;
		s_DInfo.szMaxIdx = szMaxIdx;
		s_DTools.pIdxBuf->SetData(szMaxIdx, nullptr);
		if (szMaxIdx > 0) { s_DTools.pIdxData = MemSys::NewTArr<UInt32>(szMaxIdx); }
	}
	void DrawEngine::SetMaxShdSize(Size szMaxShd) {
		MemSys::DelTArr(s_DTools.pShdData, s_DInfo.szMaxShd);
		s_DTools.pShdData = nullptr;
		s_DInfo.szMaxShd = szMaxShd;
		s_DTools.pShdBuf->SetData(szMaxShd, nullptr);
		if (szMaxShd > 0) { s_DTools.pShdData = MemSys::NewTArr<UByte>(szMaxShd); }
	}
	void DrawEngine::SetMaxTexCount(UInt8 unMaxTex) {
		if (unMaxTex > NW_MAX_TEXTURES || unMaxTex < NW_MIN_TEXTURES) { NW_ERR("We can not set this amount of slots"); return; }
		s_DInfo.unMaxTex = unMaxTex;
	}

	// --==<core_methods>==--
	bool DrawEngine::Init(GApiTypes GraphicsApiType)
	{
		s_pGApi = AGraphicsApi::Create(GraphicsApiType);

		s_DTools.pVtxBuf = AVertexBuf::Create(0);
		s_DTools.pIdxBuf = AIndexBuf::Create(0);
		s_DTools.pShdBuf = AShaderBuf::Create(0);

		SetMaxVtxSize(2 << 8);
		SetMaxIdxSize(s_DInfo.szMaxVtx << 2);
		SetMaxShdSize(2 << 8);
		SetMaxTexCount(NW_MAX_TEXTURES);
		
		s_DTools.ResetData();

		s_pGApi->SetClearColor(Pixel{ 64, 64, 96 });

		return true;
	}
	void DrawEngine::OnQuit()
	{
		SetMaxVtxSize(0);
		SetMaxIdxSize(0);
		SetMaxTexCount(1);
		
		if (s_DTools.pVtxBuf != nullptr) { MemSys::DelT<AVertexBuf>(s_DTools.pVtxBuf); s_DTools.pVtxBuf = nullptr; }
		if (s_DTools.pIdxBuf != nullptr) { MemSys::DelT<AIndexBuf>(s_DTools.pIdxBuf); s_DTools.pIdxBuf = nullptr; }
		if (s_DTools.pShdBuf != nullptr) { MemSys::DelT<AShaderBuf>(s_DTools.pShdBuf); s_DTools.pShdBuf = nullptr; }
		
		s_DStates.clear();

		MemSys::DelT<AGraphicsApi>(s_pGApi);
	}
	void DrawEngine::Update()
	{
		s_DInfo.Reset();
	}
	// --==<Drawing>==--
	void DrawEngine::BeginDraw() {
		bool bCanDraw = !s_bIsDrawing;
		if (!bCanDraw) { NW_ERR("Can not Draw now"); return; }
		s_bIsDrawing = true;
	}
	void DrawEngine::OnDraw(ADrawable* pDrb, const char* strStateName)
	{
		bool bCanDraw = s_bIsDrawing && pDrb != nullptr;
		if (!bCanDraw) { NW_ERR("Can not Draw now"); return; }

		GetState(strStateName).AddDrawable(pDrb);
	}
	void DrawEngine::EndDraw()
	{
		bool bCanDraw = s_bIsDrawing;
		if (!bCanDraw) { NW_ERR("Can not Draw now"); return; }
		
		DArray<DrawState*> DStatesOrder;
		DStatesOrder.reserve(16);

		for (auto& itDState : s_DStates) {
			DStatesOrder.push_back(&itDState.second);
		}
		std::sort(DStatesOrder.begin(), DStatesOrder.end());

		for (auto itDState : DStatesOrder) {
			const V4f& xywhViewport = itDState->xywhViewport;

			itDState->pGCamera->nAspectRatio = (xywhViewport.z - xywhViewport.x) / (xywhViewport.w - xywhViewport.y);
			
			s_pGApi->SetPrimitiveType(itDState->DPrimitive);
			DrawEngine::GetGApi()->SetViewport(xywhViewport.x, xywhViewport.y, xywhViewport.z, xywhViewport.w);
			
			DrawSceneData DSData;
			DSData.m4Proj = itDState->pGCamera->GetProjMatrix();
			DSData.m4View = itDState->pGCamera->GetViewMatrix();
			UploadShdData(&DSData);

			itDState->pFrameBuf->Bind();
			itDState->pFrameBuf->Clear(FB_COLOR | FB_DEPTH | FB_STENCIL);

			for (auto& itDData : itDState->m_DData) {
				s_DTools.pGMtl = itDData.first;
				std::sort(itDData.second.begin(), itDData.second.end());
				for (auto& pDrb : itDData.second) { UploadVtxData(pDrb); }
				DrawCall();
			}
			itDState->pFrameBuf->Unbind();
			itDState->Reset();
			DStatesOrder.pop_back();

			s_DTools.ResetData();
		}

		s_bIsDrawing = false;
	}

	inline void DrawEngine::UploadVtxData(ADrawable* pDrawable)
	{
		UInt32 szVtx = pDrawable->GetVDataSize();
		UInt32 unVtx = pDrawable->GetVDataCount();
		UInt32 szIdx = pDrawable->GetIDataSize();
		UInt32 unIdx = pDrawable->GetIDataCount();
		const UByte* pVData = reinterpret_cast<const UByte*>(pDrawable->GetVData());
		const UInt32* pIData = reinterpret_cast<const UInt32*>(pDrawable->GetIData());

		if (s_DTools.szVtxData + szVtx > s_DInfo.szMaxVtx ||
			s_DTools.szIdxData + szIdx > s_DInfo.szMaxIdx /*||
			s_DTools.unTexCount >= NW_MAX_TEXTURES*/) { DrawCall(); }

	//TextureData:
	VertexData:
		pDrawable->UpdateVData();
		memcpy(s_DTools.pVtxIter, pVData, szVtx);
		s_DTools.pVtxIter += szVtx;
	IndexData:
		pDrawable->UpdateIData();
		memcpy(s_DTools.pIdxIter, pIData, szIdx);
		s_DTools.pIdxIter += szIdx;
	UpdateInfo:
		s_DTools.szVtxData += szVtx;
		s_DTools.unVtxData += unVtx;
		s_DTools.szIdxData += szIdx;
		s_DTools.unIdxData += unIdx;
	}
	inline void DrawEngine::UploadShdData(DrawSceneData* pDSData)
	{
		Size szShd = pDSData->GetDataSize();
		const Byte* pShd = reinterpret_cast<const Byte*>(pDSData->GetData());

		if (s_DTools.szShdData + szShd > s_DInfo.szMaxShd) { DrawCall(); }
		
		memcpy(s_DTools.pShdData, pShd, szShd);

		s_DTools.szShdData += szShd;
	}
	inline void DrawEngine::DrawCall()
	{
		s_DTools.pGMtl->Enable();

		s_DTools.pShdBuf->SetSubData(s_DTools.szShdData, &s_DTools.pShdData[0]);
		s_DTools.pVtxBuf->SetSubData(s_DTools.szVtxData, &s_DTools.pVtxData[0]);
		s_DTools.pIdxBuf->SetSubData(s_DTools.szIdxData, &s_DTools.pIdxData[0]);

		s_DTools.pShdBuf->SetLayout(s_DTools.pGMtl->GetShader()->GetShdLayout());
		s_DTools.pVtxBuf->SetLayout(s_DTools.pGMtl->GetShader()->GetVertexLayout());
		s_DTools.pVtxBuf->Bind();
		s_DTools.pIdxBuf->Bind();
		s_pGApi->DrawIndexed(s_DTools.szIdxData / sizeof(UInt32));
		s_DTools.pIdxBuf->Unbind();
		s_DTools.pVtxBuf->Unbind();
		s_DTools.pShdBuf->Unbind();

		s_DTools.pGMtl->Disable();

		s_DInfo.unVtx = s_DInfo.unVtx + s_DTools.unVtxData;
		s_DInfo.unIdx = s_DInfo.unIdx + s_DTools.unIdxData;
		if (s_DInfo.unTex < s_DTools.pGMtl->GetTexCount()) { s_DInfo.unTex = s_DTools.pGMtl->GetTexCount(); }
		s_DInfo.unDrawCalls++;
		s_DTools.ResetData();
	}
	// --==<Drawing>==--
	// --==</core_methods>==--
}

/// OnDraw (AShape)
	/// Description:
	/// -- Shapes have a vertex data, indices and bound AGMaterial
	/// -- DrawEngine collects all the Vertex and Index data into some arrays from given object
	/// -- AGMaterial of the object provides DrawEngine's AGMaterial with textures
	/// -- Every new texture from new AGMaterial will be bound to the current GMtl
	/// -- In the EndDraw it will bind all owned textures and will set their samplerIDs to the own shader
	/// -- There is also default white texture which will be used if we don not have other textures