#include <nw_pch.hpp>
#include "nw_drawer.h"

#include <gl/control/nw_gapi.h>
#include <gl/gcontext/nw_gcontext.h>
#include <gl/gcontext/nw_window.h>
#include <gl/gcontext/nw_framebuf.h>
#include <gl/vision/nw_gcamera.h>
#include <gl/vision/nw_gmaterial.h>
#include <gl/render/nw_drawable.h>

#include <core/nw_engine.h>
#include <ecs/nw_scene.h>

#include <sys/nw_mem_sys.h>

namespace NW
{
	class NW_API GMaterialOfDrawer : public AGMaterial
	{
	public:
		GMaterialOfDrawer() : AGMaterial("gmt_of_drawer") {}
		~GMaterialOfDrawer() {}

		// -- Getters
		inline UInt8 GetTexCount() const { return m_unTex; }
		virtual inline AShader* GetShader() override { return m_pShader; }
		virtual inline ATexture* GetTexture(CString strNum) override { return m_pTexs[static_cast<UInt32>(strNum[0])]; }
		virtual inline V4f GetColor(CString strNum) override { return { 1.0f, 1.0f, 1.0f, 1.0f }; }
		// -- Setters
		virtual inline void SetShader(AShader* pShader) override { m_pShader = pShader; }
		virtual inline void SetTexture(ATexture* pTex, CString strNum) override {
			m_pTexs[static_cast<UInt32>(strNum[0])] = pTex;
		}
		inline void SetTexture(ATexture* pTex, UInt8 nIndex) { m_pTexs[nIndex] = pTex; }
		// -- Predicates
		inline Int8 HasTexture(ATexture* pTex) {
			for (Int8 txi = 0; txi < m_unTex; txi++) { if (m_pTexs[txi] == pTex) return txi; } return -1;
		}
		virtual inline void SetColor(V4f rgbaClr, CString strNum) { V4f{ 1.0f, 1.0f, 1.0f, 1.0f }; }
		// -- Core Methods
		virtual void Enable() override {}
		virtual void Disable() override {}
		// -- Data Methods
		virtual bool SaveF(const char* strFPath) override { return true; }
		virtual bool LoadF(const char* strFPath) override { return true; }
	private:
		AShader* m_pShader = nullptr;
		ATexture* m_pTexs[NW_MAX_TEXTURES]{nullptr};
		UInt8 m_unTex = 0;
	};

	struct NW_API DrawTools {
	public:
		// -- Vertex data
		UInt8* pVtxData = nullptr;
		UInt8* pVtxDataIter = nullptr;
		UInt32 unVtx = 0;
		Size szVtx = 0;
		// -- Index data
		UInt32* pIndData = nullptr;
		UInt32* pIndDataIter = nullptr;
		UInt32 unInd = 0;
		// -- Texture data
		// -- Objects
		GMaterialOfDrawer* pGMtl = nullptr;
		AVertexBuf* pVtxBuf = nullptr;
		AIndexBuf* pIndBuf = nullptr;
	public:
		inline void ResetData() {
			pVtxDataIter = &pVtxData[0];
			unVtx = szVtx = 0;
			pIndDataIter = &pIndData[0];
			unInd = 0;
			UChar unTex = pGMtl->GetTexCount();
			for (UChar txi = 0; txi < unTex; txi++) pGMtl->SetTexture(nullptr, CString(txi));
		}
	};
	static DArray<DrawObjectData> s_DOData;
	static DArray<DrawObjectData>::iterator s_pDODataCurr;
	static DrawTools s_DTools = { 0 };

	RefOwner<AGraphicsApi> Drawer::s_pGApi(nullptr);
	DrawerInfo Drawer::s_DInfo = { 0 };
	bool Drawer::s_bIsDrawing = false;
}

