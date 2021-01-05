#include <nw_pch.hpp>
#include "nw_gapi.h"

#include <gl/nw_gl_core.h>
#include <gl/gcontext/nw_gcontext.h>
#include <gl/gcontext/nw_framebuf.h>

#include <sys/nw_mem_sys.h>

#if (defined NW_GRAPHICS)
namespace NW
{
	AGraphicsApi* AGraphicsApi::Create(GApiTypes GApitype)
	{
		AGraphicsApi* pGApi = nullptr;
		switch (GApitype)
		{
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
		case (GAPI_COUT):
			pGApi = MemSys::NewT<GraphicsApiCout>();
			break;
#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case (GAPI_OPENGL):
			pGApi = MemSys::NewT<GraphicsApiOgl>();
			break;
#endif // NW_GRAPHICS
		default:
			NW_ERR("This graphics API is not defined");
			break;
		}
		return pGApi;
	}
}
#endif	// NW_GRAPHICS

#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
#if defined NW_GL_USE_GLAD
#include <glad/glad.h>
#endif	// NW_GL
namespace NW
{
	GraphicsApiOgl::GraphicsApiOgl() :
		m_gapiType(GAPI_OPENGL),
		m_unPrimitiveTypeId(PT_TRIANGLES),
		m_pGContext(nullptr) {}
	GraphicsApiOgl::~GraphicsApiOgl() = default;

	// ========<Setters>========
	void GraphicsApiOgl::SetModes(bool bEnable, ProcModes unModeId) {
		if (bEnable) { glEnable(unModeId); }
		else { glDisable(unModeId); }
	}
	void GraphicsApiOgl::SetClearColor(const Pixel& rpxColor) { glClearColor(rpxColor.fR(), rpxColor.fG(), rpxColor.fB(), rpxColor.fA()); }
	void GraphicsApiOgl::SetClearColor(float nRed, float nGreen, float nBlue, float nAlpha) { glClearColor(nRed, nGreen, nBlue, nAlpha); }
	void GraphicsApiOgl::SetViewport(Int32 nX, Int32 nY, Int32 nW, Int32 nH) { glViewport(nX, nY, nW, nH); }
	void GraphicsApiOgl::SetDrawMode(DrawModes unDrawModeId, FacePlanes unFacePlaneId) { m_unDrawModeId = unDrawModeId; glPolygonMode(unFacePlaneId, m_unDrawModeId); }
	void GraphicsApiOgl::SetLineWidth(UInt16 unPxW) { glLineWidth(unPxW); }
	void GraphicsApiOgl::SetPixelSize(float unPixelSize) { glPointSize(unPixelSize); }
	void GraphicsApiOgl::SetBlendFunc(BlendConfigs unSrcFactorId, BlendConfigs unDestFactorId) { glBlendFunc(unSrcFactorId, unDestFactorId); }
	void GraphicsApiOgl::SetDepthFunc(DepthConfigs unDepthFuncId) { glDepthFunc(unDepthFuncId); }
	void GraphicsApiOgl::SetStencilFunc(StencilConfigs unFuncId, UInt32 unRefValue, UInt8 unBitMask) { glStencilFunc(unFuncId, unRefValue, unBitMask); }
	// ========</Setters>========

	// ========<Drawing>========
	void GraphicsApiOgl::DrawIndexed(UInt32 unIndexCount) { glDrawElements(m_unPrimitiveTypeId, unIndexCount, GL_UNSIGNED_INT, nullptr); }
	void GraphicsApiOgl::DrawArray(UInt32 unVertexCount) { glDrawArrays(m_unPrimitiveTypeId, 0, unVertexCount); }
	// ========</Drawing>========

}
#endif // NW_GRAPHICS

#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
#include <cn/nw_engine.h>
#include <window/cn_CoutWindow.h>

#include <gl/cout/cout_gTypes.h>

#include <window/cout_FrameBuf.h>
#include <window/cout_GContext.h>

namespace NW
{
	// Constructor&Destructor
	GraphicsApiCout::GraphicsApiCout() :
		AGraphicsApi(),
		m_unClearColor(0), m_ucPattern(PxCharTypes::PX_SOLID)
	{
		m_pWindow = dynamic_cast<CoutWindow*>(&NW_Engine::Get().GetWindow());
		m_pGContext = m_pWindow->GetGContextCout();
		m_pBuffer = m_pGContext->GetFrameBuf();
	}
	GraphicsApiCout::~GraphicsApiCout()
	{
	}

	// Getters
	// Setters

