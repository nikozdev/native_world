#include <nw_pch.hpp>
#include "gfx/gfx_buffer.h"

#pragma warning(disable : 4312)

#if (defined NW_GAPI)
#include <gfx/gfx_api.h>
#include <gfx/gfx_loader.h>
#endif	// NW_API

#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	// --==<AGfxBuf>==--
	AGfxBuffer::AGfxBuffer(GfxBufferTypes gbType) : m_unRId(0), m_gbType(gbType), m_bIsBound(false), m_szData(0) { }
	AGfxBuffer::~AGfxBuffer() { Unbind(); if (m_unRId != 0) { glDeleteBuffers(1, &m_unRId); } }
	// --setters
	void AGfxBuffer::SetSubData(Size szData, const Ptr pVtxData, Size szOffset) {
		Bind();
		glBufferSubData(m_gbType, szOffset, szData, pVtxData);
		Unbind();
	}
	void AGfxBuffer::SetData(Size szData, const Ptr pVtxData) {
		Unbind();
		m_szData = szData;
		if (m_unRId != 0) { glDeleteBuffers(1, &m_unRId); m_unRId = 0; }
		if (szData == 0) { return; }
		glGenBuffers(1, &m_unRId);
		Bind();
		glBufferData(m_gbType, szData, pVtxData, pVtxData == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		Unbind();
	}
	void AGfxBuffer::Bind() const {
		if (m_bIsBound) { return; }
		glBindBuffer(m_gbType, m_unRId);
		m_bIsBound = true;
	}
	void AGfxBuffer::Unbind() const {
		if (!m_bIsBound) { return; }
		glBindBuffer(m_gbType, 0);
		m_bIsBound = false;
	}
	// --==</AGfxBuf>==--
}
namespace NW
{
	// --==<VertexBuf>==--
	VertexBuf::VertexBuf() : AGfxBuffer(GBT_VERTEX) { }
	VertexBuf::~VertexBuf() { }
	// --==</VertexBuf>==--
}
namespace NW
{
	// --==<IndexBuf>==--
	IndexBuf::IndexBuf() : AGfxBuffer(GBT_INDEX) { }
	IndexBuf::~IndexBuf() { }
	// --==</IndexBuf>==--
}
namespace NW
{
	// --==<ShaderBuf>==--
	ShaderBuf::ShaderBuf() : AGfxBuffer(GBT_SHADER) { }
	ShaderBuf::~ShaderBuf() { }
	// --core_methods
	void ShaderBuf::Bind() const {
		if (m_bIsBound) { return; }
		glBindBuffer(m_gbType, m_unRId);
		m_bIsBound = true;
	}
	void ShaderBuf::Bind(UInt32 unPoint) const {
		glBindBufferBase(GL_UNIFORM_BUFFER, unPoint, m_unRId);
	}
	void ShaderBuf::Bind(UInt32 unPoint, Size szData, Size szOffset) const {
		glBindBufferRange(GL_UNIFORM_BUFFER, unPoint, m_unRId, szOffset, szData);
	}
	void ShaderBuf::Remake(const ShaderBufLayout& rBufLayout) {
		if (m_szData < rBufLayout.GetSize()) { SetData(rBufLayout.GetSize()); }
		for (auto& rBlock : rBufLayout.GetBlocks()) { Bind(rBlock.unBindPoint, rBlock.szAll, rBlock.szOffset); }
	}
	// --==</AShaderBuf>==--
}
namespace NW
{
	VertexArr::VertexArr() :
		m_unRId(0), m_bIsBound(false),
		m_gpType(GPT_TRIANGLES) { glCreateVertexArrays(1, &m_unRId); }
	VertexArr::~VertexArr() { glDeleteVertexArrays(1, &m_unRId); }
	
