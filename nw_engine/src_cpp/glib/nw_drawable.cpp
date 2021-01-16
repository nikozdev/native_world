#include "nw_pch.hpp"
#include "nw_drawable.h"

#include <glib/nw_texture.h>
#include <glib/vision/nw_gmaterial.h>
#include <glib/core/nw_gengine.h>

#include <sys/nw_data_sys.h>

// --==<Drawable>==--
namespace NW
{
	// --==<Particles>==--
	Particles::Particles() :
		ADrawable(),
		unPartCount(0), unPartScale(0.0f),
		DrawPrimitive(PT_POINTS) { pGMtl = DataSys::GetDataRes<GMaterial>("gmt_default"); }

	// --core_methods
	void Particles::OnDraw()
	{
	}
	// --==</Particles>==--
	
	// --==<PolyLine>==--
	PolyLine::PolyLine() :
		ADrawable() { pGMtl = DataSys::GetDataRes<GMaterial>("gmt_default"); }

	// --core_methods
	void PolyLine::OnDraw()
	{
	}
	// --==</PolyLine>==--

	// --==<Polygons>==--
	Polygons::Polygons() : ADrawable(),
		xyzCrd{ 0.0f, 0.0f, 0.0f }, xyzRtn{ 0.0f, 0.0f, 0.0f }, xyzScale{ 1.0f, 1.0f, 1.0f },
		rgbaClr{ 1.0f, 1.0f, 1.0f, 1.0f }
	{
		pGMtl = DataSys::GetDataRes<GMaterial>("gmt_default");
	}

	// --core_methods
	void Polygons::OnDraw()
	{
		//
	}
}
// --==</Drawable>==--

// --==<Shapes2d>==--
namespace NW
{
	// --==<Triangle>==--
	Triangle::Triangle(const V2f& xyV0, const V2f& xyV1, const V2f& xyV2) :
		ADrawable(),
		vtxCrds{ xyV0, xyV1, xyV2 } { pGMtl = DataSys::GetDataRes<GMaterial>("gmt_default"); }

	// --core_methods
	void Triangle::OnDraw() { }
	// --==</Triangle>==--

	// --==<Rectangle>==--
	Rectangle::Rectangle(const V2f& whSize) :
		ADrawable(),
		whSize(whSize), xyPivot{ V2f{ 0.5f, 0.5f } },
		idxData{ 0, 1, 2,	2, 3, 0 },
		SubTex(SubTexture2d())
	{
		pGMtl = DataSys::GetDataRes<GMaterial>("gmt_3d_batch");
	}

	// --core_methods
	void Rectangle::OnDraw()
	{
		V2f xyTexCrd = { 0.0f, 0.0f }, whTexSize = { 1.0f, 1.0f };
		if (ATexture2d* pTex = dynamic_cast<ATexture2d*>(pGMtl->GetTexture())) {
			if (SubTex.pOverTex != pTex) {
				SubTex.pOverTex = pTex;
				SubTex.xyTexCrd = { 0, 0 };
				SubTex.whOverTexSize = SubTex.whTexSize = { pTex->GetWidth(), pTex->GetHeight() };
			}
		}
		xyTexCrd = SubTex.GetTexCoord_0_1();
		whTexSize = SubTex.GetTexSize_0_1();
		
		vtxData[0].texCrd = V2f{ xyTexCrd.x, xyTexCrd.y + whTexSize.y };					// { left; bottom } == { xCoord, height }
		vtxData[1].texCrd = V2f{ xyTexCrd.x, xyTexCrd.y };									// { left; up } == { xCoord, yCoord }
		vtxData[2].texCrd = V2f{ xyTexCrd.x + whTexSize.x, xyTexCrd.y };					// { right; up } == { width, yCoord }
		vtxData[3].texCrd = V2f{ xyTexCrd.x + whTexSize.x, xyTexCrd.y + whTexSize.y };		// { right; bottom } == { width, height }

		vtxCrds[0] = V2f{ xyPivot.x * whSize.x - ((1.0f - xyPivot.x) * whSize.x), xyPivot.y * whSize.y - ((1.0f - xyPivot.y) * whSize.y) };
		vtxCrds[1] = V2f{ xyPivot.x * whSize.x - ((1.0f - xyPivot.x) * whSize.x), xyPivot.y * whSize.y + ((1.0f + xyPivot.y) * whSize.y) };
		vtxCrds[2] = V2f{ xyPivot.x * whSize.x + ((1.0f + xyPivot.x) * whSize.x), xyPivot.y * whSize.y + ((1.0f + xyPivot.y) * whSize.y) };
		vtxCrds[3] = V2f{ xyPivot.x * whSize.x + ((1.0f + xyPivot.x) * whSize.x), xyPivot.y * whSize.y - ((1.0f - xyPivot.y) * whSize.y) };
		
		for (UInt8 vti = 0; vti < 4; vti++) {
			vtxData[vti].vtxCrd = V3f{ vtxCrds[vti].x, vtxCrds[vti].y, 0.0f };
			vtxData[vti].nTexSlot = static_cast<float>(SubTex.pOverTex->GetTexSlot());
			vtxData[vti].m4Transform = m4Transform;
			vtxData[vti].vtxClr = *pGMtl->GetColor();
		}

		GEngine::Get().GetLayer()->UploadObjectData(&vtxData[0],
			4 * sizeof(VertexBatch3d), &idxData[0], 6 * sizeof(UInt32));
	}
	// --==</Rectangle>==--

