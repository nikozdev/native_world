#ifndef NW_AFRAME_BUFFER_H
#define NW_AFRAME_BUFFER_H

#include <glib_decl.hpp>

#if (defined NW_GRAPHICS)
namespace NW
{
	enum FBAttachTypes : UInt32 {
		FBAT_NONE = 0, FBAT_COLOR, FBAT_DEPTH, FBAT_STENCIL,
	};
	/// FrameBufInfo struct
	struct NW_API FrameBufInfo
	{
		UInt16 unWidth = 0, unHeight = 0;
		UInt16 unSamples = 1;

		bool bSwapChain = false;
	};
	/// Abstract FrameBuf class
	class NW_API AFrameBuf : public ADataRes
	{
	public:
		AFrameBuf(const char* strName, const FrameBufInfo& rFbInfo);
		virtual ~AFrameBuf();
	
		// --getters
		inline const UInt32 GetRenderId() const { return m_unRId; }
		inline ATexture2d* GetAttachment(FBAttachTypes fbAttachment, UInt32 unIdx = 0);
		inline const Int16 GetWidth() const { return static_cast<Int16>(m_Info.unWidth); }
		inline const Int16 GetHeight() const { return static_cast<Int16>(m_Info.unHeight); }
		inline const FrameBufInfo& GetInfo() const { return m_Info; }
		// --setters
		void SetSizeWH(UInt32 unWidth, UInt32 unHeight);
		// --core_methods
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Remake() = 0;
		virtual void Clear(UInt32 unAttachmentsMask = FB_COLOR | FB_DEPTH | FB_STENCIL) = 0;
		
		// --data_methods
		virtual bool SaveF(const char* strFPath) override { return true; }
		virtual bool LoadF(const char* strFPath) override { return true; }

		static AFrameBuf* Create(const char* strName, const FrameBufInfo& rFbInfo);
		static void Create(const char* strName, const FrameBufInfo& rFbInfo, RefOwner<AFrameBuf>& rfBuf);
		static void Create(const char* strName, const FrameBufInfo& rFbInfo, RefKeeper<AFrameBuf>& rfBuf);
	protected:
		UInt32 m_unRId;
		RefKeeper<ATexture2d> m_texColor;
		RefKeeper<ATexture2d> m_texDepth;
		RefKeeper<ATexture2d> m_texStencil;
		FrameBufInfo m_Info;
	};
	inline ATexture2d* AFrameBuf::GetAttachment(FBAttachTypes fbaType, UInt32 unIdx) {
		switch (fbaType) {
		case FBAT_COLOR: return m_texColor.GetRef(); break;
		case FBAT_DEPTH: return m_texDepth.GetRef(); break;
		case FBAT_STENCIL: return m_texStencil.GetRef(); break;
		default: return nullptr; break;
		}
		return nullptr;
	}
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
namespace NW
{
	/// FrameBufOpenGL
	class FrameBufOgl : public AFrameBuf
	{
	public:
		FrameBufOgl(const char* strName, const FrameBufInfo& rFbInfo);
		virtual ~FrameBufOgl();

		// --core_methods
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Remake() override;
		virtual void Clear(UInt32 unAttachmentsMask = FB_COLOR | FB_DEPTH | FB_STENCIL) override;
	};
}
#endif // NW_GRAPHICS
#endif // NW_AFRAME_BUFFER_H