namespace NW
{
	// -- Setters
	void Drawer::SetVertexSize(UInt32 szVertex) {
		s_DInfo.SetVertexSize(szVertex);
	}
	void Drawer::SetMaxVCount(UInt32 unMaxVtx) {
		MemSys::DelTArr<UByte>(s_DTools.pVtxData, s_DInfo.szMaxVtx);
		if (s_DTools.pVtxBuf != nullptr) { MemSys::DelT<AVertexBuf>(s_DTools.pVtxBuf); s_DTools.pVtxBuf = nullptr; }

		s_DInfo.SetMaxVCount(unMaxVtx);
		if (unMaxVtx > 0 && s_DInfo.szVertex > 0) {
			s_DTools.pVtxData = MemSys::NewTArr<UByte>(s_DInfo.szMaxVtx);
			s_DTools.pVtxBuf = AVertexBuf::Create(s_DInfo.szMaxVtx);
		}
	}
	void Drawer::SetMaxICount(UInt32 unMaxInd) {
		MemSys::DelTArr<UInt32>(s_DTools.pIndData, s_DInfo.unMaxInd);
		if (s_DTools.pIndBuf != nullptr) { MemSys::DelT<AIndexBuf>(s_DTools.pIndBuf); s_DTools.pIndBuf = nullptr; }

		s_DInfo.SetMaxICount(unMaxInd);
		if (unMaxInd > 0) {
			s_DTools.pIndData = MemSys::NewTArr<UInt32>(s_DInfo.unMaxInd);
			s_DTools.pIndBuf = AIndexBuf::Create(s_DInfo.unMaxInd);
		}
	}
	void Drawer::SetMaxTexCount(UInt8 unMaxTex) {
		if (unMaxTex > NW_MAX_TEXTURES || unMaxTex < NW_MIN_TEXTURES) { NW_ERR("We can not set this amount of slots"); return; }
		s_DInfo.unMaxTex = unMaxTex;
	}

	// ========<Core Methods>========
	bool Drawer::Init(GApiTypes GraphicsApiType)
	{
		s_pGApi.reset(AGraphicsApi::Create(GraphicsApiType));

		s_DTools.pGMtl = MemSys::NewT<GMaterialOfDrawer>();

		s_DInfo.SetVertexSize(sizeof(VertexBatch2d));
		SetMaxVCount(300);
		SetMaxICount(300 * 3);
		SetMaxTexCount(NW_MAX_TEXTURES);
		s_DTools.ResetData();

		s_pGApi->SetClearColor(Pixel{ 64, 64, 96 });
		
		FrameBufInfo fbInfo;
		fbInfo.unWidth = Engine::Get().GetWindow()->GetWidth();
		fbInfo.unHeight = Engine::Get().GetWindow()->GetHeight();
		fbInfo.unSamples = 1;
		Scene::Get().SetFrameBuf(AFrameBuf::Create("fmb_scene", fbInfo));

		return true;
	}
	void Drawer::OnQuit()
	{
		SetVertexSize(0);
		SetMaxVCount(0);
		SetMaxICount(0);
		SetMaxTexCount(1);
		s_pGApi.reset(nullptr);

		MemSys::DelT<AFrameBuf>(Scene::Get().GetFrameBuf());
	}
	void Drawer::Update()
	{
		s_DInfo.Reset();
	}
	// --==<Drawing>==--
	void Drawer::BeginDraw()
	{
		bool bCanDraw = !s_bIsDrawing;
		if (!bCanDraw) { NW_ERR("Can not Draw now"); return; }
		s_bIsDrawing = true;
	}
	void Drawer::OnDraw(const DrawObjectData& rDOData)
	{
		bool bCanDraw = s_bIsDrawing && rDOData.pDrawable != nullptr;
		if (!bCanDraw) { NW_ERR("Can not Draw now"); return; }

		s_DOData.push_back(rDOData);
	}
	void Drawer::EndDraw()
	{
		bool bCanDraw = s_bIsDrawing;
		if (!bCanDraw) { NW_ERR("Can not Draw now"); return; }
		
		std::sort(s_DOData.begin(), s_DOData.end(), std::greater());
		for (DArray<DrawObjectData>::iterator itDOD = s_DOData.begin();
			itDOD != s_DOData.end(); itDOD++) {
			UploadData(itDOD->pDrawable);
			s_DTools.pGMtl->SetShader(itDOD->pDrawable->GetGMaterial()->GetShader());
			s_pGApi->SetPrimitiveType(itDOD->DrawPrimitive);
			DrawCall();
		}
		s_DOData.clear();

		s_bIsDrawing = false;
	}