	// ========<Drawing>========
	/// Algorythm of data usage:
	/// -> Create VertexBuffer -> Load vertex data
	/// -> Vertex data saves as a char array which will be casted
	/// -> Set Buffer elements to the Buffer Layout of the vertex buffer
	/// -> Add that vertex buffer to a vertex array
	/// -> Create Index Buffer -> Load index data
	/// -> Add the index buffer to a vertex array
	/// -> Give that vertex array to the renderer
	/// -> Renderer gives the vertex array to the GraphicsAPI
	/// -> GraphicsApiCout takes vertex array pointers to vertex buffers
	/// -> GraphicsApiCout itterates throught all the buffer layou of every vb
	/// -> Depending on the element type, count and buffer layout stride
	/// copy data into temp buffers
	/// -> Temporary buffers for different types will be filled due to loop
	/// -> That loops iterator goes in buffer layout stride for iteration
	/// -> Every iteration we copy the bytes from the buffer into our draw buffers
	/// -> Indicies just gets copied from the index buffer into the temp UInt buffer
	void GraphicsApiCout::DrawIndexed(AVertexArr* pVtxArray)
	{
		IndexBufCout* pIBuf = dynamic_cast<IndexBufCout*>(pVtxArray->GetIBuffer());
		VertexArrCout* pVArr = dynamic_cast<VertexArrCout*>(pVtxArray);
		if (pIBuf == nullptr || pVArr == nullptr) return;
		DArray<Int16> siVData;

		for (auto vb : pVArr->GetVBuffers())
		{
			VertexBufCout* pVBuf = dynamic_cast<VertexBufCout*>(vb);
			if (pVBuf == nullptr) return;
			auto& tempArr = pVBuf->GetVData();
			for (UInt16 vtByte = 0; vtByte < tempArr.size();
				vtByte += pVBuf->GetLayout().unStride)
			{
				siVData.push_back(tempArr[vtByte]);
			}
		}
		FillRect(siVData[0], siVData[1], siVData[2], siVData[3],
			PxInt(BG_BLUE));
	}

	void GraphicsApiCout::DrawArray(AVertexArr* pVtxArray)
	{
		//
	}

	void GraphicsApiCout::DrawLine(vsi2d xy1, vsi2d xy2, PxInt pxiColor)
	{
	}

	// --Drawing in the buffer
	void GraphicsApiCout::DrawRect(vsi4d viRect, PxInt pxiColor)
	{
		//
	}
	void GraphicsApiCout::DrawRect(Int16 nX, Int16 nY, Int16 nW, Int16 nH,
		PxInt pxiColor)
	{
		if (!m_pBuffer->IsInBounds({ nX, nY, nW, nH })) return;
		vsi4d rect = GetAbsRect(nX, nY, nW, nH);
		for (UInt16 pxY = rect.Top; pxY < rect.Bottom; pxY++)
		{
			for (UInt16 pxX = rect.Left; pxX < rect.Right; pxX++)
			{
				m_pBuffer->SetPixelXY(pxX, pxY, pxiColor.unColor, m_ucPattern);
			}
		}
	}

	void GraphicsApiCout::FillRect(vsi4d xywhRect, PxInt pxiColor)
	{
		FillRect(xywhRect.Left, xywhRect.Top, xywhRect.Right, xywhRect.Bottom, pxiColor);
	}

	void GraphicsApiCout::FillRect(Int16 nX, Int16 nY, Int16 nW, Int16 nH, PxInt pxiColor)
	{
		if (!m_pBuffer->IsInBounds({ nX, nY, nW, nH })) return;
		vsi4d rect = GetAbsRect(nX, nY, nW, nH);
		for (UInt16 pxY = rect.Top; pxY < rect.Bottom; pxY++)
		{
			for (UInt16 pxX = rect.Left; pxX < rect.Right; pxX++)
			{
				m_pBuffer->SetPixelXY(pxX, pxY, pxiColor.unColor, m_ucPattern);
			}
		}
	}

	void GraphicsApiCout::DrawImage(vsi2d xyCoord, Image* pImage, Int16 nSize)
	{
		//
	}
	// --Drawing in the buffer
	// ========</Drawing>========

	// ========<Configurations>========
	void GraphicsApiCout::ClearBuffers(UInt32 uiBufBitMask)
	{
		if (uiBufBitMask & NW_BUFFER_COLOR_BIT)
			// m_pBuffer->Clear(m_unClearColor);
			if (uiBufBitMask & NW_BUFFER_DEPTH_BIT);
	}
	void GraphicsApiCout::SetViewport(int iX, int iY, int iW, int iH)
	{
	}
	void GraphicsApiCout::SetBlend(bool bEnableBlending, UInt32 ucPatternFace, UInt32 ucPatternBack)
	{
		if (bEnableBlending)
		{
		}
		if (ucPatternFace != 0 && ucPatternBack != 0)
		{
			m_ucPattern = ucPatternFace;
		}
	}
	void GraphicsApiCout::SetDepth(bool bEnableDepth, UInt32 uiDepthBit)
	{}
	// ========</Configurations>========

	// ========<Implementation Methods>========
	inline void GraphicsApiCout::GetAbsRect(vsi4d& xywhRect)
	{
		if (xywhRect.Left < xywhRect.Right)
		{
			UInt16 temp = xywhRect.Left;
			xywhRect.Left = xywhRect.Right;
			xywhRect.Right = temp;
		}
		if (xywhRect.Top > xywhRect.Bottom)
		{
			UInt16 temp = xywhRect.Top;
			xywhRect.Top = xywhRect.Bottom;
			xywhRect.Bottom = temp;
		}
	}
	inline vsi4d GraphicsApiCout::GetAbsRect(UInt16 nX, UInt16 nY, UInt16 nW, UInt16 nH)
	{
		vsi4d rect;
		if (nX < nW)
		{
			rect.Left = nX;
			rect.Right = nW;
		}
		else
		{
			rect.Left = nW;
			rect.Right = nX;
		}
		if (nY < nH)
		{
			rect.Top = nY;
			rect.Bottom = nH;
		}
		else
		{
			rect.Top = nH;
			rect.Bottom = nY;
		}
		return rect;
	}
	// ========</Implementation Methods>========
}
#endif // NW_GRAPHICS