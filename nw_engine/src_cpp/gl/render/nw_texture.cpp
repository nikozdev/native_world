#include <nw_pch.hpp>
#include "nw_texture.h"

#include <sys/nw_data_sys.h>
#include <sys/nw_mem_sys.h>

#if (defined NW_GRAPHICS)
#include <gl/control/nw_draw_engine.h>

#include <sys/nw_log_sys.h>

NW::UByte NW::ATexture::s_ClearColorData[4] = { 255, 255, 255, 255 };

namespace NW
{
	ATexture::ATexture(const char* strName) :
		ADataRes(strName),
		m_strName(strName), m_unRId(0), m_unTexSlot(0),
		m_TexInfo(TextureInfo()), m_ImgInfo(ImageInfo())
	{
		DataSys::AddDataRes<ATexture>(this);
	}
	ATexture::~ATexture() { DataSys::RemoveDataRes<ATexture>(GetName()); }

	// --==<ATexture1d>==--
	ATexture1d::ATexture1d(const char* strName) :
	ATexture(strName) { DataSys::AddDataRes<ATexture1d>(this); }
	ATexture1d::~ATexture1d() { DataSys::RemoveDataRes<ATexture1d>(GetName()); }

	bool ATexture1d::SaveF(const char* strFPath) { return true; }
	bool ATexture1d::LoadF(const char* strFPath)
	{
		String strFile("");
		Size szBytes = 0;
		bool bSuccess = true;
		
		ImageInfo ImgInfoTemp;

		if (!DataSys::LoadF_image(strFPath, &ImgInfoTemp)) {
			m_TexInfo.FilterMag = m_TexInfo.FilterMin = TC_FILTER_NEAREST;
			m_TexInfo.Format = m_TexInfo.InterFormat = TC_FORMAT_RGBA;
			m_TexInfo.WrapTypeS = m_TexInfo.WrapTypeT = m_TexInfo.WrapTypeR = TC_WRAP_REPEAT;
			
			ImgInfoTemp.nWidth = ImgInfoTemp.nHeight = ImgInfoTemp.nDepth;
			ImgInfoTemp.nChannels = 4;
			ImgInfoTemp.ClrData = &s_ClearColorData[0];
			
			bSuccess = false;
			if (DataSys::LoadF_string(strFPath, strFile) == false) {}
			else {}
		}

		SetInfo(m_TexInfo);
		SetInfo(ImgInfoTemp);
		Remake();

		return bSuccess;
	}
	// --==</ATexture1d>==--

	// --==<ATexture2d>==--
	ATexture2d::ATexture2d(const char* strName) :
		ATexture(strName) { DataSys::AddDataRes<ATexture2d>(this); }
	ATexture2d::~ATexture2d() { DataSys::RemoveDataRes<ATexture2d>(GetName()); }

	bool ATexture2d::SaveF(const char* strFPath) { return true; }
	bool ATexture2d::LoadF(const char* strFPath)
	{
		String strFile("");
		Size szBytes = 0;
		bool bSuccess = true;

		ImageInfo ImgInfoTemp;
		TextureInfo TexInfoTemp;

		if (!DataSys::LoadF_image(strFPath, &ImgInfoTemp)) {
			TexInfoTemp.FilterMag = TexInfoTemp.FilterMin = TC_FILTER_NEAREST;
			TexInfoTemp.Format = TexInfoTemp.InterFormat = TC_FORMAT_RGBA;
			TexInfoTemp.WrapTypeS = TexInfoTemp.WrapTypeT = TexInfoTemp.WrapTypeR = TC_WRAP_REPEAT;

			ImgInfoTemp.nWidth = ImgInfoTemp.nHeight = ImgInfoTemp.nDepth;
			ImgInfoTemp.nChannels = 4;
			ImgInfoTemp.ClrData = &s_ClearColorData[0];

			bSuccess = false;
			if (DataSys::LoadF_string(strFPath, strFile) == false) {}
			else {}
		}
		switch (ImgInfoTemp.nChannels) {
		case 1: TexInfoTemp.Format = TC_FORMAT_RED; TexInfoTemp.InterFormat = TC_FORMAT_RED; break;
		case 2: LogSys::WriteErrStr(NW_ERR_NO_SUPPORT, "Unsupported format!"); break;
		case 3: TexInfoTemp.InterFormat = TC_FORMAT_RGB; TexInfoTemp.Format = TC_FORMAT_RGB; break;
		case 4: TexInfoTemp.Format = TC_FORMAT_RGBA; TexInfoTemp.InterFormat = TC_FORMAT_RGBA8; break;
		}
		TexInfoTemp.FilterMag = TexInfoTemp.FilterMin = TC_FILTER_NEAREST;
		TexInfoTemp.Format = TexInfoTemp.InterFormat = TC_FORMAT_RGBA;
		TexInfoTemp.WrapTypeS = TexInfoTemp.WrapTypeT = TexInfoTemp.WrapTypeR = TC_WRAP_REPEAT;

		SetInfo(TexInfoTemp);
		SetInfo(ImgInfoTemp);
		Remake();

		return bSuccess;
	}
	// --==</ATexture2d>==--

