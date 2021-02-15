#include <nw_pch.hpp>
#include "gfx/gfx_buffer.h"
#pragma warning(disable : 4312)
#if (defined NW_GAPI)
#include <gfx/gfx_engine.h>
#include <gfx/gfx_loader.h>
namespace NW
{
	VertexBuf::VertexBuf() : m_szData(0) { }
	VertexBuf::~VertexBuf() {}
	IndexBuf::IndexBuf() : m_szData(0) { }
	IndexBuf::~IndexBuf() { }
	ShaderBuf::ShaderBuf() : m_szData(0) { }
	ShaderBuf::~ShaderBuf() { }
}
#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	VertexBufOgl::VertexBufOgl(GfxEngineOgl& rGfx) : VertexBuf(), GfxEntityOgl(rGfx) { }
	VertexBufOgl::~VertexBufOgl() { SetData(0, nullptr); }
	// --setters
	void VertexBufOgl::SetSubData(Size szData, const Ptr pVtxData, Size szOffset) {
		glBufferSubData(GL_ARRAY_BUFFER, szOffset, szData, pVtxData);
	}
	void VertexBufOgl::SetData(Size szData, const Ptr pVtxData) {
		m_szData = szData;
		if (m_unRId != 0) { glDeleteBuffers(1, &m_unRId); m_unRId = 0; }
		if (szData == 0) { return; }
		glGenBuffers(1, &m_unRId);
		glBindBuffer(GL_ARRAY_BUFFER, m_unRId);
		glBufferData(GL_ARRAY_BUFFER, szData, pVtxData, pVtxData == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}
	void VertexBufOgl::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_unRId);
	}
	
	IndexBufOgl::IndexBufOgl(GfxEngineOgl& rGfx) : IndexBuf(), GfxEntityOgl(rGfx) { }
	IndexBufOgl::~IndexBufOgl() { SetData(0, nullptr); }
	// --setters
	void IndexBufOgl::SetSubData(Size szData, const Ptr pVtxData, Size szOffset) {
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, szOffset, szData, pVtxData);
	}
	void IndexBufOgl::SetData(Size szData, const Ptr pVtxData) {
		m_szData = szData;
		if (m_unRId != 0) { glDeleteBuffers(1, &m_unRId); m_unRId = 0; }
		if (szData == 0) { return; }
		glGenBuffers(1, &m_unRId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unRId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, szData, pVtxData, pVtxData == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}
	void IndexBufOgl::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unRId);
	}

	ShaderBufOgl::ShaderBufOgl(GfxEngineOgl& rGfx) : ShaderBuf(), GfxEntityOgl(rGfx) { }
	ShaderBufOgl::~ShaderBufOgl() { SetData(0, nullptr); }
	// --setters
	void ShaderBufOgl::SetSubData(Size szData, const Ptr pVtxData, Size szOffset) {
		glBufferSubData(GL_UNIFORM_BUFFER, szOffset, szData, pVtxData);
	}
	void ShaderBufOgl::SetData(Size szData, const Ptr pVtxData) {
		m_szData = szData;
		if (m_unRId != 0) { glDeleteBuffers(1, &m_unRId); m_unRId = 0; }
		if (szData == 0) { return; }
		glGenBuffers(1, &m_unRId);
		glBindBuffer(GL_UNIFORM_BUFFER, m_unRId);
		glBufferData(GL_UNIFORM_BUFFER, szData, pVtxData, pVtxData == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}
	// --core_methods
	void ShaderBufOgl::Bind() const {
		glBindBuffer(GL_UNIFORM_BUFFER, m_unRId);
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
	VertexBufDx::VertexBufDx(GfxEngineDx& rGfx) : VertexBuf(), GfxEntityDx(rGfx) { }
	VertexBufDx::~VertexBufDx() { SetData(0, nullptr); }
	// --setters
	void VertexBufDx::SetSubData(Size szData, const Ptr pVtxData, Size szOffset) {
	}
	void VertexBufDx::SetData(Size szData, const Ptr pVtxData) {
	}
	// --core_methods
	void VertexBufDx::Bind() const {
	}
	
	IndexBufDx::IndexBufDx(GfxEngineDx& rGfx) : IndexBuf(), GfxEntityDx(rGfx) { }
	IndexBufDx::~IndexBufDx() { SetData(0, nullptr); }
	// --setters
	void IndexBufDx::SetSubData(Size szData, const Ptr pVtxData, Size szOffset) {
	}
	void IndexBufDx::SetData(Size szData, const Ptr pVtxData) {
	}
	// --core_methods
	void IndexBufDx::Bind() const {
	}

	ShaderBufDx::ShaderBufDx(GfxEngineDx& rGfx) : ShaderBuf(), GfxEntityDx(rGfx) { }
	ShaderBufDx::~ShaderBufDx() { SetData(0, nullptr); }
	// --setters
	void ShaderBufDx::SetSubData(Size szData, const Ptr pVtxData, Size szOffset) {
	}
	void ShaderBufDx::SetData(Size szData, const Ptr pVtxData) {
	}
	// --core_methods
	void ShaderBufDx::Bind() const {
	}
	void ShaderBufDx::Bind(UInt32 unPoint) const { }
	void ShaderBufDx::Bind(UInt32 unPoint, Size szData, Size szOffset) const { }
	void ShaderBufDx::Remake(const ShaderBufLayout& rBufLayout) {
		if (m_szData < rBufLayout.GetSize()) { SetData(rBufLayout.GetSize()); }
		for (auto& rBlock : rBufLayout.GetBlocks()) { Bind(rBlock.unBindPoint, rBlock.szAll, rBlock.szOffset); }
	}
}
#endif
#endif	// NW_GAPI