#include <nw_pch.hpp>
#include "gfx_array.h"
#if (defined NW_GAPI)
namespace NW
{
	VertexArr::VertexArr() :
		m_unRId(0), m_bIsBound(false), m_gpType(GPT_TRIANGLES) { }
	VertexArr::~VertexArr() { }
}
#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	VertexArrOgl::VertexArrOgl() :
		VertexArr() {
		glCreateVertexArrays(1, &m_unRId);
	}
	VertexArr::~VertexArr() { glDeleteVertexArrays(1, &m_unRId); }

	// --setters
	// --==<core_methods>==--
	void VertexArrOgl::Bind() const {
		if (m_bIsBound) { return; }
		glBindVertexArray(m_unRId);
		m_bIsBound = true;
}
	void VertexArrOgl::Unbind() const {
		if (!m_bIsBound) { return; }
		glBindVertexArray(0);
		m_bIsBound = false;
	}

	void VertexArr::Remake(const VertexBufLayout& rvtxLayout) {
		NWL_ASSERT(m_vtxBufs.size() > 0, "there are no vertex buffers!");
		Bind();
		if (m_idxBuf.GetRef() != nullptr) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxBuf->GetRenderId()); }
		else { glBindBuffer(GBT_INDEX, 0); }
		for (auto& itBuf : m_vtxBufs) {
			NWL_ASSERT(itBuf->GetRenderId() != 0, "vertex buffer is invalid!");
			glBindBuffer(GL_ARRAY_BUFFER, itBuf->GetRenderId());
		}
		for (UInt32 ati = 0; ati < m_vtxLayout.GetElems().size(); ati++) { glDisableVertexAttribArray(ati); }
		m_vtxLayout = rvtxLayout;
		for (UInt32 ati = 0; ati < m_vtxLayout.GetElems().size(); ati++) {
			const BufferElement& rBufElem = m_vtxLayout.GetElem(ati);
			glEnableVertexAttribArray(ati);
			glVertexAttribPointer(ati, rBufElem.unCount, rBufElem.sdType,
				rBufElem.bNormalized ? GL_TRUE : GL_FALSE, m_vtxLayout.GetStride(), reinterpret_cast<Ptr>(rBufElem.unOffset));
		}
		Unbind();
	}
	void VertexArr::CreateVtxBuffer()
	{
		RefKeeper<VertexBuf> vtxBuf;
		vtxBuf.MakeRef<VertexBuf>();
		AddVtxBuffer(vtxBuf);
	}
	void VertexArr::CreateIdxBuffer()
	{
		RefKeeper<IndexBuf> idxBuf;
		idxBuf.MakeRef<IndexBuf>();
		SetIdxBuffer(idxBuf);
	}
	// --==</core_methods>==--
}
#endif
#if (NW_GAPI & NW_GAPI_DX)
namespace NW
{
	VertexArrDx::VertexArrDx() :
		VertexArr() { }
	VertexArrDx::~VertexArrDx() { }

	// --setters
	// --==<core_methods>==--
	void VertexArrDx::Bind() const {
		if (m_bIsBound) { return; }
		m_bIsBound = true;
	}
	void VertexArrDx::Unbind() const {
		if (!m_bIsBound) { return; }
		m_bIsBound = false;
	}

	void VertexArrDx::Remake(const VertexBufLayout& rvtxLayout) {
		Bind();
		Unbind();
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