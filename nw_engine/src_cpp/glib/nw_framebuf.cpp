#include <nw_pch.hpp>
#include "nw_framebuf.h"

#if (defined NW_GRAPHICS)
#include <core/nw_core_engine.h>
#include <glib/nw_texture.h>
#include <glib/nw_gapi.h>
namespace NW
{
	AFrameBuf::AFrameBuf(const char* strName, const FrameBufInfo& rfbInfo) :
		ADataRes(strName),
		m_Info(rfbInfo), m_unRId(0),
		m_texColor(RefKeeper<ATexture2d>(CoreEngine::Get().GetMemory())),
		m_texDepth(RefKeeper<ATexture2d>(CoreEngine::Get().GetMemory())),
		m_texStencil(RefKeeper<ATexture2d>(CoreEngine::Get().GetMemory()))
	{
		String strAtchName = "fbc_" + m_strName;
		ATexture2d::Create(&strAtchName[0], m_texColor);
		strAtchName = "fbd_" + m_strName;
		ATexture2d::Create(&strAtchName[0], m_texDepth);
		strAtchName = "fbs_" + m_strName;
		ATexture2d::Create(&strAtchName[0], m_texStencil);

		ADataRes::AddDataRes<AFrameBuf>(this);
	}
	AFrameBuf::~AFrameBuf() { ADataRes::RmvDataRes<AFrameBuf>(GetId()); }

	// --setters
	void AFrameBuf::SetSizeWH(UInt32 unWidth, UInt32 unHeight) {
		m_Info.unWidth = unWidth; m_Info.unHeight = unHeight; Remake();
	}

	AFrameBuf* AFrameBuf::Create(const char* strName, const FrameBufInfo& rfbInfo)
	{
		AFrameBuf* pFB = nullptr;
		switch (CoreEngine::Get().GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL: pFB = CoreEngine::Get().NewT<FrameBufOgl>(strName, rfbInfo); break;
	#endif // NW_GRAPHICS
		default: NWL_ERR("Graphics API is not defined"); break;
		}
		return pFB;
	}
	void AFrameBuf::Create(const char* strName, const FrameBufInfo& rfbInfo, RefOwner<AFrameBuf>& rfBuf)
	{
		switch (CoreEngine::Get().GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL: rfBuf.MakeRef<FrameBufOgl>(strName, rfbInfo); break;
	#endif // NW_GRAPHICS
		default: NWL_ERR("Graphics API is not defined"); break;
		}
	}
	void AFrameBuf::Create(const char* strName, const FrameBufInfo& rfbInfo, RefKeeper<AFrameBuf>& rfBuf)
	{
		switch (CoreEngine::Get().GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL: rfBuf.MakeRef<FrameBufOgl>(strName, rfbInfo); break;
	#endif // NW_GRAPHICS
		default: NWL_ERR("Graphics API is not defined"); break;
		}
	}
}
#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
#include <glad/glad.h>
namespace NW
{
	// Constructor&Destructor
	FrameBufOgl::FrameBufOgl(const char* strName, const FrameBufInfo& rfbInfo) :
		AFrameBuf(strName, rfbInfo) { Remake(); }
	FrameBufOgl::~FrameBufOgl() { glDeleteFramebuffers(1, &m_unRId); }

	// --==<Core methods>==--
	void FrameBufOgl::Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_unRId); }
	void FrameBufOgl::Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	void FrameBufOgl::Remake()
	{
		if (m_Info.unHeight > 4096 || m_Info.unHeight == 0 ||
			m_Info.unWidth > 4096 || m_Info.unWidth == 0) { return; }
		if (m_unRId != 0) {
			glDeleteFramebuffers(1, &m_unRId);
		}

		glCreateFramebuffers(1, &m_unRId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_unRId);

		{
			ImageInfo ImgInfo;
			ImgInfo.ClrData = nullptr;
			ImgInfo.nChannels = 4;
			ImgInfo.nDepth = 1;
			ImgInfo.nWidth = static_cast<Int32>(m_Info.unWidth);
			ImgInfo.nHeight = static_cast<Int32>(m_Info.unHeight);
			TextureInfo TexInfo;
			TexInfo.FilterMag = TexInfo.FilterMin = TC_FILTER_LINEAR;
			TexInfo.InterFormat = TC_FORMAT_RGBA8; TexInfo.Format = TC_FORMAT_RGB;
			m_texColor->SetInfo(TexInfo);
			m_texColor->SetInfo(ImgInfo);
			m_texColor->Remake();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texColor->GetRenderId(), 0);
		}

		{
			ImageInfo ImgInfo;
			ImgInfo.ClrData = nullptr;
			ImgInfo.nChannels = 4;
			ImgInfo.nDepth = 1;
			ImgInfo.nWidth = static_cast<Int32>(m_Info.unWidth);
			ImgInfo.nHeight = static_cast<Int32>(m_Info.unHeight);
			TextureInfo TexInfo;
			TexInfo.FilterMag = TexInfo.FilterMin = TC_FILTER_LINEAR;
			TexInfo.InterFormat = TC_FORMAT_RED; TexInfo.Format = TC_FORMAT_RED;
			m_texColor->SetInfo(TexInfo);
			m_texColor->SetInfo(ImgInfo);
			m_texColor->Remake();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texColor->GetRenderId(), 0);
		}

		{
			ImageInfo ImgInfo;
			ImgInfo.ClrData = nullptr;
			ImgInfo.nChannels = 4;
			ImgInfo.nDepth = 1;
			ImgInfo.nWidth = static_cast<Int32>(m_Info.unWidth);
			ImgInfo.nHeight = static_cast<Int32>(m_Info.unHeight);
			TextureInfo TexInfo;
			TexInfo.FilterMag = TexInfo.FilterMin = TC_FILTER_LINEAR;
			TexInfo.InterFormat = TC_FORMAT_RED; TexInfo.Format = TC_FORMAT_RED;
			m_texColor->SetInfo(TexInfo);
			m_texColor->SetInfo(ImgInfo);
			m_texColor->Remake();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_texColor->GetRenderId(), 0);
		}

			bool bIsCompleted = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
				NWL_ASSERT(bIsCompleted, "FrameBufOgl is not created!");
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void FrameBufOgl::Clear(UInt32 unAttachmentsMask) { glClear(unAttachmentsMask); }
	// --==</Core methods>==--
}
#endif // NW_GRAPHICS