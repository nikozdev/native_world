#include <glib_pch.hpp>
#include "glib_api.h"

#if (defined GLIB_GAPI)
#include <glib_core.hpp>
#include <glib_context.h>
#include <glib_framebuf.h>
namespace GLIB
{
	AGraphApi* AGraphApi::Create(GApiTypes GApitype)
	{
		AGraphApi* pGApi = nullptr;
		switch (GApitype) {
	#if (GLIB_GAPI & GLIB_GAPI_OGL)
		case (GAPI_OPENGL): pGApi = new GraphicsApiOgl(); break;
	#endif // GLIB_GAPI
		default: GLIB_ERR("This graphics API is not defined"); break;
		}
		return pGApi;
	}
}
#endif	// GLIB_GAPI

#if (GLIB_GAPI & GLIB_GAPI_OGL)
#include <glad/glad.h>

namespace GLIB
{
	GraphicsApiOgl::GraphicsApiOgl() :
		m_gapiType(GAPI_OPENGL),
		m_unPrimitiveTypeId(PT_TRIANGLES),
		m_pGContext(nullptr) {}
	GraphicsApiOgl::~GraphicsApiOgl() = default;

	// --==<Setters>==--
	void GraphicsApiOgl::SetModes(bool bEnable, ProcModes unModeId) {
		if (bEnable) { glEnable(unModeId); }
		else { glDisable(unModeId); }
	}
	void GraphicsApiOgl::SetClearColor(float nRed, float nGreen, float nBlue, float nAlpha) { glClearColor(nRed, nGreen, nBlue, nAlpha); }
	void GraphicsApiOgl::SetViewport(Int32 nX, Int32 nY, Int32 nW, Int32 nH) { glViewport(nX, nY, nW, nH); }
	void GraphicsApiOgl::SetDrawMode(DrawModes unDrawModeId, FacePlanes unFacePlaneId) { m_unDrawModeId = unDrawModeId; glPolygonMode(unFacePlaneId, m_unDrawModeId); }
	void GraphicsApiOgl::SetLineWidth(UInt16 unPxW) { glLineWidth(unPxW); }
	void GraphicsApiOgl::SetPixelSize(float unPixelSize) { glPointSize(unPixelSize); }
	void GraphicsApiOgl::SetBlendFunc(BlendConfigs unSrcFactorId, BlendConfigs unDestFactorId) { glBlendFunc(unSrcFactorId, unDestFactorId); }
	void GraphicsApiOgl::SetDepthFunc(DepthConfigs unDepthFuncId) { glDepthFunc(unDepthFuncId); }
	void GraphicsApiOgl::SetStencilFunc(StencilConfigs unFuncId, UInt32 unRefValue, UInt8 unBitMask) { glStencilFunc(unFuncId, unRefValue, unBitMask); }
	// --==</Setters>==--

	// --==<Drawing>==--
	void GraphicsApiOgl::DrawIndexed(UInt32 unIndexCount) { glDrawElements(m_unPrimitiveTypeId, unIndexCount, GL_UNSIGNED_INT, nullptr); }
	void GraphicsApiOgl::DrawArray(UInt32 unVertexCount) { glDrawArrays(m_unPrimitiveTypeId, 0, unVertexCount); }
	// --==</Drawing>==--

}
#endif // GLIB_GAPI