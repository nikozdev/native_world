#ifndef NW_GFX_BUFFER_H
#define NW_GFX_BUFFER_H

#include <gfx/gfx_tools.h>

#if (defined NW_GAPI)
namespace NW
{
	/// VertexBuffer Class
	class NW_API VertexBuf
	{
		friend class VertexArr;
	public:
		VertexBuf();
		virtual ~VertexBuf();
		// --getters
		inline UInt32 GetRenderId() const { return m_unRId; }
		inline Size GetDataSize() const { return m_szData; }
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) = 0;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0) = 0;
		// --predicates
		inline Bit IsBound() const { return m_bIsBound; }
		// --core_methods
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	protected:
		UInt32 m_unRId;
		mutable Bit m_bIsBound;
		Size m_szData;
	};
	/// Abstract IndexBuffer Class
	class NW_API IndexBuf
	{
	public:
		IndexBuf();
		virtual ~IndexBuf();
		// --getters
		inline UInt32 GetRenderId() const { return m_unRId; }
		inline Size GetDataSize() const { return m_szData; }
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) = 0;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0) = 0;
		// --predicates
		inline Bit IsBound() const { return m_bIsBound; }
		// --core_methods
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	protected:
		UInt32 m_unRId;
		mutable Bit m_bIsBound;
		Size m_szData;
	};
	/// Abstract ShaderBuffer class
	/// Description:
	/// -- Is used by shaders as opengl uniform setter, or as directx constant buffer
	class NW_API ShaderBuf
	{
	public:
		ShaderBuf();
		virtual ~ShaderBuf();
		// --getters
		inline UInt32 GetRenderId() const { return m_unRId; }
		inline Size GetDataSize() const { return m_szData; }
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) = 0;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0) = 0;
		// --predicates
		inline Bit IsBound() const { return m_bIsBound; }
		// --core_methods
		virtual void Bind() const = 0;
		virtual void Bind(UInt32 unPoint) const = 0;
		virtual void Bind(UInt32 unPoint, Size szData, Size szOffset = 0) const = 0;
		virtual void Unbind() const = 0;
		virtual void Remake(const ShaderBufLayout& rShdLayout) = 0;
	protected:
		UInt32 m_unRId;
		mutable Bit m_bIsBound;
		Size m_szData;
	};
}
#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	/// VertexBuffer Class
	class NW_API VertexBufOgl : public VertexBuf
	{
		friend class GfxEngine;
		friend class VertexArrOgl;
	public:
		VertexBufOgl();
		virtual ~VertexBufOgl();
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) override;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0) override;
		// --core_methods
		virtual void Bind() const override;
		virtual void Unbind() const override;
	protected:
		UInt32 m_unRId;
	};
	/// Abstract IndexBuffer Class
	class NW_API IndexBufOgl : public IndexBuf
	{
		friend class GfxEngine;
	public:
		IndexBufOgl();
		virtual ~IndexBufOgl();
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) override;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0) override;
		// --core_methods
		virtual void Bind() const override;
		virtual void Unbind() const override;
	};
	/// Abstract ShaderBuffer class
	/// Description:
	/// -- Is used by shaders as opengl uniform setter, or as directx constant buffer
	class NW_API ShaderBufOgl : public ShaderBuf
	{
	public:
		ShaderBufOgl();
		virtual ~ShaderBufOgl();
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) override;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0) override;
		// --core_methods
		virtual void Bind() const override;
		virtual void Bind(UInt32 unPoint) const override;
		virtual void Bind(UInt32 unPoint, Size szData, Size szOffset = 0) const override;
		virtual void Unbind() const override;
		virtual void Remake(const ShaderBufLayout& rShdLayout) override;
	};
}
#endif
#if (NW_GAPI & NW_GAPI_DX)
namespace NW
{
	/// VertexBuffer Class
	class NW_API VertexBufDx : public VertexBuf
	{
		friend class GfxEngine;
		friend class VertexArrDx;
	public:
		VertexBufDx();
		virtual ~VertexBufDx();
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) override;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0) override;
		// --core_methods
		virtual void Bind() const override;
		virtual void Unbind() const override;
	protected:
		UInt32 m_unRId;
	};
	/// Abstract IndexBuffer Class
	class NW_API IndexBufDx : public IndexBuf
	{
		friend class GfxEngine;
	public:
		IndexBufDx();
		virtual ~IndexBufDx();
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) override;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0) override;
		// --core_methods
		virtual void Bind() const override;
		virtual void Unbind() const override;
	};
	/// Abstract ShaderBuffer class
	/// Description:
	/// -- Is used by shaders as opengl uniform setter, or as directx constant buffer
	class NW_API ShaderBufDx : public ShaderBuf
	{
		friend class GfxEngine;
	public:
		ShaderBufDx();
		virtual ~ShaderBufDx();
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) override;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0u) override;
		// --core_methods
		virtual void Bind() const override;
		virtual void Bind(UInt32 unPoint) const override;
		virtual void Bind(UInt32 unPoint, Size szData, Size szOffset = 0u) const override;
		virtual void Unbind() const override;
		virtual void Remake(const ShaderBufLayout& rShdLayout) override;
	};
}
#endif
#endif	// NW_GAPI
#endif	// NW_GFX_BUFFER_H