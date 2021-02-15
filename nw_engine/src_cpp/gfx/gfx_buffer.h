#ifndef NWG_BUFFER_H
#define NWG_BUFFER_H

#include <gfx/gfx_entity.h>
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
		inline Size GetDataSize() const { return m_szData; }
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) = 0;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0) = 0;
	protected:
		Size m_szData;
	};
	/// Abstract IndexBuffer Class
	class NW_API IndexBuf
	{
	public:
		IndexBuf();
		virtual ~IndexBuf();
		// --getters
		inline Size GetDataSize() const { return m_szData; }
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) = 0;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0) = 0;
	protected:
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
		inline Size GetDataSize() const { return m_szData; }
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) = 0;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0) = 0;
		// --core_methods
		virtual void Bind(UInt32 unPoint) const = 0;
		virtual void Bind(UInt32 unPoint, Size szData, Size szOffset = 0) const = 0;
		virtual void Remake(const ShaderBufLayout& rShdLayout) = 0;
	protected:
		Size m_szData;
	};
}
#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	/// VertexBuffer Class
	class NW_API VertexBufOgl : public VertexBuf, public GfxEntityOgl
	{
		friend class VertexArrOgl;
	public:
		VertexBufOgl(GfxEngineOgl& rGfx);
		virtual ~VertexBufOgl();
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) override;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0) override;
		// --core_methods
		virtual void Bind() const override;
	};
	/// Abstract IndexBuffer Class
	class NW_API IndexBufOgl : public IndexBuf, public GfxEntityOgl
	{
		friend class GfxEngine;
	public:
		IndexBufOgl(GfxEngineOgl& rGfx);
		virtual ~IndexBufOgl();
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) override;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0) override;
		// --core_methods
		virtual void Bind() const override;
	};
	/// Abstract ShaderBuffer class
	/// Description:
	/// -- Is used by shaders as opengl uniform setter, or as directx constant buffer
	class NW_API ShaderBufOgl : public ShaderBuf, public GfxEntityOgl
	{
	public:
		ShaderBufOgl(GfxEngineOgl& rGfx);
		virtual ~ShaderBufOgl();
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) override;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0) override;
		// --core_methods
		virtual void Bind() const override;
		virtual void Bind(UInt32 unPoint) const override;
		virtual void Bind(UInt32 unPoint, Size szData, Size szOffset = 0) const override;
		virtual void Remake(const ShaderBufLayout& rShdLayout) override;
	};
}
#endif
#if (NW_GAPI & NW_GAPI_DX)
namespace NW
{
	/// VertexBuffer Class
	class NW_API VertexBufDx : public VertexBuf, public GfxEntityDx
	{
		friend class VertexArrDx;
	public:
		VertexBufDx(GfxEngineDx& rGfx);
		virtual ~VertexBufDx();
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) override;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0) override;
		// --core_methods
		virtual void Bind() const override;
	};
	/// Abstract IndexBuffer Class
	class NW_API IndexBufDx : public IndexBuf, public GfxEntityDx
	{
	public:
		IndexBufDx(GfxEngineDx& rGfx);
		virtual ~IndexBufDx();
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) override;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0) override;
		// --core_methods
		virtual void Bind() const override;
	};
	/// Abstract ShaderBuffer class
	/// Description:
	/// -- Is used by shaders as opengl uniform setter, or as directx constant buffer
	class NW_API ShaderBufDx : public ShaderBuf, public GfxEntityDx
	{
	public:
		ShaderBufDx(GfxEngineDx& rGfx);
		virtual ~ShaderBufDx();
		// --setters
		virtual void SetData(Size szData, const Ptr pVtxData = nullptr) override;
		virtual void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0u) override;
		// --core_methods
		virtual void Bind() const override;
		virtual void Bind(UInt32 unPoint) const override;
		virtual void Bind(UInt32 unPoint, Size szData, Size szOffset = 0u) const override;
		virtual void Remake(const ShaderBufLayout& rShdLayout) override;
	};
}
#endif
#endif	// NW_GAPI
#endif	// NWG_BUFFER_H