	inline void Drawer::UploadData(ADrawable* pDrawable)
	{
		UInt32 unVtx = pDrawable->GetVDataCount();
		UInt32 szVtx = pDrawable->GetVDataSize();
		UInt32 unInd = pDrawable->GetIDataCount();

		if (s_DTools.szVtx + szVtx > s_DInfo.szMaxVtx ||
			s_DTools.unVtx + unVtx > s_DInfo.unMaxVtx ||
			s_DTools.unInd + unInd > s_DInfo.unMaxInd ||
			s_DTools.pGMtl->GetTexCount() >= NW_MAX_TEXTURES) {
			DrawCall();
		}
	TextureData:
		if (ATexture* pTex = pDrawable->GetGMaterial()->GetTexture())
		{
			if (Int32 txi = s_DTools.pGMtl->HasTexture(pTex) != -1) { goto VertexData; }
			else { s_DTools.pGMtl->SetTexture(pTex, s_DTools.pGMtl->GetTexCount()); }
		}
	VertexData:
		const UInt8* pVData = reinterpret_cast<UInt8*>(pDrawable->GetVData());
		for (UInt32 vti = 0; vti < szVtx; vti++) {
			*s_DTools.pVtxDataIter = pVData[vti];
			s_DTools.pVtxDataIter++;
		}
	IndexData:
		const UInt32* punIData = pDrawable->GetIData();
		for (UInt32 ini = 0; ini < unInd; ini++) {
			*s_DTools.pIndDataIter = punIData[ini] + s_DTools.unVtx;
			s_DTools.pIndDataIter++;
		}
		s_DTools.szVtx += szVtx;
		s_DTools.unVtx += unVtx;
		s_DTools.unInd += unInd;
	}
	inline void Drawer::DrawCall()
	{
		AShader* pShader = s_DTools.pGMtl->GetShader();
		s_DTools.pGMtl->Enable();

		s_DTools.pVtxBuf->SetLayout(pShader->GetBufferLayout());
		s_DTools.pVtxBuf->SetData(s_DTools.szVtx, &s_DTools.pVtxData[0]);
		s_DTools.pIndBuf->SetData(s_DTools.unInd, &s_DTools.pIndData[0]);
		s_DTools.pVtxBuf->Bind();
		s_DTools.pIndBuf->Bind();
		s_pGApi->DrawIndexed(s_DTools.unInd);
		s_DTools.pVtxBuf->Unbind();
		s_DTools.pIndBuf->Unbind();

		s_DTools.pGMtl->Disable();

		s_DInfo.SetVCount(s_DInfo.unVtx + s_DTools.unVtx);
		s_DInfo.SetICount(s_DInfo.unInd + s_DTools.unInd);
		if (s_DInfo.unTex < s_DTools.pGMtl->GetTexCount()) { s_DInfo.unTex = s_DTools.pGMtl->GetTexCount(); }
		s_DInfo.unDrawCalls++;
		s_DTools.ResetData();
	}
	// --==<Drawing>==--
	// ========</Core Methods>========
}

/// OnDraw (AShape)
	/// Description:
	/// -- Shapes have a vertex data, indices and bound AGMaterial
	/// -- Drawer collects all the Vertex and Index data into some arrays from given object
	/// -- AGMaterial of the object provides Drawer's AGMaterial with textures
	/// -- Every new texture from new AGMaterial will be bound to the current GMtl
	/// -- In the EndDraw it will bind all owned textures and will set their samplerIDs to the own shader
	/// -- There is also default white texture which will be used if we don not have other textures