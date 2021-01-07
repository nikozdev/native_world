#include <nw_pch.hpp>
#include "nw_graph_engine.h"

#include <gl/control/nw_gapi.h>
#include <gl/gcontext/nw_gcontext.h>
#include <gl/gcontext/nw_window.h>
#include <gl/gcontext/nw_framebuf.h>
#include <gl/vision/nw_gcamera.h>
#include <gl/vision/nw_gmaterial.h>
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
		}
	};
	static DrawSceneData s_DSData;
	static HashMap<GMaterial*, DArray<DrawObjectData>> s_DOData;
	static DArray<DrawObjectData>::iterator s_DODataIter;
	static DrawTools s_DTools = { 0 };

	RefOwner<AGraphicsApi> GraphEngine::s_pGApi(nullptr);
	GraphEngineInfo GraphEngine::s_DInfo = { 0 };
	bool GraphEngine::s_bIsDrawing = false;
}

namespace NW
{
	// --setters
	void GraphEngine::SetMaxVtxSize(Size szMaxVtx) {
		MemSys::DelTArr<UByte>(s_DTools.pVtxData, s_DInfo.szMaxVtx);
		s_DInfo.szMaxVtx = szMaxVtx;
		s_DTools.pVtxData = nullptr;
		if (szMaxVtx > 0) { s_DTools.pVtxData = MemSys::NewTArr<UByte>(szMaxVtx); }
	}
	void GraphEngine::SetMaxIdxSize(Size szMaxIdx) {
		MemSys::DelTArr(s_DTools.pIdxData, s_DInfo.szMaxIdx / sizeof(UInt32));
		s_DTools.pIdxData = nullptr;
		s_DInfo.szMaxIdx = szMaxIdx;
		if (szMaxIdx > 0) { s_DTools.pIdxData = MemSys::NewTArr<UInt32>(szMaxIdx); }
	}
	void GraphEngine::SetMaxShdSize(Size szMaxShd) {
		MemSys::DelTArr(s_DTools.pShdData, s_DInfo.szMaxShd);
		s_DTools.pShdData = nullptr;
		s_DInfo.szMaxShd = szMaxShd;
		if (szMaxShd > 0) { s_DTools.pShdData = MemSys::NewTArr<UByte>(szMaxShd); }
	}
	void GraphEngine::SetMaxTexCount(UInt8 unMaxTex) {
		if (unMaxTex > NW_MAX_TEXTURES || unMaxTex < NW_MIN_TEXTURES) { NW_ERR("We can not set this amount of slots"); return; }
		s_DInfo.unMaxTex = unMaxTex;
	}

	// --==<core_methods>==--
	bool GraphEngine::Init(GApiTypes GraphicsApiType)
	{
		s_pGApi.reset(AGraphicsApi::Create(GraphicsApiType));

		s_DTools.pGMtl = MemSys::NewT<GMaterial>("gmt_of_drawer");
		
		s_DTools.pVtxBuf = AVertexBuf::Create(0);
		s_DTools.pIdxBuf = AIndexBuf::Create(0);
		s_DTools.pShdBuf = AShaderBuf::Create(0);

		SetMaxVtxSize(2 << 8);
		SetMaxIdxSize(s_DInfo.szMaxVtx << 2);
		SetMaxShdSize(2 << 10);
		SetMaxTexCount(NW_MAX_TEXTURES);
		
		s_DTools.ResetData();

		s_pGApi->SetClearColor(Pixel{ 64, 64, 96 });

		return true;
	}
	void GraphEngine::OnQuit()
	{
		SetMaxVtxSize(0);
		SetMaxIdxSize(0);
		SetMaxTexCount(1);
		
		if (s_DTools.pVtxBuf != nullptr) { MemSys::DelT<AVertexBuf>(s_DTools.pVtxBuf); s_DTools.pVtxBuf = nullptr; }
		if (s_DTools.pIdxBuf != nullptr) { MemSys::DelT<AIndexBuf>(s_DTools.pIdxBuf); s_DTools.pIdxBuf = nullptr; }
		if (s_DTools.pShdBuf != nullptr) { MemSys::DelT<AShaderBuf>(s_DTools.pShdBuf); s_DTools.pShdBuf = nullptr; }
		
		s_pGApi.reset(nullptr);
	}
	void GraphEngine::Update()
	{
		s_DInfo.Reset();
	}
	// --==<Drawing>==--
	void GraphEngine::BeginDraw(const DrawSceneData& rDSData)
	{
		bool bCanDraw = !s_bIsDrawing;
		if (!bCanDraw) { NW_ERR("Can not Draw now"); return; }
		s_bIsDrawing = true;

		s_DSData = rDSData;
	}
	void GraphEngine::OnDraw(const DrawObjectData& rDOData)
	{
		bool bCanDraw = s_bIsDrawing && rDOData.pDrawable != nullptr;
		if (!bCanDraw) { NW_ERR("Can not Draw now"); return; }

		s_DOData[rDOData.pDrawable->pGMtl].push_back(rDOData);
	}
	void GraphEngine::EndDraw()
	{
		bool bCanDraw = s_bIsDrawing;
		if (!bCanDraw) { NW_ERR("Can not Draw now"); return; }
		
		Byte* pData = nullptr;
		Mat4f m4Cam = s_DSData.pGCamera->GetProjMatrix();
		memcpy(s_DTools.pShdIter, reinterpret_cast<Byte*>(&m4Cam), sizeof(Mat4f));
		s_DTools.pShdIter += sizeof(Mat4f);
		m4Cam = s_DSData.pGCamera->GetViewMatrix();
		memcpy(s_DTools.pShdIter, reinterpret_cast<Byte*>(&m4Cam), sizeof(Mat4f));
		s_DTools.pShdIter += sizeof(Mat4f);
		
		s_DTools.szShdData += sizeof(Mat4f) * 2;

		for (auto itDODatas : s_DOData) {
			std::sort(itDODatas.second.begin(), itDODatas.second.end(), std::greater());
			for (s_DODataIter = itDODatas.second.begin();
				s_DODataIter != itDODatas.second.end(); s_DODataIter++) {
				UploadVtxData(s_DODataIter->pDrawable);
			}
			s_pGApi->SetPrimitiveType(s_DOData.begin()->second.begin()->DrawPrimitive);
			s_DTools.pGMtl = itDODatas.first;
			DrawCall();
		}
		s_DOData.clear();
		s_bIsDrawing = false;
	}

