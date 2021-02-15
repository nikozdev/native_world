#ifndef NWG_ARRAY_H
#define NWG_ARRAY_H
#include <gfx_core.hpp>
#if (defined NW_GAPI)
#include <gfx/gfx_buffer.h>
namespace NW
{
	/// VertexArray class
	class VertexArr
	{
	public:
		VertexArr();
		virtual ~VertexArr();
		// --getters
		inline VertexBufLayout& GetLayout() { return m_vtxLayout; }
		inline GfxPrimitives GetDrawPrimitive() const { return m_gpType; }
		virtual inline VertexBuf* GetVtxBuffer(UInt32 unIdx = 0) = 0;
		virtual inline IndexBuf* GetIdxBuffer() = 0;
		// --setters
		inline void SetDrawPrimitive(GfxPrimitives gpType) { m_gpType = gpType; }
		// --core_methods
		virtual void Remake(const VertexBufLayout& rvtxLayout) = 0;
		virtual void CreateVtxBuffer() = 0;
		virtual void CreateIdxBuffer() = 0;
	protected:
		VertexBufLayout m_vtxLayout;
		GfxPrimitives m_gpType;
	};
}
#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	/// VertexArray class
	class VertexArrOgl : public VertexArr, public GfxEntityOgl
	{
	public:
		VertexArrOgl(GfxEngineOgl& rGfx);
		virtual ~VertexArrOgl();
		// --getters
		virtual inline VertexBuf* GetVtxBuffer(UInt32 unIdx = 0) override { return m_vtxBufs[unIdx].GetRef(); }
		virtual inline IndexBuf* GetIdxBuffer() override { return m_idxBuf.GetRef(); }
		// --core_methods
		virtual void Bind() const override;
		virtual void Remake(const VertexBufLayout& rVtxLayout) override;
		virtual void CreateVtxBuffer() override;
		virtual void CreateIdxBuffer() override;
	private:
		DArray<RefKeeper<VertexBufOgl>> m_vtxBufs;
		RefKeeper<IndexBufOgl> m_idxBuf;
	};
}
#endif
#if (NW_GAPI & NW_GAPI_DX)
namespace NW
{
	/// VertexArray class
	class VertexArrDx : public VertexArr, public AGfxEntityDx
	{
	public:
		VertexArrDx(GfxEngineDx& rGfx);
		virtual ~VertexArrDx();
		// --getters
		virtual inline VertexBuf* GetVtxBuffer(UInt32 unIdx = 0) override { return m_vtxBufs[unIdx].GetRef(); }
		virtual inline IndexBuf* GetIdxBuffer() override { return m_idxBuf.GetRef(); }
		// --core_methods
		virtual void Bind() const override;
		virtual void Remake(const VertexBufLayout& rVtxLayout) override;
		virtual void CreateVtxBuffer() override;
		virtual void CreateIdxBuffer() override;
	private:
		DArray<RefKeeper<VertexBufDx>> m_vtxBufs;
		RefKeeper<IndexBufDx> m_idxBuf;
	};
}
#endif
#endif	// NW_GAPI
#endif	// NWG_ARRAY_H