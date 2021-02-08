#include <gfx_pch.hpp>
#include "gfx_buffer.h"

#pragma warning(disable : 4312)

#if (defined GFX_GAPI)
#include <gfx_engine.h>
#include <gfx_loader.h>
#endif	// GFX_API

#if (GFX_GAPI & GFX_GAPI_OGL)
namespace NW
{
	// --==<AGBuf>==--
	AGBuffer::AGBuffer(GBufferTypes gbType) : m_unRId(0), m_gbType(gbType), m_bIsBound(false), m_szData(0) { }
	AGBuffer::~AGBuffer() { Unbind(); if (m_unRId != 0) { glDeleteBuffers(1, &m_unRId); } }
	// --setters
	void AGBuffer::SetSubData(Size szData, const Ptr pVtxData, Size szOffset) {
		Bind();
		glBufferSubData(m_gbType, szOffset, szData, pVtxData);
		Unbind();
	}
	void AGBuffer::SetData(Size szData, const Ptr pVtxData) {
		Unbind();
		m_szData = szData;
		if (m_unRId != 0) { glDeleteBuffers(1, &m_unRId); m_unRId = 0; }
		if (szData == 0) { return; }
		glGenBuffers(1, &m_unRId);
		Bind();
		glBufferData(m_gbType, szData, pVtxData, pVtxData == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		Unbind();
	}
	void AGBuffer::Bind() const {
		if (m_bIsBound) { return; }
		glBindBuffer(m_gbType, m_unRId);
		m_bIsBound = true;
	}
	void AGBuffer::Unbind() const {
		if (!m_bIsBound) { return; }
		glBindBuffer(m_gbType, 0);
		m_bIsBound = false;
	}
	// --==</AGBuf>==--
}
namespace NW
{
	// --==<VertexBuf>==--
	VertexBuf::VertexBuf() : AGBuffer(GBT_VERTEX) { }
	VertexBuf::~VertexBuf() { }

	// --core_methods
	VertexBuf* VertexBuf::Create() { return GfxEngine::Get().NewT<VertexBuf>(); }
	void VertexBuf::Create(RefKeeper<VertexBuf>& rvtxBuf) { rvtxBuf.MakeRef<VertexBuf>(GfxEngine::Get().GetMemory()); }
	// --==</VertexBuf>==--
}
namespace NW
{
	// --==<IndexBuf>==--
	IndexBuf::IndexBuf() : AGBuffer(GBT_INDEX) { }
	IndexBuf::~IndexBuf() { }

	// --core_methods
	IndexBuf* IndexBuf::Create() { return GfxEngine::Get().NewT<IndexBuf>(); }
	void IndexBuf::Create(RefKeeper<IndexBuf>& ridxBuf) { ridxBuf.MakeRef<IndexBuf>(GfxEngine::Get().GetMemory()); }
	// --==</IndexBuf>==--
}
namespace NW
{
	// --==<ShaderBuf>==--
	ShaderBuf::ShaderBuf() : AGBuffer(GBT_SHADER) { }
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

	ShaderBuf* ShaderBuf::Create() { return GfxEngine::Get().NewT<ShaderBuf>(); }
	void ShaderBuf::Create(RefKeeper<ShaderBuf>& rshdBuf) { rshdBuf.MakeRef<ShaderBuf>(GfxEngine::Get().GetMemory()); }
	// --==</AShaderBuf>==--
}
namespace NW
{
	VertexArr::VertexArr() :
		m_unRId(0), m_bIsBound(false),
		m_gpType(PT_TRIANGLES) { glCreateVertexArrays(1, &m_unRId); }
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
		NW_ASSERT(m_vtxBufs.size() > 0, "there are no vertex buffers!");
		Bind();
		if (m_idxBuf.GetRef() != nullptr) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxBuf->GetRenderId()); }
		else { glBindBuffer(GBT_INDEX, 0); }
		for (auto& itBuf : m_vtxBufs) {
			NW_ASSERT(itBuf->GetRenderId() != 0, "vertex buffer is invalid!");
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
		VertexBuf::Create(vtxBuf);
		AddVtxBuffer(vtxBuf);
	}
	void VertexArr::CreateIdxBuffer()
	{
		RefKeeper<IndexBuf> idxBuf;
		IndexBuf::Create(idxBuf);
		SetIdxBuffer(idxBuf);
	}

	VertexArr* VertexArr::Create() { return GfxEngine::Get().NewT<VertexArr>(); }
	void VertexArr::Create(RefKeeper<VertexArr>& rvtxArr) { rvtxArr.MakeRef<VertexArr>(GfxEngine::Get().GetMemory()); }
	// --==</core_methods>==--
}
#endif	// GFX_GAPI

