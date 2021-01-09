#include "nw_pch.hpp"
#include "nw_drawable.h"

#include <glib/render/nw_texture.h>
#include <glib/vision/nw_gmaterial.h>

#include <sys/nw_data_sys.h>

// --==<Drawable>==--
namespace NW
{
	// --==<Particles>==--
	Particles::Particles() :
		ADrawable(),
		unPartCount(0), unPartScale(0.0f),
		DrawPrimitive(PT_POINTS)
	{
		pGMtl = DataSys::GetDataRes<GMaterial>("gmt_default");

		UpdateVData(); UpdateIData();
	}

	// --core_methods
	void Particles::UpdateVData()
	{
	}
	void Particles::UpdateIData()
	{
	}
	// --==</Particles>==--
	
	// --==<PolyLine>==--
	PolyLine::PolyLine() :
		ADrawable()
	{
		pGMtl = DataSys::GetDataRes<GMaterial>("gmt_default");
		UpdateVData(); UpdateIData();
	}

	// --core_methods
	void PolyLine::UpdateVData()
	{
		UInt32 vCount = vtxCrds.size();
		if (vCount != vtxVData.size()) {
			vtxVData.resize(vCount);
			if (vCount > 1) {
				for (UInt8 vti = 0; vti < vCount; vti++) {
					vtxVData[vti].vtxCrd = vtxCrds[vti];
					vtxVData[vti].texCrd = 1.0f / vti;
					if (ATexture* pTex = pGMtl->GetTexture()) {
						vtxVData[vti].nTexSlot = static_cast<float>(pTex->GetTexSlot());
					}
					vtxVData[vti].vtxClr = *pGMtl->GetColor();
				}
			}
		}
	}
	void PolyLine::UpdateIData()
	{
		UInt32 vCount = vtxCrds.size();
		if (indIData.size() != vCount) {
			indIData.clear(); indIData.reserve(vCount * 2);
			if (vCount > 1) {
				for (UInt32 idi = 0; idi < vCount; idi++) {
					indIData.push_back(idi);
					indIData.push_back(idi + 1);
				}
			}
		}
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
	void Polygons::UpdateVData()
	{
		UInt32 vCount = vtxCrds.size();
		if (vCount != vtxVData.size()) {
			vtxVData.resize(vCount);
			if (vCount > 3) {
				for (UInt32 vti = 0; vti < vCount; vti++) {
					for (UInt32 vti = 0; vti < vCount; vti++) {
						vtxVData[vti].vtxCrd = vtxCrds[vti];
						vtxVData[vti].texCrd = V2f{ 1.0f / (vti + 1), 1.0f / (vti + 1) };
						if (ATexture* pTex = pGMtl->GetTexture()) {
							vtxVData[vti].nTexSlot = static_cast<float>(pTex->GetTexSlot());
						}
						vtxVData[vti].vtxClr = rgbaClr;
						vtxVData[vti].m4Transform = m4Transform;
						vtxVData[vti].vtxClr = *pGMtl->GetColor();
					}
				}
			}
		}
	}
	void Polygons::UpdateIData()
	{
		UInt32 vCount = vtxCrds.size();
		if (vCount != indIData.size()) {
			indIData.clear(); indIData.reserve(vCount * 3);
			if (vCount > 3) {
				indIData.push_back(0);
				indIData.push_back(1);
				indIData.push_back(2);
				for (UInt32 idi = 3; idi < vCount;) {
					if (idi % 3 == 0) { indIData.push_back(idi - 1); }
					else { indIData.push_back(idi); }
					idi++;
				}
			}
		}
	}
	// --==</Polygons>==--
}
// --==</Drawable>==--

// --==<Shapes2d>==--
namespace NW
{
	// --==<Triangle>==--
	Triangle::Triangle() :
		ADrawable(),
		vtxCrds{ {-0.5f, -0.5f}, { 0.25f, 0.5f }, {0.5f, -0.5f} }
	{
		pGMtl = DataSys::GetDataRes<GMaterial>("gmt_default");
		UpdateVData();
		UpdateIData();
	}
	Triangle::Triangle(const V2f& xyV0, const V2f& xyV1, const V2f& xyV2) :
		ADrawable(),
		vtxCrds{ xyV0, xyV1, xyV2 }
	{
		pGMtl = DataSys::GetDataRes<GMaterial>("gmt_default");
		UpdateVData();
		UpdateIData();
	}