	// --==<ATexture3d>==--
	ATexture3d::ATexture3d(const char* strName) :
		ATexture(strName) { DataSys::AddDataRes<ATexture3d>(this); }
	ATexture3d::~ATexture3d() { DataSys::RemoveDataRes<ATexture3d>(GetName()); }

	bool ATexture3d::SaveF(const char* strFPath) { return true; }
	bool ATexture3d::LoadF(const char* strFPath)
	{
		String strFile("");
		Size szBytes = 0;
		bool bSuccess = true;

		ImageInfo ImgInfoTemp;

		if (!DataSys::LoadF_image(strFPath, &ImgInfoTemp)) {
			m_TexInfo.FilterMag = m_TexInfo.FilterMin = TC_FILTER_NEAREST;
			m_TexInfo.Format = m_TexInfo.InterFormat = TC_FORMAT_RGBA;
			m_TexInfo.WrapTypeS = m_TexInfo.WrapTypeT = m_TexInfo.WrapTypeR = TC_WRAP_REPEAT;

			ImgInfoTemp.nWidth = ImgInfoTemp.nHeight = ImgInfoTemp.nDepth;
			ImgInfoTemp.nChannels = 4;
			ImgInfoTemp.ClrData = &s_ClearColorData[0];

			bSuccess = false;
			if (DataSys::LoadF_string(strFPath, strFile) == false) {}
			else {}
		}

		SetInfo(m_TexInfo);
		SetInfo(ImgInfoTemp);
		Remake();

		return bSuccess;
	}
	// --==</ATexture3d>==--

	ATexture1d* ATexture1d::Create(const char* strName) {
		ATexture1d* pTex = nullptr;
		switch (DrawEngine::GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
		case GAPI_COUT:
			pTex = MemSys::NewT<Texture1dCout>(strName);
			break;
	#endif	// NW_GRAPHICS
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GAPI_OPENGL:
			pTex = MemSys::NewT<Texture1dOgl>(strName);
			break;
	#endif	// NW_GRAPHICS
		default: break;
		}
		return pTex;
	}
	ATexture2d* ATexture2d::Create(const char* strName) {
		ATexture2d* pTex = nullptr;
		switch (DrawEngine::GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
		case GAPI_COUT:
			pTex = MemSys::NewT<Texture2dCout>(strName);
			break;
	#endif	// NW_GRAPHICS
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GAPI_OPENGL:
			pTex = MemSys::NewT<Texture2dOgl>(strName);
			break;
	#endif	// NW_GRAPHICS
		default: break;
		}
		return pTex;
	}
	ATexture3d* ATexture3d::Create(const char* strName) {
		ATexture3d* pTex = nullptr;
		switch (DrawEngine::GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
		case GAPI_COUT:
			pTex = MemSys::NewT<Texture3dCout>(strName);
			break;
	#endif	// NW_GRAPHICS
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GAPI_OPENGL:
			pTex = MemSys::NewT<Texture3dOgl>(strName);
			break;
	#endif	// NW_GRAPHICS
		default: break;
		}
		return pTex;
	}
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
#include <glad/glad.h>
namespace NW
{
	// Texture1d
	Texture1dOgl::Texture1dOgl(const char* strName) :
		ATexture1d(strName)
	{ }
	Texture1dOgl::~Texture1dOgl()
	{ }

	// --setters
	void Texture1dOgl::SetInfo(const TextureInfo& rTexInfo) {
		m_TexInfo = rTexInfo;
	}
	void Texture1dOgl::SetInfo(const ImageInfo& rImgInfo) {
		if (rImgInfo.nWidth < 1 || rImgInfo.nHeight < 1 || rImgInfo.nChannels < 1) { return; }
		if (m_ImgInfo.ClrData != &s_ClearColorData[0] && m_ImgInfo.ClrData != nullptr) {
			MemSys::DelTArr<UByte>(m_ImgInfo.ClrData, m_ImgInfo.nWidth * m_ImgInfo.nHeight * m_ImgInfo.nChannels);
		}
		m_ImgInfo.ClrData = nullptr;

		m_ImgInfo = rImgInfo;
	}