	// --setters
	// --==<core_methods>==--
	void VertexArr::Bind() const {
		if (m_bIsBound) { return; }
		glBindVertexArray(m_unRId);
		m_bIsBound = true;
	}
	void VertexArr::Unbind() const {
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
#endif	// NW_GAPI

#if (NW_GAPI & NW_GAPI_DX)
namespace NW
{
	// --==<AGfxBuf>==--
	AGfxBuffer::AGfxBuffer(GfxBufferTypes gbType) : m_unRId(0), m_gbType(gbType), m_bIsBound(false), m_szData(0) { }
	AGfxBuffer::~AGfxBuffer() { Unbind(); if (m_unRId != 0) { } }
	// --setters
	void AGfxBuffer::SetSubData(Size szData, const Ptr pVtxData, Size szOffset) {
		Bind();
		Unbind();
	}
	void AGfxBuffer::SetData(Size szData, const Ptr pVtxData) {
		Unbind();
		m_szData = szData;
		if (m_unRId != 0) { }
		if (szData == 0) { return; }
		Bind();
		Unbind();
	}
	void AGfxBuffer::Bind() const {
		if (m_bIsBound) { return; }
		m_bIsBound = true;
	}
	void AGfxBuffer::Unbind() const {
		if (!m_bIsBound) { return; }
		m_bIsBound = false;
	}
	// --==</AGfxBuf>==--
}
namespace NW
{
	// --==<VertexBuf>==--
	VertexBuf::VertexBuf() : AGfxBuffer(GBT_VERTEX) { }
	VertexBuf::~VertexBuf() { }

	// --core_methods
	VertexBuf* VertexBuf::Create() { return GfxApi::Get().NewT<VertexBuf>(); }
	void VertexBuf::Create(RefKeeper<VertexBuf>& rvtxBuf) { rvtxBuf.MakeRef<VertexBuf>(); }
	// --==</VertexBuf>==--
}
namespace NW
{
	// --==<IndexBuf>==--
	IndexBuf::IndexBuf() : AGfxBuffer(GBT_INDEX) { }
	IndexBuf::~IndexBuf() { }

	// --core_methods
	IndexBuf* IndexBuf::Create() { return GfxApi::Get().NewT<IndexBuf>(); }
	void IndexBuf::Create(RefKeeper<IndexBuf>& ridxBuf) { ridxBuf.MakeRef<IndexBuf>(); }
	// --==</IndexBuf>==--
}
namespace NW
{
	// --==<ShaderBuf>==--
	ShaderBuf::ShaderBuf() : AGfxBuffer(GBT_SHADER) { }
	ShaderBuf::~ShaderBuf() { }
	// --core_methods
	void ShaderBuf::Bind() const {
		if (m_bIsBound) { return; }
		m_bIsBound = true;
	}
	void ShaderBuf::Bind(UInt32 unPoint) const {
	}
	void ShaderBuf::Bind(UInt32 unPoint, Size szData, Size szOffset) const {
	}
	void ShaderBuf::Remake(const ShaderBufLayout& rBufLayout) {
		if (m_szData < rBufLayout.GetSize()) { SetData(rBufLayout.GetSize()); }
		for (auto& rBlock : rBufLayout.GetBlocks()) { Bind(rBlock.unBindPoint, rBlock.szAll, rBlock.szOffset); }
	}

	ShaderBuf* ShaderBuf::Create() { return GfxApi::Get().NewT<ShaderBuf>(); }
	void ShaderBuf::Create(RefKeeper<ShaderBuf>& rshdBuf) { rshdBuf.MakeRef<ShaderBuf>(); }
	// --==</AShaderBuf>==--
}
namespace NW
{
	VertexArr::VertexArr() :
		m_unRId(0), m_bIsBound(false),
		m_gpType(PT_TRIANGLES) { }
	VertexArr::~VertexArr() { }

	// --setters
	// --==<core_methods>==--
	void VertexArr::Bind() const {
		if (m_bIsBound) { return; }
		m_bIsBound = true;
	}
	void VertexArr::Unbind() const {
		if (!m_bIsBound) { return; }
		m_bIsBound = false;
	}

	void VertexArr::Remake(const VertexBufLayout& rvtxLayout) {
		NWL_ASSERT(m_vtxBufs.size() > 0, "there are no vertex buffers!");
		Bind();
		Unbind();
	}
	void VertexArr::CreateVtxBuffer()
	{
		RefKeeper<VertexBuf> vtxBuf;
		VertexBuf::Create(vtxBuf);
		AddVtxBuffer(vtxBuf);
	}
	void VertexArr::CreateIdxBuffer()
	{
		RefKeeper<IndexBuf> idxBuf;
		IndexBuf::Create(idxBuf);
		SetIdxBuffer(idxBuf);
	}

	VertexArr* VertexArr::Create() { return GfxApi::Get().NewT<VertexArr>(); }
	void VertexArr::Create(RefKeeper<VertexArr>& rvtxArr) { rvtxArr.MakeRef<VertexArr>(); }
	// --==</core_methods>==--
}
#endif // NW_GAPI