	inline void GraphEngine::UploadVtxData(ADrawable* pDrawable)
	{
		UInt32 szVtx = pDrawable->GetVDataSize();
		UInt32 unVtx = pDrawable->GetVDataCount();
		UInt32 szIdx = pDrawable->GetIDataSize();
		UInt32 unIdx = pDrawable->GetIDataCount();
		const UByte* pVData = reinterpret_cast<const UByte*>(pDrawable->GetVData());
		const UInt32* punIData = reinterpret_cast<const UInt32*>(pDrawable->GetIData());

		if (s_DTools.szVtxData + szVtx > s_DInfo.szMaxVtx ||
			s_DTools.szIdxData + szIdx > s_DInfo.szMaxIdx ||
			s_DTools.pGMtl->GetTexCount() >= NW_MAX_TEXTURES) {
			DrawCall();
		}
	TextureData:
		if (ATexture* pTex = pDrawable->pGMtl->GetTexture())
		{
			if (Int32 txi = s_DTools.pGMtl->HasTexture(pTex) != -1) { goto VertexData; }
			else { s_DTools.pGMtl->SetTexture(pTex, ""); }
		}
	VertexData:
		pDrawable->UpdateVData();
		for (UInt32 vti = 0; vti < szVtx; vti++) { *s_DTools.pVtxIter++ = pVData[vti]; }
	IndexData:
		pDrawable->UpdateIData();
		for (UInt32 ini = 0; ini < unIdx; ini++) { *s_DTools.pIdxIter++ = punIData[ini] + s_DTools.unVtxData; }
	UpdateInfo:
		s_DTools.szVtxData += szVtx;
		s_DTools.unVtxData += unVtx;
		s_DTools.szIdxData += szIdx;
		s_DTools.unIdxData += unIdx;
	}
	inline void GraphEngine::UploadShdData()
	{
	}
	inline void GraphEngine::DrawCall()
	{
		s_DTools.pGMtl->Enable();

		s_DTools.pVtxBuf->SetLayout(s_DTools.pGMtl->GetShader()->GetVertexLayout());
		s_DTools.pShdBuf->SetSubData(s_DTools.szShdData, &s_DTools.pShdData[0]);
		s_DTools.pVtxBuf->SetSubData(s_DTools.szVtxData, &s_DTools.pVtxData[0]);
		s_DTools.pIdxBuf->SetSubData(s_DTools.szIdxData, &s_DTools.pIdxData[0]);

		s_DTools.pShdBuf->Bind(1, sizeof(Mat4f) * 2, 0);
		s_DTools.pVtxBuf->Bind();
		s_DTools.pIdxBuf->Bind();
		s_pGApi->DrawIndexed(s_DTools.szIdxData / sizeof(UInt32));
		s_DTools.pVtxBuf->Unbind();
		s_DTools.pIdxBuf->Unbind();
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
	/// -- GraphEngine collects all the Vertex and Index data into some arrays from given object
	/// -- AGMaterial of the object provides GraphEngine's AGMaterial with textures
	/// -- Every new texture from new AGMaterial will be bound to the current GMtl
	/// -- In the EndDraw it will bind all owned textures and will set their samplerIDs to the own shader
	/// -- There is also default white texture which will be used if we don not have other textures