	// --==<Interface Methods>==--
	void Texture1dOgl::Bind(UInt32 unTexSlot)
	{
		m_unTexSlot = unTexSlot;
		glActiveTexture(GL_TEXTURE0 + m_unTexSlot);
		glBindTexture(GL_TEXTURE_1D, m_unRId);
	}
	void Texture1dOgl::Unbind()
	{
		glActiveTexture(GL_TEXTURE0 + m_unTexSlot);
		glBindTexture(GL_TEXTURE_1D, 0);
		m_unTexSlot = 0;
	}
	void Texture1dOgl::Remake()
	{
		if (m_unRId != 0) { glDeleteTextures(1, &m_unRId); m_unRId = 0; }
		glCreateTextures(GL_TEXTURE_1D, 1, &m_unRId);

		switch (m_ImgInfo.nChannels) {
		case 1: m_TexInfo.InterFormat = m_TexInfo.Format = TC_FORMAT_RED;
			break;
		case 3: m_TexInfo.InterFormat = m_TexInfo.Format = TC_FORMAT_RGB;
			break;
		case 4: m_TexInfo.InterFormat = m_TexInfo.Format = TC_FORMAT_RGBA;
			break;
		default: return;
		}
		Bind(m_unTexSlot);
		glTextureParameteri(m_unRId, GL_TEXTURE_MIN_FILTER, m_TexInfo.FilterMin);
		glTextureParameteri(m_unRId, GL_TEXTURE_MAG_FILTER, m_TexInfo.FilterMag);
		glTextureParameteri(m_unRId, GL_TEXTURE_WRAP_S, m_TexInfo.WrapTypeS);

		glTexImage1D(GL_TEXTURE_1D, 0, m_TexInfo.InterFormat,
			m_ImgInfo.nWidth, 0,
			m_TexInfo.Format, GL_UNSIGNED_BYTE, &m_ImgInfo.ClrData[0]);
		glTexSubImage1D(GL_TEXTURE_1D, 0,
			0, m_ImgInfo.nWidth,
			m_TexInfo.Format, GL_UNSIGNED_BYTE, &m_ImgInfo.ClrData[0]);

		glGenerateMipmap(GL_TEXTURE_1D);
		Unbind();
	}
	// --==</core_methods>==--
	// Texture2d
	Texture2dOgl::Texture2dOgl(const char* strName) :
		ATexture2d(strName)
	{ }
	Texture2dOgl::~Texture2dOgl()
	{ }

	// --setters
	void Texture2dOgl::SetInfo(const TextureInfo& rTexInfo) {
		m_TexInfo = rTexInfo;
	}
	void Texture2dOgl::SetInfo(const ImageInfo& rImgInfo) {
		if (rImgInfo.nWidth < 1 || rImgInfo.nHeight < 1 || rImgInfo.nChannels < 1) { return; }
		if (m_ImgInfo.ClrData != &s_ClearColorData[0] && m_ImgInfo.ClrData != nullptr) {
			MemSys::DelTArr<UByte>(m_ImgInfo.ClrData, m_ImgInfo.nWidth * m_ImgInfo.nHeight * m_ImgInfo.nChannels);
		}
		m_ImgInfo = rImgInfo;
	}