	// --==<TileMapSprite>==--
	TileMapSprite::TileMapSprite() :
		pTileMap(DataSys::GetDataRes<ATexture2d>("tex_white_solid")),
		whTileSize{ 1, 1 }, xywhTilePadding{ 1, 1, 1, 1 }
	{
		pGMtl = DataSys::GetDataRes<GMaterial>("gmt_default_tile");
		UpdateTileData();
	}

	// --core_methods
	void TileMapSprite::OnDraw() {
		for (UInt8 vti = 0; vti < vtxData.size() / 4; vti++) {
			vtxData[vti + 0].vtxCrd = V4f{1.0f, 1.0f, 1.0f, 1.0f};
			vtxData[vti + 0].m4Transform = m4Transform;
			vtxData[vti + 0].nTexSlot = pTileMap->GetTexSlot();
			vtxData[vti + 0].vtxClr = *pGMtl->GetColor();
		
			vtxData[vti + 1].vtxCrd = V4f{ 1.0f, 1.0f, 1.0f, 1.0f };
			vtxData[vti + 1].m4Transform = m4Transform;
			vtxData[vti + 1].nTexSlot = pTileMap->GetTexSlot();
			vtxData[vti + 1].vtxClr = *pGMtl->GetColor();

			vtxData[vti + 2].vtxCrd = V4f{ 1.0f, 1.0f, 1.0f, 1.0f };
			vtxData[vti + 2].m4Transform = m4Transform;
			vtxData[vti + 2].nTexSlot = pTileMap->GetTexSlot();
			vtxData[vti + 2].vtxClr = *pGMtl->GetColor();
		}
	}

	void TileMapSprite::UpdateTileData()
	{
		whWholeTileSize.x = whTileSize.x + xywhTilePadding.x + xywhTilePadding.y;
		whWholeTileSize.y = whTileSize.y + xywhTilePadding.y + xywhTilePadding.w;
		xyTileCount.x = pTileMap->GetWidth() / (whWholeTileSize.x);
		xyTileCount.y = pTileMap->GetHeight() / (whWholeTileSize.y);
		Int16 nVCount = xyTileCount.x * xyTileCount.y * 3;
		if (nVCount == 0) { nVCount = 1; }
		vtxData.resize(nVCount);
		idxData.resize(nVCount);
		for (UInt16 ix = 0; ix < xyTileCount.x; ix++) {		// itterate throught all columns
			for (UInt16 iy = 0; iy < xyTileCount.y; iy++) {	// throught all rows
				UInt16 nCurrCell = ix * 3 + iy * 3;

				auto& rVtx = vtxData[nCurrCell + 0];
				rVtx.texCrd = {		// uv of left bottom
					static_cast<float>(whTileSize.x) * (ix + 0) / static_cast<float>(pTileMap->GetWidth()),
					static_cast<float>(whTileSize.y) * (iy + 1) / static_cast<float>(pTileMap->GetHeight()) };
				rVtx.vtxCrd = { rVtx.texCrd.x, rVtx.texCrd.y, 0.0f };
				
				rVtx = vtxData[nCurrCell + 1];
				rVtx.texCrd = {		// uv of left top
						static_cast<float>(whTileSize.x) * (ix + 0) / static_cast<float>(pTileMap->GetWidth()),
						static_cast<float>(whTileSize.y) * (iy + 0) / static_cast<float>(pTileMap->GetHeight()) };
				rVtx.vtxCrd = { rVtx.texCrd.x, rVtx.texCrd.y, 0.0f };
				
				rVtx = vtxData[nCurrCell + 2];
				rVtx.texCrd = {		// uv of right top
						static_cast<float>(whTileSize.x) * (ix + 1) / static_cast<float>(pTileMap->GetWidth()),
						static_cast<float>(whTileSize.y) * (iy + 1) / static_cast<float>(pTileMap->GetHeight()) };
				rVtx.vtxCrd = { rVtx.texCrd.x, rVtx.texCrd.y, 0.0f };
			};
		}
		for (UInt32 idi = 0; idi < nVCount; idi++) { idxData[idi] = idi; }
	}
	// --==</TileMapSprite>==--
}
// --==</Shape2d>==--

// --==<Shape3d>==--
namespace NW
{
	Mesh3d::Mesh3d() :
		ADrawable() { }
	Mesh3d::~Mesh3d() = default;

	// --setters
	void Mesh3d::SetData(VertexShape3d * pVData, UInt32 unVCount, UInt32 * punIndData, UInt32 unIndCount)
	{
		vtxData.reserve(unVCount);
		for (UInt32 vti = 0; vti < unVCount; vti++)
		{
			vtxData.emplace_back(pVData[vti]);
		}
		idxData.reserve(unIndCount);
		for (UInt32 idi = 0; idi < unIndCount; idi++)
		{
			idxData.emplace_back(punIndData[idi]);
		}
	}
	// --core_methods
	void Mesh3d::OnDraw()
	{
	}
}
// --==</Shape3d>==--