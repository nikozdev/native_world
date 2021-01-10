#include <nw_pch.hpp>
#include "nw_framebuf.h"

#include <sys/nw_mem_sys.h>
#include <sys/nw_data_sys.h>

#if (defined NW_GRAPHICS)
#include <glib/control/nw_graph_engine.h>
#include <glib/render/nw_texture.h>
namespace NW
{
	AFrameBuf::AFrameBuf(const char* strName, const FrameBufInfo& rfbInfo) :
		ADataRes(strName),
		m_Info(rfbInfo), m_unRId(0), m_unRIdDepth(0), m_ColorAttach(nullptr)
	{
		String strClrName = strName;
		strClrName += std::to_string(GetId());
		m_ColorAttach = ATexture2d::Create(&strClrName[0]);
		DataSys::AddDataRes<AFrameBuf>(this);
	}
	AFrameBuf::~AFrameBuf() { DataSys::RemoveDataRes<AFrameBuf>(GetName()); }

	// --setters
	void AFrameBuf::SetSizeWH(UInt32 unWidth, UInt32 unHeight) {
		m_Info.unWidth = unWidth; m_Info.unHeight = unHeight; Remake();
	}

	AFrameBuf* AFrameBuf::Create(const char* strName, const FrameBufInfo& rfbInfo)
	{
		AFrameBuf* pFB = nullptr;
		switch (GraphEngine::Get().GetGApi()->GetType())
		{
	#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
		case GApiTypes::GAPI_COUT:
			pFB = MemSys::NewT<FrameBufCout>(strName, rfbInfo);
			break;
	#endif // NW_GRAPHICS
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL:
			pFB = MemSys::NewT<FrameBufOgl>(strName, rfbInfo);
			break;
	#endif // NW_GRAPHICS
		default:
			NW_ERR("Graphics API is not defined");
			break;
		}
		return pFB;
	}
}
#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
#include <glad/glad.h>
namespace NW
{
	// Constructor&Destructor
	FrameBufOgl::FrameBufOgl(const char* strName, const FrameBufInfo& rfbInfo) :
		AFrameBuf(strName, rfbInfo)
	{
		Remake();
	}
	FrameBufOgl::~FrameBufOgl()
	{
		glDeleteFramebuffers(1, &m_unRId);
		glDeleteRenderbuffers(1, &m_unRIdDepth);
	}

	// --==<Core methods>==--
	void FrameBufOgl::Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_unRId); }
	void FrameBufOgl::Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	void FrameBufOgl::Remake()
	{
		if (m_Info.unHeight > 4096 || m_Info.unHeight == 0 ||
			m_Info.unWidth > 4096 || m_Info.unWidth == 0) {
			return;
		}

		if (m_unRId != 0 || m_ColorAttach->GetRenderId() != 0 || m_unRIdDepth != 0) {
			glDeleteFramebuffers(1, &m_unRId);
			glDeleteRenderbuffers(1, &m_unRIdDepth);
		}

		glCreateFramebuffers(1, &m_unRId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_unRId);

		ImageInfo ImgInfo;
		ImgInfo.ClrData = nullptr;
		ImgInfo.nChannels = 4;
		ImgInfo.nDepth = 1;
		ImgInfo.nWidth = static_cast<Int32>(m_Info.unWidth);
		ImgInfo.nHeight = static_cast<Int32>(m_Info.unHeight);
		TextureInfo TexInfo;
		TexInfo.FilterMag = TexInfo.FilterMin = TC_FILTER_LINEAR;
		TexInfo.InterFormat = TC_FORMAT_RGBA8; TexInfo.Format = TC_FORMAT_RGB;
		m_ColorAttach->SetInfo(TexInfo);
		m_ColorAttach->SetInfo(ImgInfo);
		m_ColorAttach->Remake();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttach->GetRenderId(), 0);

		glCreateRenderbuffers(1, &m_unRIdDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, m_unRIdDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
			static_cast<Int32>(m_Info.unWidth), static_cast<Int32>(m_Info.unHeight));
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_unRIdDepth);

		bool bIsCompleted = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
		NW_ASSERT(bIsCompleted, "FrameBufOgl is not created!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void FrameBufOgl::Clear(UInt32 unAttachmentsMask) { glClear(unAttachmentsMask); }
	// --==</Core methods>==--

	// --==<Data methods>==--
	bool FrameBufOgl::LoadF(const char* strFPath) {
		return true;
	}
	bool FrameBufOgl::SaveF(const char* strFPath) {
		return true;
	}
	// --==</Data methods>==--
}
#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
namespace NW
{
	CoutFrameBuf::CoutFrameBuf(CoutWindow* pWindow) :
		m_ciChars(nullptr), m_pWindow(pWindow),
		m_xywhRect(m_pWindow->GetRect()), m_whSize({ 0, 0 }), m_whMaxSize({ 0, 0 }),
		m_unSize(0)
	{
		memset(&m_csbInfo, 0, sizeof(m_csbInfo));
		memset(&m_csbOrigInfo, 0, sizeof(m_csbOrigInfo));
	}
	CoutFrameBuf::~CoutFrameBuf()
	{
	}

	// --getters
	// --setters
	bool CoutFrameBuf::SetSizeWH(Int16 sizeW, Int16 sizeH)
	{
		if (m_whSize.X == sizeW || m_whSize.Y == sizeH || !m_pWindow) return false;
		void* hCout = m_pWindow->GetHCout();

		m_whSize = { sizeW, sizeH }; m_unSize = m_whSize.X * m_whSize.Y;
		m_xywhRect = { 0, 0, m_whSize.X - 1, m_whSize.Y - 1 };

		m_ciChars = MemSys::NewTArr<ChInfo>(m_unSize);
		memset(m_ciChars, 0, sizeof(ChInfo) * m_unSize);

		return true;
	}

	// --==<core_methods>==--
	bool CoutFrameBuf::OnInit()
	{
		if (!SetSizeWH(m_pWindow->GetWindowInfo().usiWidth, m_pWindow->GetWindowInfo().usiHeight))
		{
			return false;
		}

		if (!GetConsoleScreenBufferInfo(m_pWindow->GetHCout(), &m_csbInfo))
			return false;
		m_csbOrigInfo = m_csbInfo;

		m_whMaxSize = m_csbInfo.dwMaximumWindowSize;
		if (m_xywhRect.Right > m_whMaxSize.X || m_xywhRect.Bottom > m_whMaxSize.Y)
			return false;

		return true;
	}
	void CoutFrameBuf::OnQuit()
	{
	}

	void CoutFrameBuf::OnUpdate(ChInfo* ciBuffer)
	{
		if (ciBuffer == nullptr)
			ciBuffer = m_ciChars;
	#ifdef UNICODE
		WriteConsoleOutputW(m_pWindow->GetHCout(), ciBuffer, m_whSize,
			{ m_xywhRect.Left, m_xywhRect.Top }, &m_pWindow->GetRect());
	#else
		WriteConsoleOutputA(m_pWindow->GetHCout(), ciBuffer, m_whSize,
			{ m_xywhRect.Left, m_xywhRect.Top }, &m_xywhRect);
	#endif
	}
	// --==</core_methods>==--
	void CoutFrameBuf::Clear(UInt32 clearColor = 0)
	{
		ChInfo clearChar;
		clearChar.Attributes = clearColor;
		clearChar.Char.UnicodeChar = ' ';
		clearChar.Char.AsciiChar = ' ';
		for (UInt16 px = 0; px < m_unSize; px++)
		{
			m_ciChars[px] = clearChar;
		}
	}
}
#endif	// NW_GRAPHICS