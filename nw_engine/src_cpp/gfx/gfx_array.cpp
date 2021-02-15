#include <nw_pch.hpp>
#include "gfx_array.h"
#if (defined NW_GAPI)
#include <gfx/gfx_engine.h>
#include <gfx/gfx_loader.h>
namespace NW
{
	VertexArr::VertexArr() : m_gpType(GPT_TRIANGLES) { }
	VertexArr::~VertexArr() { }
}
#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	VertexArrOgl::VertexArrOgl(GfxEngineOgl& rGfx) :
		VertexArr(), GfxEntityOgl(rGfx)
	{
		glCreateVertexArrays(1, &m_unRId);
	}
	VertexArrOgl::~VertexArrOgl() { glDeleteVertexArrays(1, &m_unRId); }

	// --setters
	// --==<core_methods>==--
	void VertexArrOgl::Bind() const { glBindVertexArray(m_unRId); }

	void VertexArrOgl::Remake(const VertexBufLayout& rvtxLayout) {
		if (m_idxBuf.GetRef() != nullptr) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxBuf->GetRenderId()); }
		else { glBindBuffer(GBT_INDEX, 0); }
		for (auto& itBuf : m_vtxBufs) { glBindBuffer(GL_ARRAY_BUFFER, itBuf->GetRenderId()); }
		for (UInt32 ati = 0; ati < m_vtxLayout.GetElems().size(); ati++) { glDisableVertexAttribArray(ati); }
		m_vtxLayout = rvtxLayout;
		for (UInt32 ati = 0; ati < m_vtxLayout.GetElems().size(); ati++) {
			const BufferElement& rBufElem = m_vtxLayout.GetElem(ati);
			glEnableVertexAttribArray(ati);
			glVertexAttribPointer(ati, rBufElem.unCount, rBufElem.sdType,
				rBufElem.bNormalized ? GL_TRUE : GL_FALSE, m_vtxLayout.GetStride(), reinterpret_cast<Ptr>(rBufElem.unOffset));
		}
	}
	void VertexArrOgl::CreateVtxBuffer()
	{
		RefKeeper<VertexBufOgl> vtxBuf;
		vtxBuf.MakeRef<VertexBufOgl>(m_rGfx);
		m_vtxBufs.push_back(vtxBuf);
	}
	void VertexArrOgl::CreateIdxBuffer()
	{
		m_idxBuf.MakeRef<IndexBufOgl>(m_rGfx);
	}
	// --==</core_methods>==--
}
#endif
#if (NW_GAPI & NW_GAPI_DX)
namespace NW
{
	VertexArrDx::VertexArrDx(GfxEngineDx& rGfx) :
		VertexArr(), GfxEntityDx(rGfx) { }
	VertexArrDx::~VertexArrDx() { }

	// --==<core_methods>==--
	void VertexArrDx::Bind() const {
	}
	void VertexArrDx::Unbind() const {
	}

	void VertexArrDx::Remake(const VertexBufLayout& rvtxLayout) {
	}
	void VertexArrDx::CreateVtxBuffer()
	{
		RefKeeper<VertexBuf> vtxBuf;
		vtxBuf.MakeRef<VertexBufDx>();
		AddVtxBuffer(vtxBuf);
	}
	void VertexArrDx::CreateIdxBuffer()
	{
		RefKeeper<IndexBuf> idxBuf;
		idxBuf.MakeRef<IndexBufDx>();
		SetIdxBuffer(idxBuf);
	}
	// --==</core_methods>==--
}
#endif
#endif	// NW_GAPI