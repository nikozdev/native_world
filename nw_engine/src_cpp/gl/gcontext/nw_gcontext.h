#ifndef NW_AGCONTEXT_H
#define NW_AGCONTEXT_H

#include <gl_decl.hpp>

#if (defined NW_GRAPHICS)
namespace NW
{
	/// GraphicsContextInfo struct
	struct NW_API GContextInfo
	{
		CString strRenderer = "none", strVersion = "1.0.0", strVendor = "none",
			strShadingLanguage = "none";
		Int32 nMaxVertexAttribs = 0;
		Int32 nActiveTextureId = 0;
		Int32 nMaxTextures = 0;
		GContextInfo() = default;
		explicit GContextInfo(CString sRenderer, CString sVersion,
			CString sVendor, CString sShadingLanguage) :
			strRenderer(sRenderer), strVersion(sVersion),
			strVendor(sVendor), strShadingLanguage(sShadingLanguage) { }
		explicit GContextInfo(UCString sRenderer, UCString sVersion,
			UCString sVendor, UCString sShadingLanguage) :
			strRenderer((CString)sRenderer), strVersion((CString)sVersion),
			strVendor((CString)sVendor), strShadingLanguage((CString)sShadingLanguage) { }
	};
	/// GraphicalContext Abstract Class
	/// Interface:
	/// -> Create the graphical context -> Initialize it
	/// -> Swap buffers after rendering
	class NW_API AGContext
	{
	public:
		virtual ~AGContext() = default;

		// -- Getters
		virtual inline const GContextInfo& GetInfo() = 0;
		// -- Setters

		// Interface Methods
		virtual bool OnInit() = 0;
		virtual void OnQuit() = 0;

		virtual void SwapBuffers() = 0;
	};
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
namespace NW
{
	class NW_API GContextOgl : public AGContext
	{
	public:
		GContextOgl(GLFWwindow* pNativeWindow);
		~GContextOgl();

		// -- Getters
		virtual inline const GContextInfo& GetInfo() override { return m_GContextInfo; }
		// -- Setters

		// -- Core Methods
		virtual bool OnInit() override;
		virtual void OnQuit() override;

		virtual void SwapBuffers() override;
	private: 
		GLFWwindow* m_pNativeWindow;
		GContextInfo m_GContextInfo;
	};
}
#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
namespace NW
{
	class NW_API GContextCout : public AGContext
	{
	public:
		GContextCout(CoutWindow* pWindow);
		~GContextCout();

		// Getters
		inline const vsi2d& GetPxSize() const { return m_cfInfoEx.dwFontSize; }
		virtual inline AFrameBuf* GetFrameBuf(UInt32 unFrameBufId) override
		{
			return m_rBuffer.get();
		}

		// Setters
		void SetPxSize(Int16 pxW, Int16 pxH);
		void SetPxSize(const vsi2d& pxSizeWH);

		// Core Methods
		virtual bool OnInit() override;
		virtual void OnQuit() override;

		virtual void SwapBuffers() override;
	private:
		RefOwner<CoutFrameBuf> m_pBuffer;
		CoutWindow* m_pWindow;

		CFontInfoX m_cfInfoEx;
	};
}
#endif // NW_GRAPHICS

#endif // NW_OGL_GCONTEXT_H