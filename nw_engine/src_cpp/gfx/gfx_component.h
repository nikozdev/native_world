#ifndef GFX_COMPONENT_H
#define GFX_COMPONENT_H

#include <gfx_engine.h>
#include <gfx_buffer.h>
#include <gfx_material.h>
#include <gfx_shader.h>
#include <gfx_texture.h>
#include <gfx_drawable.h>

#include <nwlib/nwl_entity.h>

namespace NW
{
	static inline DArray<Float32> MakeVtxRect(const Mat4f& m4Tf = Mat4f(1.0f)) {
		DArray<Float32 >vtxData = {
	-0.5f,	-0.5f,	0.0f,			1.0f,	1.0f,	1.0f,	0.5f,		0.0f,	0.0f,		0.0f,
	m4Tf[0][0],	m4Tf[0][1],	m4Tf[0][2],	m4Tf[0][3],
	m4Tf[1][0],	m4Tf[1][1],	m4Tf[1][2],	m4Tf[1][3],
	m4Tf[2][0],	m4Tf[2][1],	m4Tf[2][2],	m4Tf[2][3],
	m4Tf[3][0],	m4Tf[3][1],	m4Tf[3][2],	m4Tf[3][3],
	-0.5f,	0.5f,	0.0f,			1.0f,	1.0f,	1.0f,	0.5f,		0.0f,	1.0f,		0.0f,
	m4Tf[0][0],	m4Tf[0][1],	m4Tf[0][2],	m4Tf[0][3],
	m4Tf[1][0],	m4Tf[1][1],	m4Tf[1][2],	m4Tf[1][3],
	m4Tf[2][0],	m4Tf[2][1],	m4Tf[2][2],	m4Tf[2][3],
	m4Tf[3][0],	m4Tf[3][1],	m4Tf[3][2],	m4Tf[3][3],
	0.5f,	0.5f,	0.0f,			1.0f,	1.0f,	1.0f,	0.5f,		1.0f,	1.0f,		0.0f,
	m4Tf[0][0],	m4Tf[0][1],	m4Tf[0][2],	m4Tf[0][3],
	m4Tf[1][0],	m4Tf[1][1],	m4Tf[1][2],	m4Tf[1][3],
	m4Tf[2][0],	m4Tf[2][1],	m4Tf[2][2],	m4Tf[2][3],
	m4Tf[3][0],	m4Tf[3][1],	m4Tf[3][2],	m4Tf[3][3],
	0.5f,	-0.5f,	0.0f,			1.0f,	1.0f,	1.0f,	0.5f,		1.0f,	0.0f,		0.0f,
	m4Tf[0][0],	m4Tf[0][1],	m4Tf[0][2],	m4Tf[0][3],
	m4Tf[1][0],	m4Tf[1][1],	m4Tf[1][2],	m4Tf[1][3],
	m4Tf[2][0],	m4Tf[2][1],	m4Tf[2][2],	m4Tf[2][3],
	m4Tf[3][0],	m4Tf[3][1],	m4Tf[3][2],	m4Tf[3][3]
		};
		return vtxData;
	}
}

namespace NW
{
	/// TransformComponent class
	class GFX_API TransformCmp : public TCmp<TransformCmp>
	{
	public:
		TransformCmp() : TCmp() {}
		// --getters
		inline Mat4f GetMatrix() const;
		// --setters
	public:
		V3f xyzCrd = { 0.0f,	0.0f,	0.0f };
		V3f xyzRtn = { 0.0f,	0.0f,	0.0f };
		V3f xyzScl = { 1.0f,	1.0f,	1.0f };
	};
	// --getters
	inline Mat4f TransformCmp::GetMatrix() const {
		Mat4f m4Tf = Mat4f(1.0f);
		m4Tf = glm::translate(m4Tf, xyzCrd);
		m4Tf = glm::rotate(m4Tf, RadToDeg(xyzRtn.x), V3f{ 1.0f, 0.0f, 0.0f });
		m4Tf = glm::rotate(m4Tf, RadToDeg(xyzRtn.y), V3f{ 0.0f, 1.0f, 0.0f });
		m4Tf = glm::rotate(m4Tf, RadToDeg(xyzRtn.z), V3f{ 0.0f, 0.0f, 1.0f });
		m4Tf = glm::scale(m4Tf, xyzScl);
		return m4Tf;
	}
	/// GraphicsComponent2dComponent class
	class GFX_API Graphics2dCmp : public TCmp<Graphics2dCmp>
	{
	public:
		Drawable m_drw = Drawable();
	public:
		Graphics2dCmp() : TCmp(), m_drw(Drawable()) {
			RefKeeper<VertexBuf> vtxBuf;
			RefKeeper<IndexBuf> idxBuf;
			VertexArr::Create(m_drw.vtxArr);
			VertexBuf::Create(vtxBuf);
			IndexBuf::Create(idxBuf);

			m_drw.vtxArr->AddVtxBuffer(vtxBuf);
			m_drw.vtxArr->SetIdxBuffer(idxBuf);
			auto vtxData = MakeVtxRect();
			UInt32 idxData[] = { 0,	1,	2,		2,	3,	0 };
			BufferData vbData{ &vtxData[0], vtxData.size() * sizeof(Float32), 0 };
			BufferData ibData{ &idxData[0], sizeof(idxData), 0 };
			vtxBuf->SetData(vbData.szData, nullptr);
			idxBuf->SetData(ibData.szData, nullptr);
			m_drw.UploadVtxData(&vbData);
			m_drw.UploadIdxData(&ibData);
			m_drw.gMtl = TDataRes<GMaterial>::GetDataRes("gmt_3d_batch");
			m_drw.vtxArr->Remake(m_drw.gMtl->GetShader()->GetVtxLayout());
		}
	};
}

#endif	// GFX_COMPONENT_H