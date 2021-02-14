#include <nw_pch.hpp>
#include "gfx/gfx_buffer.h"

#pragma warning(disable : 4312)

#if (defined NW_GAPI)
#include <gfx/gfx_loader.h>
namespace NW
{
	VertexBuf::VertexBuf() :
		m_unRId(0), m_bIsBound(false), m_szData(0) { }
	VertexBuf::~VertexBuf() {}
	IndexBuf::IndexBuf() :
		m_unRId(0), m_bIsBound(false), m_szData(0) { }
	IndexBuf::~IndexBuf() { }
	ShaderBuf::ShaderBuf() :
		m_unRId(0), m_bIsBound(false), m_szData(0) { }
	ShaderBuf::~ShaderBuf() { }
}
#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	VertexBufOgl::VertexBufOgl() : VertexBuf() { }
	VertexBufOgl::~VertexBufOgl() { }
	// --setters
	void VertexBufOgl::SetSubData(Size szData, const Ptr pVtxData, Size szOffset) {
		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, szOffset, szData, pVtxData);
		Unbind();
	}
	void VertexBufOgl::SetData(Size szData, const Ptr pVtxData) {
		Unbind();
		m_szData = szData;
		if (m_unRId != 0) { glDeleteBuffers(1, &m_unRId); m_unRId = 0; }
		if (szData == 0) { return; }
		glGenBuffers(1, &m_unRId);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, szData, pVtxData, pVtxData == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		Unbind();
	}
	void VertexBufOgl::Bind() const {
		if (m_bIsBound) { return; }
		glBindBuffer(GL_ARRAY_BUFFER, m_unRId);
		m_bIsBound = true;
	}
	void VertexBufOgl::Unbind() const {
		if (!m_bIsBound) { return; }
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_bIsBound = false;
	}
	void ShaderBufOgl::ShaderBufOgl() : ShaderBuf() { }
	void ShaderBufOgl::~ShaderBufOgl() { }
	// --core_methods
	void ShaderBufOgl::Bind() const {
		if (m_bIsBound) { return; }
		glBindBuffer(GL_UNIFORM_BUFFER, m_unRId);
		m_bIsBound = true;
	}
	void ShaderBufOgl::Bind(UInt32 unPoint) const {
		glBindBufferBase(GL_UNIFORM_BUFFER, unPoint, m_unRId);
	}
	void ShaderBufOgl::Bind(UInt32 unPoint, Size szData, Size szOffset) const {
		glBindBufferRange(GL_UNIFORM_BUFFER, unPoint, m_unRId, szOffset, szData);
	}
	void ShaderBufOgl::Remake(const ShaderBufLayout& rBufLayout) {
		if (m_szData < rBufLayout.GetSize()) { SetData(rBufLayout.GetSize()); }
		for (auto& rBlock : rBufLayout.GetBlocks()) { Bind(rBlock.unBindPoint, rBlock.szAll, rBlock.szOffset); }
	}
}
#endif
#if (NW_GAPI & NW_GAPI_DX)
namespace NW
{
	VertexBufDx::VertexBufDx() : VertexBuf() { }
	VertexBufDx::~VertexBufDx() { }
	// --setters
	void VertexBufDx::SetSubData(Size szData, const Ptr pVtxData, Size szOffset) {
		Bind();
		Unbind();
	}
	void VertexBufDx::SetData(Size szData, const Ptr pVtxData) {
		Unbind();
		m_szData = szData;
		if (m_unRId != 0) { }
		if (szData == 0) { return; }
		Bind();
		Unbind();
	}
	void VertexBufDx::Bind() const {
		if (m_bIsBound) { return; }
		m_bIsBound = true;
	}
	void VertexBufDx::Unbind() const {
		if (!m_bIsBound) { return; }
		m_bIsBound = false;
	}
	
	IndexBufDx::IndexBufDx() : IndexBuf() { }
	IndexBufDx::~IndexBufDx() { }
	// --setters
	void IndexBufDx::SetSubData(Size szData, const Ptr pVtxData, Size szOffset) {
		Bind();
		Unbind();
	}
	void IndexBufDx::SetData(Size szData, const Ptr pVtxData) {
		Unbind();
		m_szData = szData;
		if (m_unRId != 0) {}
		if (szData == 0) { return; }
		Bind();
		Unbind();
	}
	// --core_methods
	void IndexBufDx::Bind() const {
		if (m_bIsBound) { return; }
		m_bIsBound = true;
	}
	void IndexBufDx::Unbind() const { }

	ShaderBufDx::ShaderBufDx() : ShaderBuf() { }
	ShaderBufDx::~ShaderBufDx() { }
	// --setters
	void ShaderBufDx::SetSubData(Size szData, const Ptr pVtxData, Size szOffset) {
		Bind();
		Unbind();
	}
	void ShaderBufDx::SetData(Size szData, const Ptr pVtxData) {
		Unbind();
		m_szData = szData;
		if (m_unRId != 0) {}
		if (szData == 0) { return; }
		Bind();
		Unbind();
	}
	// --core_methods
	void ShaderBufDx::Bind() const {
		if (m_bIsBound) { return; }
		m_bIsBound = true;
	}
	void ShaderBufDx::Bind(UInt32 unPoint) const { }
	void ShaderBufDx::Bind(UInt32 unPoint, Size szData, Size szOffset) const { }
	void ShaderBufDx::Unbind() const { }
	void ShaderBufDx::Remake(const ShaderBufLayout& rBufLayout) {
		if (m_szData < rBufLayout.GetSize()) { SetData(rBufLayout.GetSize()); }
		for (auto& rBlock : rBufLayout.GetBlocks()) { Bind(rBlock.unBindPoint, rBlock.szAll, rBlock.szOffset); }
	}
}
#endif
#endif	// NW_GAPI