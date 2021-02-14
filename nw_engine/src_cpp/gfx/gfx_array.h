#ifndef NW_GFX_ARRAY_H
#define NW_GFX_ARRAY_H
#include <gfx_core.hpp>
#if (defined NW_GAPI)
#include <gfx/gfx_buffer.h>
namespace NW
{
	/// VertexArray class
	class VertexArr
	{
		friend class GfxEngine;
	public:
		VertexArr();
		virtual ~VertexArr();
		// --getters
		inline UInt32 GetRenderId() const { return m_unRId; }
		inline DArray<RefKeeper<VertexBuf>>& GetVtxBuffers() { return m_vtxBufs; }
		inline VertexBuf* GetVtxBuffer(UInt32 unIdx = 0) { return m_vtxBufs[unIdx].GetRef(); }
		inline IndexBuf* GetIdxBuffer() { return m_idxBuf.GetRef(); }
		inline VertexBufLayout& GetLayout() { return m_vtxLayout; }
		inline GfxPrimitiveTypes GetDrawPrimitive() const { return m_gpType; }
		// --setters
		inline void AddVtxBuffer(RefKeeper<VertexBuf>& rvtxBuf) { m_vtxBufs.push_back(rvtxBuf); }
		inline void RmvVtxBuffer(UInt32 unIdx) { m_vtxBufs.erase(m_vtxBufs.begin() + unIdx); }
		inline void SetIdxBuffer(RefKeeper<IndexBuf>& ridxBuf) { m_idxBuf.SetRef(ridxBuf); }
		inline void SetDrawPrimitive(GfxPrimitiveTypes gpType) { m_gpType = gpType; }
		// --predicates
		inline Bit IsBound() { return m_bIsBound; }
		// --core_methods
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Remake(const VertexBufLayout& rvtxLayout) = 0;
		virtual void CreateVtxBuffer() = 0;
		virtual void CreateIdxBuffer() = 0;
	protected:
		UInt32 m_unRId;
		mutable Bit m_bIsBound;
		DArray<RefKeeper<VertexBuf>> m_vtxBufs;
		RefKeeper<IndexBuf> m_idxBuf;
		VertexBufLayout m_vtxLayout;
		GfxPrimitiveTypes m_gpType;
	};
}
#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	/// VertexArray class
	class VertexArrOgl : public VertexArr
	{
		friend class GfxEngine;
	public:
		VertexArrOgl();
		virtual ~VertexArrOgl();
		// --core_methods
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Remake(const VertexBufLayout& rVtxLayout) override;
		virtual void CreateVtxBuffer() override;
		virtual void CreateIdxBuffer() override;
	};
}
#endif
#if (NW_GAPI & NW_GAPI_DX)
namespace NW
{
	/// VertexArray class
	class VertexArrDx : public VertexArr
	{
		friend class GfxEngine;
	public:
		VertexArrDx();
		virtual ~VertexArrDx();
		// --core_methods
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Remake(const VertexBufLayout& rVtxLayout) override;
		virtual void CreateVtxBuffer() override;
		virtual void CreateIdxBuffer() override;
	};
}
#endif
#endif	// NW_GAPI
#endif	// NW_GFX_ARRAY_H