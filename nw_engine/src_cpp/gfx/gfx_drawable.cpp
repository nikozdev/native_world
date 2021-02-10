#include <nw_pch.hpp>
#include <gfx/gfx_drawable.h>

#include <gfx/gfx_material.h>
#include <gfx/gfx_buffer.h>
#include <gfx/gfx_api.h>

namespace NW
{
	Drawable::Drawable() :
		vtxArr(RefKeeper<VertexArr>()),
		gMtl(nullptr) { }
	Drawable::Drawable(Size szVtxData, Size szIdxData) :
		vtxArr(RefKeeper<VertexArr>()),
		gMtl(nullptr)
	{
		vtxArr.MakeRef<VertexArr>();
		gMtl = *DataSys::GetDR("gmt_3d_batch");

		RefKeeper<VertexBuf> vtxBuf;
		vtxBuf.MakeRef<VertexBuf>();
		vtxBuf->SetData(szVtxData, nullptr);
		vtxArr->AddVtxBuffer(vtxBuf);

		RefKeeper<IndexBuf> idxBuf;
		idxBuf.MakeRef<IndexBuf>();
		idxBuf->SetData(szIdxData, nullptr);
		vtxArr->SetIdxBuffer(idxBuf);
	}

	Drawable::~Drawable() { vtxArr.Reset(); }
	// --core_methods
	void Drawable::UploadVtxData(const BufferData* pData, UInt32 unCount, UInt32 unFirstBuf)
	{
		for (UInt32 di = 0; di < unCount; di++) {
			if (VertexBuf* pvtxBuf = vtxArr->GetVtxBuffer(unFirstBuf + di)) {
				pvtxBuf->SetSubData(pData[di].szData, pData[di].pData, pData[di].szOffset);
			}
			else { return; }
		}
	}
	void Drawable::UploadIdxData(const BufferData* pData)
	{
		if (IndexBuf* pidxBuf = vtxArr->GetIdxBuffer()) {
			pidxBuf->SetSubData(pData->szData, pData->pData, pData->szOffset);
		}
	}
}