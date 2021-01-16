#ifndef NW_AGCONTEXT_H
#define NW_AGCONTEXT_H

#include <glib_decl.hpp>

#if (defined NW_GRAPHICS)
namespace NW
{
	/// GraphicsContextInfo struct
	struct NW_API GContextInfo
	{
		const char* strRenderer = "none", *strVersion = "1.0.0", *strVendor = "none",
			*strShadingLanguage = "none";
		Int32 nMaxVertexAttribs = 0;
		Int32 nActiveTextureId = 0;
		Int32 nMaxTextures = 0;
		GContextInfo() = default;
		explicit GContextInfo(CString sRenderer, CString sVersion,
			CString sVendor, CString sShadingLanguage) :
			strRenderer(sRenderer), strVersion(sVersion),
			strVendor(sVendor), strShadingLanguage(sShadingLanguage) { }
		explicit GContextInfo(const UChar* sRenderer, const UChar* sVersion,
			const UChar* sVendor, const UChar* sShadingLanguage) :
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

		// --getters
		virtual inline const GContextInfo& GetInfo() = 0;
		// --setters

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

		// --getters
		virtual inline const GContextInfo& GetInfo() override { return m_GContextInfo; }
		// --setters

		// --core_methods
		virtual bool OnInit() override;
		virtual void OnQuit() override;

		virtual void SwapBuffers() override;
	private: 
		GLFWwindow* m_pNativeWindow;
		GContextInfo m_GContextInfo;
	};
}
#endif // NW_GRAPHICS
#endif // NW_GCONTEXT_H