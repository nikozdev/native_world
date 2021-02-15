#ifndef NWG_FRAME_BUFFER_H
#define NWG_FRAME_BUFFER_H
#include <gfx_core.hpp>
#include <gfx/gfx_entity.h>
#include <gfx/gfx_texture.h>
#if (defined NW_GAPI)
namespace NW
{
	/// FrameBufInfo struct
	struct NW_API FrameBufInfo
	{
	public:
		V4i rectViewport = { 0, 0, 1, 1 };
		Bit bSwapChain = false;
		Bit bResizable = true;
		UInt32 unColorCount = 0;
		Bit bHasDepth = false;
		Bit bHasStencil = false;

	public:
		inline Int32 GetWidth() const { return { rectViewport.z - rectViewport.x }; }
		inline Int32 GetHeight() const { return { rectViewport.w - rectViewport.y }; }
	};
}
namespace NW
{
	/// FrameBuffer class
	class NW_API FrameBuf : public TDataRes<FrameBuf>
	{
	public:
		FrameBuf(const char* strName, const FrameBufInfo& rFbInfo);
		virtual ~FrameBuf();
		// --getters
		inline Int32 GetWidth() const { return m_Info.GetWidth(); }
		inline Int32 GetHeight() const { return m_Info.GetHeight(); }
		inline const V4i& GetViewport() const { return m_Info.rectViewport; }
		inline const V4f GetClearColor() const { return m_rgbaClear; }
		inline const FrameBufInfo& GetInfo() const { return m_Info; }
		virtual inline Texture* GetAttachment(UInt32 unIdx = 0) = 0;
		// --setters
		void SetViewport(V4i rectViewport);
		void SetClearColor(V4f rgbaClear);
		virtual void AttachTexture(Texture& rTex) = 0;
		virtual void DetachTexture(UInt32 unIdx) = 0;
		// --core_methods
		virtual void Remake() = 0;
		virtual void Clear(UInt32 bitMask = FB_COLOR | FB_DEPTH | FB_STENCIL) = 0;
		virtual void ReadPixels(Ptr pData, UInt32 unAttachIdx, Int32 nX, Int32 nY, Int32 nWidth = 1, Int32 nHeight = 1) = 0;
		virtual void WritePixels(Ptr pData, UInt32 unAttachIdx, Int32 nX, Int32 nY, Int32 nWidth = 1, Int32 nHeight = 1) = 0;
		// --data_methods
		virtual bool SaveF(const char* strFPath) override { return true; }
		virtual bool LoadF(const char* strFPath) override { return true; }
	protected:
		FrameBufInfo m_Info;
		V4f m_rgbaClear;
	};
}
#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	/// FrameBufferOgl class
	class NW_API FrameBufOgl : public FrameBuf, public GfxEntityOgl
	{
	public:
		FrameBufOgl(const char* strName, const FrameBufInfo& rFbInfo, GfxEngineOgl& rGfx);
		virtual ~FrameBufOgl();
		// --getters
		virtual inline Texture* GetAttachment(UInt32 unIdx = 0) override { return static_cast<Texture*>(m_Attachments[unIdx]); }
		// --setters
		virtual void AttachTexture(Texture& rTex) override;
		virtual void DetachTexture(UInt32 unIdx) override;
		// --core_methods
		virtual void Bind() const override;
		virtual void Remake() override;
		virtual void Clear(UInt32 bitMask = FB_COLOR | FB_DEPTH | FB_STENCIL) override;
		virtual void ReadPixels(Ptr pData, UInt32 unAttachIdx, Int32 nX, Int32 nY, Int32 nWidth = 1, Int32 nHeight = 1) override;
		void WritePixels(Ptr pData, UInt32 unAttachIdx, Int32 nX, Int32 nY, Int32 nWidth = 1, Int32 nHeight = 1) override;
	private:
		DArray<TextureOgl*> m_Attachments;
	};
}
#endif
#if (NW_GAPI & NW_GAPI_DX)
namespace NW
{
	/// FrameBufferOgl class
	class NW_API FrameBufDx : public FrameBuf, public GfxEntityDx
	{
	public:
		FrameBufDx(const char* strName, const FrameBufInfo& rFbInfo, GfxEngineDx& rGfx);
		virtual ~FrameBufDx();
		// --core_methods
		virtual void Bind() const override;
		virtual void Remake() override;
		virtual void Clear(UInt32 bitMask = FB_COLOR | FB_DEPTH | FB_STENCIL) override;
		virtual void ReadPixels(Ptr pData, UInt32 unAttachIdx, Int32 nX, Int32 nY, Int32 nWidth = 1, Int32 nHeight = 1) override;
		void WritePixels(Ptr pData, UInt32 unAttachIdx, Int32 nX, Int32 nY, Int32 nWidth = 1, Int32 nHeight = 1) override;
	};
}
#endif
#endif	// NW_GAPI
#endif // NWG_FRAME_BUFFER_H