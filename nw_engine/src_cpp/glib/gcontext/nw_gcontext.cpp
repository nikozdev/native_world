#include <nw_pch.hpp>
#include "nw_gcontext.h"

#include <sys/nw_mem_sys.h>

#if (defined NW_GRAPHICS)
#include <glib/gcontext/nw_framebuf.h>
#include <glib/nw_glib_core.h>
#endif	// NW_WINDOW
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
#include <GLFW/glfw3.h>
#if defined (NW_GL_USE_GLAD)
#include <glad/glad.h>
#endif	// NW_GL_USE
#if (NW_GUI & NW_GUI_IMGUI)
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#endif	// NW_GUI

namespace NW
{
	static bool s_GLinit = false;

	GContextOgl::GContextOgl(GLFWwindow* pNativeWindow) :
		m_pNativeWindow(pNativeWindow)
	{
		NW_ASSERT(pNativeWindow, "Window handler is not correct!");
	}
	GContextOgl::~GContextOgl() = default;

	// --==<core_methods>==--
	bool GContextOgl::OnInit()
	{
		glfwMakeContextCurrent(m_pNativeWindow);
	#if defined NW_GL_USE_GLAD
		if (!s_GLinit) {
			s_GLinit = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			NW_ASSERT(s_GLinit, "Failed to initialize Glad");
		}
	#endif	// NW_GL
		m_GContextInfo = GContextInfo(glGetString(GL_RENDERER), glGetString(GL_VERSION),
			glGetString(GL_VENDOR), glGetString(GL_SHADING_LANGUAGE_VERSION));
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_GContextInfo.nMaxTextures);
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &m_GContextInfo.nMaxVertexAttribs);
		std::cout << std::endl << "Graphics Context: GLFW_" << glfwGetVersionString() << std::endl <<
			"OpenGL Renderer: " << m_GContextInfo.strRenderer << std::endl <<
			"OpenGL Version: " << m_GContextInfo.strVersion << std::endl <<
			"OpenGL Vendor: " << m_GContextInfo.strVendor << std::endl <<
			"OpenGL Shading Language: " << m_GContextInfo.strShadingLanguage << "\n\n";
		
	#if (NW_GUI & NW_GUI_IMGUI)
	#endif	// NW_GUI

		return true;
	}
	void GContextOgl::OnQuit()
	{
		if (glfwWindowShouldClose(m_pNativeWindow) == GLFW_TRUE) { glfwTerminate(); }
	}

	void GContextOgl::SwapBuffers()
	{
		glfwSwapBuffers(m_pNativeWindow);
	}
	// --==</core_methods>==--
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
namespace NW
{
	GContextCout::GContextCout(CoutWindow* pWindow) :
		m_pWindow(pWindow)
	{
		m_rBuffer.reset(MemSys::NewT<CoutFrameBuf>(m_pWindow));
		memset(&m_cfInfoEx, 0, sizeof(m_cfInfoEx));
	}
	GContextCout::~GContextCout()
	{
	}

	// Getters
	void GContextCout::SetPxSize(Int16 pxW, Int16 pxH)
	{
		if (pxW == 0 || pxH == 0 || m_pWindow->GetHCout() == nullptr) return;
		m_cfInfoEx.dwFontSize.X = pxW;
		m_cfInfoEx.dwFontSize.Y = pxH;
		if (m_cfInfoEx.cbSize != 0) SetCurrentConsoleFontEx(m_pWindow->GetHCout(), TRUE, &m_cfInfoEx);

		//FN_SETCONSOLEFONT SetConsoleFont;
		//HMODULE hm = GetModuleHandleW(L"KERNEL32.DLL");
		//SetConsoleFont = (FN_SETCONSOLEFONT)GetProcAddress(hm, "SetConsoleFont");
		//BOOL bRet = SetConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	}
	void GContextCout::SetPxSize(const vsi2d& pxSizeWH)
	{
		if (pxSizeWH.X == 0 || pxSizeWH.Y == 0 || m_pWindow->GetHCout() == nullptr) return;
		m_cfInfoEx.dwFontSize = pxSizeWH;
		if (m_cfInfoEx.cbSize != 0) SetCurrentConsoleFontEx(m_pWindow->GetHCout(), TRUE, &m_cfInfoEx);
	}

	// --==<Core methods>==--
	bool GContextCout::OnInit()
	{
		GetCurrentConsoleFontEx(m_pWindow->GetHCout(), false, &m_cfInfoEx);
		m_cfInfoEx.cbSize = sizeof(CFontInfoX); // Size in chars
		m_cfInfoEx.nFont = 0; // Font number
		m_cfInfoEx.FontFamily = FF_DONTCARE;
		m_cfInfoEx.FontWeight = FW_DONTCARE;
		wcscpy_s(m_cfInfoEx.FaceName, L"Consolas");
		if (!SetCurrentConsoleFontEx(m_pWindow->GetHCout(), false, &m_cfInfoEx)
			|| !GetCurrentConsoleFontEx(m_pWindow->GetHCout(), false, &m_cfInfoEx))
			return false;

		if (!m_rBuffer->OnInit())
			return false;

		return true;
	}
	void GContextCout::OnQuit()
	{
	}

	void GContextCout::SwapBuffers()
	{
		m_rBuffer->OnUpdate();
	}
	// --==</Core methods>==--
}

#endif	// NW_GRAPHICS