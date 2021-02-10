#ifndef NW_GFX_FRAME_BUFFER_H
#define NW_GFX_FRAME_BUFFER_H

#include <gfx_core.hpp>

#if (defined NW_GAPI)
#if (NW_GAPI & NW_GAPI_OGL)
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
	/// FrameBuffer class
	class NW_API FrameBuf : public TDataRes<FrameBuf>
	{
	public:
		FrameBuf(const char* strName, const FrameBufInfo& rFbInfo);
		virtual ~FrameBuf();

		// --getters
		inline UInt32 GetRenderId() const { return m_unRId; }
		inline Int32 GetWidth() const { return m_Info.GetWidth(); }
		inline Int32 GetHeight() const { return m_Info.GetHeight(); }
		inline Texture* GetAttachment(UInt32 unIdx = 0);
		inline const V4i& GetViewport() const { return m_Info.rectViewport; }
		inline const FrameBufInfo& GetInfo() const { return m_Info; }
		inline V4f GetClearColor() const { return m_rgbaClear; }
		// --setters
		void SetViewport(V4i rectViewport);
		void SetClearColor(V4f rgbaClear);
		void AttachTexture(Texture& rTex);
		void DetachTexture(UInt32 unIdx);
		// --predicates
		inline bool IsBound() { return m_bIsBound; }
		// --core_methods
		void Bind() const;
		void Unbind() const;
		void Remake();
		void Clear(UInt32 bitMask = FB_COLOR | FB_DEPTH | FB_STENCIL);
		void ReadPixels(Ptr pData, UInt32 unAttachIdx, Int32 nX, Int32 nY, Int32 nWidth = 1, Int32 nHeight = 1);
		void WritePixels(Ptr pData, UInt32 unAttachIdx, Int32 nX, Int32 nY, Int32 nWidth = 1, Int32 nHeight = 1);
		// --data_methods
		virtual bool SaveF(const char* strFPath) override { return true; }
		virtual bool LoadF(const char* strFPath) override { return true; }
	protected:
		mutable Bit m_bIsBound;
		UInt32 m_unRId;
		FrameBufInfo m_Info;
		V4f m_rgbaClear;
		DArray<RefKeeper<Texture>> m_Attachments;
	};
	inline Texture* FrameBuf::GetAttachment(UInt32 unIdx) {
		NWL_ASSERT(unIdx <= m_Attachments.size(), "Overflow");
		return m_Attachments[unIdx].GetRef();
	}
}
#endif
#endif	// NW_GAPI
#endif // NW_GFX_FRAME_BUFFER_H