#if (GFX_GAPI & GFX_GAPI_DX)
namespace NW
{
	// --==<AGBuf>==--
	AGBuffer::AGBuffer(GBufferTypes gbType) : m_unRId(0), m_gbType(gbType), m_bIsBound(false), m_szData(0) { }
	AGBuffer::~AGBuffer() { Unbind(); if (m_unRId != 0) { } }
	// --setters
	void AGBuffer::SetSubData(Size szData, const Ptr pVtxData, Size szOffset) {
		Bind();
		Unbind();
	}
	void AGBuffer::SetData(Size szData, const Ptr pVtxData) {
		Unbind();
		m_szData = szData;
		if (m_unRId != 0) { }
		if (szData == 0) { return; }
		Bind();
		Unbind();
	}
	void AGBuffer::Bind() const {
		if (m_bIsBound) { return; }
		m_bIsBound = true;
	}
	void AGBuffer::Unbind() const {
		if (!m_bIsBound) { return; }
		m_bIsBound = false;
	}
	// --==</AGBuf>==--
}
namespace NW
{
	// --==<VertexBuf>==--
	VertexBuf::VertexBuf() : AGBuffer(GBT_VERTEX) { }
	VertexBuf::~VertexBuf() { }

	// --core_methods
	VertexBuf* VertexBuf::Create() { return GfxEngine::Get().NewT<VertexBuf>(); }
	void VertexBuf::Create(RefKeeper<VertexBuf>& rvtxBuf) { rvtxBuf.MakeRef<VertexBuf>(GfxEngine::Get().GetMemory()); }
	// --==</VertexBuf>==--
}
namespace NW
{
	// --==<IndexBuf>==--
	IndexBuf::IndexBuf() : AGBuffer(GBT_INDEX) { }
	IndexBuf::~IndexBuf() { }

	// --core_methods
	IndexBuf* IndexBuf::Create() { return GfxEngine::Get().NewT<IndexBuf>(); }
	void IndexBuf::Create(RefKeeper<IndexBuf>& ridxBuf) { ridxBuf.MakeRef<IndexBuf>(GfxEngine::Get().GetMemory()); }
	// --==</IndexBuf>==--
}
namespace NW
{
	// --==<ShaderBuf>==--
	ShaderBuf::ShaderBuf() : AGBuffer(GBT_SHADER) { }
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

	ShaderBuf* ShaderBuf::Create() { return GfxEngine::Get().NewT<ShaderBuf>(); }
	void ShaderBuf::Create(RefKeeper<ShaderBuf>& rshdBuf) { rshdBuf.MakeRef<ShaderBuf>(GfxEngine::Get().GetMemory()); }
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
		NW_ASSERT(m_vtxBufs.size() > 0, "there are no vertex buffers!");
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

	VertexArr* VertexArr::Create() { return GfxEngine::Get().NewT<VertexArr>(); }
	void VertexArr::Create(RefKeeper<VertexArr>& rvtxArr) { rvtxArr.MakeRef<VertexArr>(GfxEngine::Get().GetMemory()); }
	// --==</core_methods>==--
}
#endif // GFX_GAPI