	// --==<Interface Methods>==--
	void Texture2dOgl::Bind(UInt32 unTexSlot)
	{
		m_unTexSlot = unTexSlot;
		glActiveTexture(GL_TEXTURE0 + m_unTexSlot);
		glBindTexture(GL_TEXTURE_2D, m_unRId);
	}
	void Texture2dOgl::Unbind()
	{
		glActiveTexture(GL_TEXTURE0 + m_unTexSlot);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_unTexSlot = 0;
	}
	void Texture2dOgl::Remake()
	{
		if (m_unRId != 0) { glDeleteTextures(1, &m_unRId); m_unRId = 0; }
		glCreateTextures(GL_TEXTURE_2D, 1, &m_unRId);

		Bind(m_unTexSlot);
		glTextureParameteri(m_unRId, GL_TEXTURE_MIN_FILTER, m_TexInfo.FilterMin);
		glTextureParameteri(m_unRId, GL_TEXTURE_MAG_FILTER, m_TexInfo.FilterMag);
		glTextureParameteri(m_unRId, GL_TEXTURE_WRAP_S, m_TexInfo.WrapTypeS);
		glTextureParameteri(m_unRId, GL_TEXTURE_WRAP_T, m_TexInfo.WrapTypeT);

		glTexImage2D(GL_TEXTURE_2D, 0, m_TexInfo.InterFormat,
			m_ImgInfo.nWidth, m_ImgInfo.nHeight, 0,
			m_TexInfo.Format, GL_UNSIGNED_BYTE, &m_ImgInfo.ClrData[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0,
			0, 0, m_ImgInfo.nWidth, m_ImgInfo.nHeight,
			m_TexInfo.Format, GL_UNSIGNED_BYTE, &m_ImgInfo.ClrData[0]);
	
		glGenerateMipmap(GL_TEXTURE_2D);
		Unbind();
	}
	// --==</core_methods>==--
	
	// Texture3d
	Texture3dOgl::Texture3dOgl(const char* strName) :
		ATexture3d(strName)
	{ }
	Texture3dOgl::~Texture3dOgl() { }

	// --setters
	void Texture3dOgl::SetInfo(const TextureInfo& rTexInfo) {
		m_TexInfo = rTexInfo;
	}
	void Texture3dOgl::SetInfo(const ImageInfo& rImgInfo) {
		if (rImgInfo.nWidth < 1 || rImgInfo.nHeight < 1 || rImgInfo.nChannels < 1) { return; }
		if (m_ImgInfo.ClrData != &s_ClearColorData[0] && m_ImgInfo.ClrData != nullptr) {
			MemSys::DelTArr<UByte>(m_ImgInfo.ClrData, m_ImgInfo.nWidth * m_ImgInfo.nHeight * m_ImgInfo.nChannels);
		}
		m_ImgInfo = rImgInfo;
	}

	// --==<Interface Methods>==--
	void Texture3dOgl::Bind(UInt32 unTexSlot)
	{
		m_unTexSlot = unTexSlot;
		glActiveTexture(GL_TEXTURE0 + m_unTexSlot);
		glBindTexture(GL_TEXTURE_3D, m_unRId);
	}
	void Texture3dOgl::Unbind()
	{
		glActiveTexture(GL_TEXTURE0 + m_unTexSlot);
		glBindTexture(GL_TEXTURE_3D, 0);
		m_unTexSlot = 0;
	}
	void Texture3dOgl::Remake()
	{
		if (m_unRId != 0) { glDeleteTextures(1, &m_unRId); m_unRId = 0; }
		glCreateTextures(GL_TEXTURE_3D, 1, &m_unRId);

		switch (m_ImgInfo.nChannels) {
		case 1: m_TexInfo.InterFormat = m_TexInfo.Format = TC_FORMAT_RED;
			break;
		case 3: m_TexInfo.InterFormat = m_TexInfo.Format = TC_FORMAT_RGB;
			break;
		case 4: m_TexInfo.InterFormat = m_TexInfo.Format = TC_FORMAT_RGBA;
			break;
		default: return;
		}
		Bind(m_unTexSlot);
		glTextureParameteri(m_unRId, GL_TEXTURE_MIN_FILTER, m_TexInfo.FilterMin);
		glTextureParameteri(m_unRId, GL_TEXTURE_MAG_FILTER, m_TexInfo.FilterMag);
		glTextureParameteri(m_unRId, GL_TEXTURE_WRAP_S, m_TexInfo.WrapTypeS);
		glTextureParameteri(m_unRId, GL_TEXTURE_WRAP_T, m_TexInfo.WrapTypeT);
		glTextureParameteri(m_unRId, GL_TEXTURE_WRAP_R, m_TexInfo.WrapTypeR);

		glTexImage3D(GL_TEXTURE_3D, 0, m_TexInfo.InterFormat,
			m_ImgInfo.nWidth, m_ImgInfo.nHeight, m_ImgInfo.nDepth, 0,
			m_TexInfo.Format, GL_UNSIGNED_BYTE, &m_ImgInfo.ClrData[0]);
		glTexSubImage3D(GL_TEXTURE_3D, 0,
			0, 0, 0, m_ImgInfo.nWidth, m_ImgInfo.nHeight, m_ImgInfo.nDepth,
			m_TexInfo.Format, GL_UNSIGNED_BYTE, &m_ImgInfo.ClrData[0]);

		glGenerateMipmap(GL_TEXTURE_3D);
		Unbind();
	}
	// --==</core_methods>==--
}
#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
	namespace NW
	{
		Texture2dCout::Texture2dCout() :
			m_unRId(0)
		{
			b_pDataClr = nullptr;
			b_unSize = 0;
			b_nPxW = b_nPxH = 0;
			b_nChannels = 1;
		}
		Texture2dCout::~Texture2dCout()
		{
		}

		// Interface Methods
		inline void Texture2dCout::Bind(UInt32 textureSlot) const
		{
		}
		inline void Texture2dCout::Unbind(UInt32 textureSlot) const
		{
		}
		bool Texture2dCout::MakeTexture(UInt32 bitMask)
		{
			return true;
		}
	}
#endif // NW_GRAPHICS