	// --core_methods
	void Triangle::UpdateVData()
	{
		for (UInt8 vti = 0; vti < 3; vti++) {
			vtxVData[vti].vtxCrd = V3f{ vtxCrds[vti].x, vtxCrds[vti].y, 0.0f };
			if (ATexture* pTex = pGMtl->GetTexture()) {
				vtxVData[vti].nTexSlot = static_cast<float>(pTex->GetTexSlot());
			}
			//vtxVData[vti].vtxClr = pGMtl->GetColor();
			vtxVData[vti].m4Transform = m4Transform;
		}
	}
	void Triangle::UpdateIData()
	{
		indIData[0] = 0; indIData[1] = 1; indIData[2] = 2;
	}
	// --==</Triangle>==--

	// --==<Rectangle>==--
	Rectangle::Rectangle(const V2f& whSize) :
		ADrawable(),
		whSize(whSize), xyPivot{ V2f{ 0.5f, 0.5f } },
		SubTex(SubTexture2d())
	{
		pGMtl = DataSys::GetDataRes<GMaterial>("gmt_default");
		UpdateVData();
		UpdateIData();
	}

	// --core_methods
	void Rectangle::UpdateVData()
	{
		V2f xyTexCrd = { 0.0f, 0.0f }, whTexSize = { 1.0f, 1.0f };
		if (ATexture2d* pTex = dynamic_cast<ATexture2d*>(pGMtl->GetTexture())) {
			if (SubTex.pOverTex != pTex) {
				SubTex.pOverTex = pTex;
				SubTex.xyTexCrd = { 0, 0 };
				SubTex.whTexSize = { pTex->GetWidth(), pTex->GetHeight() };
			}
		}
		xyTexCrd = SubTex.GetTexCoord_0_1();
		whTexSize = SubTex.GetTexSize_0_1();
		
		vtxVData[0].texCrd = V2f{ xyTexCrd.x, xyTexCrd.y + whTexSize.y };					// { left; bottom } == { xCoord, height }
		vtxVData[1].texCrd = V2f{ xyTexCrd.x, xyTexCrd.y };									// { left; up } == { xCoord, yCoord }
		vtxVData[2].texCrd = V2f{ xyTexCrd.x + whTexSize.x, xyTexCrd.y };					// { right; up } == { width, yCoord }
		vtxVData[3].texCrd = V2f{ xyTexCrd.x + whTexSize.x, xyTexCrd.y + whTexSize.y };		// { right; bottom } == { width, height }

		vtxCrds[0] = V2f{ xyPivot.x * whSize.x - ((1.0f - xyPivot.x) * whSize.x), xyPivot.y * whSize.y - ((1.0f - xyPivot.y) * whSize.y) };
		vtxCrds[1] = V2f{ xyPivot.x * whSize.x - ((1.0f - xyPivot.x) * whSize.x), xyPivot.y * whSize.y + ((1.0f + xyPivot.y) * whSize.y) };
		vtxCrds[2] = V2f{ xyPivot.x * whSize.x + ((1.0f + xyPivot.x) * whSize.x), xyPivot.y * whSize.y + ((1.0f + xyPivot.y) * whSize.y) };
		vtxCrds[3] = V2f{ xyPivot.x * whSize.x + ((1.0f + xyPivot.x) * whSize.x), xyPivot.y * whSize.y - ((1.0f - xyPivot.y) * whSize.y) };
		
		for (UInt8 vti = 0; vti < 4; vti++) {
			vtxVData[vti].vtxCrd = V3f{ vtxCrds[vti].x, vtxCrds[vti].y, 0.0f };
			if (ATexture* pTex = pGMtl->GetTexture()) {
				vtxVData[vti].nTexSlot = static_cast<float>(pTex->GetTexSlot());
			}
			vtxVData[vti].m4Transform = m4Transform;
			vtxVData[vti].vtxClr = *pGMtl->GetColor();
		}
	}
	void Rectangle::UpdateIData()
	{
		indIData[0] = 0; indIData[1] = 1; indIData[2] = 2;
		indIData[3] = 2; indIData[4] = 3; indIData[5] = 0;
	}
	// --==</Rectangle>==--
}
// --==</Shape2d>==--

// --==<Shape3d>==--
namespace NW
{
	Mesh3d::Mesh3d() :
		ADrawable() { }
	Mesh3d::~Mesh3d() = default;

	// --getters
	// --setters
	void Mesh3d::SetData(VertexShape3d * pVData, UInt32 unVCount, UInt32 * punIndData, UInt32 unIndCount)
	{
		vtxVData.reserve(unVCount);
		for (UInt32 vti = 0; vti < unVCount; vti++)
		{
			vtxVData.emplace_back(pVData[vti]);
		}
		indIData.reserve(unIndCount);
		for (UInt32 idi = 0; idi < unIndCount; idi++)
		{
			indIData.emplace_back(punIndData[idi]);
		}
	}
	// --core_methods
	void Mesh3d::UpdateVData()
	{
	}
	void Mesh3d::UpdateIData()
	{
	}
}
// --==</Shape3d>==--