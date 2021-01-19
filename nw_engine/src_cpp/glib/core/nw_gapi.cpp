#include <nw_pch.hpp>
#include "nw_gapi.h"

#include <sys/nw_mem_sys.h>

#if (defined NW_GRAPHICS)
#include <glib/glib_tools.h>
#include <glib/gcontext/nw_gcontext.h>
#include <glib/gcontext/nw_framebuf.h>
namespace NW
{
	AGApi* AGApi::Create(GApiTypes GApitype)
	{
		AGApi* pGApi = nullptr;
		switch (GApitype) {
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case (GAPI_OPENGL): pGApi = new GApiOgl(); break;
	#endif // NW_GRAPHICS
		default: NW_ERR("This graphics API is not defined"); break;
		}
		return pGApi;
	}
}
#endif	// NW_GRAPHICS

#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
#include <glad/glad.h>

namespace NW
{
	GApiOgl::GApiOgl() :
		m_gapiType(GAPI_OPENGL),
		m_unPrimitiveTypeId(PT_TRIANGLES),
		m_pGContext(nullptr) {}
	GApiOgl::~GApiOgl() = default;

	// --==<Setters>==--
	void GApiOgl::SetModes(bool bEnable, ProcModes unModeId) {
		if (bEnable) { glEnable(unModeId); }
		else { glDisable(unModeId); }
	}
	void GApiOgl::SetClearColor(float nRed, float nGreen, float nBlue, float nAlpha) { glClearColor(nRed, nGreen, nBlue, nAlpha); }
	void GApiOgl::SetViewport(Int32 nX, Int32 nY, Int32 nW, Int32 nH) { glViewport(nX, nY, nW, nH); }
	void GApiOgl::SetDrawMode(DrawModes unDrawModeId, FacePlanes unFacePlaneId) { m_unDrawModeId = unDrawModeId; glPolygonMode(unFacePlaneId, m_unDrawModeId); }
	void GApiOgl::SetLineWidth(UInt16 unPxW) { glLineWidth(unPxW); }
	void GApiOgl::SetPixelSize(float unPixelSize) { glPointSize(unPixelSize); }
	void GApiOgl::SetBlendFunc(BlendConfigs unSrcFactorId, BlendConfigs unDestFactorId) { glBlendFunc(unSrcFactorId, unDestFactorId); }
	void GApiOgl::SetDepthFunc(DepthConfigs unDepthFuncId) { glDepthFunc(unDepthFuncId); }
	void GApiOgl::SetStencilFunc(StencilConfigs unFuncId, UInt32 unRefValue, UInt8 unBitMask) { glStencilFunc(unFuncId, unRefValue, unBitMask); }
	// --==</Setters>==--

	// --==<Drawing>==--
	void GApiOgl::DrawIndexed(UInt32 unIndexCount) { glDrawElements(m_unPrimitiveTypeId, unIndexCount, GL_UNSIGNED_INT, nullptr); }
	void GApiOgl::DrawArray(UInt32 unVertexCount) { glDrawArrays(m_unPrimitiveTypeId, 0, unVertexCount); }
	// --==</Drawing>==--

}
#endif // NW_GRAPHICS