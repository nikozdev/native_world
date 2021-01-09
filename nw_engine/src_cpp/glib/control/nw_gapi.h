#ifndef NW_AGRAPHICS_API_H
#define NW_AGRAPHICS_API_H

#include <glib_decl.hpp>

// --==<AGraphicsApi>==--
#if (defined NW_GRAPHICS)	// This has to be defined anyway
namespace NW
{
	/// Abstract GraphicsApplicationProgrammingInterface class
	/// Interface:
	/// -> Create -> use functions as a wrappers of the partucular GAPI:
	/// -> ClearBuffers(): clear the frame buffers
	/// -> SetViewport(int [XYWH]):
	/// Description:
	/// -- This is the tool of the engine renderer
	/// -- Renderer owns an instance of the derrived from this class
	/// -- Renderer is static and he owns any API with which he can draw
	class NW_API AGraphicsApi
	{
	public:
		virtual ~AGraphicsApi() = default;
		
		// --getters
		virtual inline const GApiTypes GetType() const = 0;
		virtual inline const DrawModes GetDrawMode() const = 0;
		virtual inline const PrimitiveTypes GetPrimitiveType() const = 0;
		// --setters
		virtual void SetModes(bool bEnable, ProcModes unModeId) = 0;
		virtual void SetClearColor(const Pixel& rpxColor) = 0;
		virtual void SetClearColor(float nRed, float nGreen, float nBlue, float nAlpha = 1.0f) = 0;
		virtual void SetViewport(Int32 nX = 0, Int32 nY = 0, Int32 nW = 800, Int32 nH = 600) = 0;
		virtual void SetLineWidth(UInt16 unPxW) = 0;
		virtual void SetPixelSize(float unPixelSize) = 0;

		virtual void SetDrawMode(DrawModes unDrawModeId, FacePlanes unFacePlaneId) = 0;
		virtual void SetPrimitiveType(PrimitiveTypes unPrimitiveId) = 0;
		virtual void SetDepthFunc(DepthConfigs unFuncId) = 0;
		virtual void SetStencilFunc(StencilConfigs unFuncId, UInt32 unRefValue, UInt8 unBitMask) = 0;
		virtual void SetBlendFunc(BlendConfigs unSrcFactorId, BlendConfigs unDestFactorId) = 0;
		// --Drawing
		virtual void DrawIndexed(UInt32 unIndexCount) = 0;
		virtual void DrawArray(UInt32 unVertexCount) = 0;

		static AGraphicsApi* Create(GApiTypes gapiType);
	};
}
#endif	// NW_GRAPHICS
// --==</AGraphicsApi>==--

// --==<GraphicsApiOgl>==--
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
namespace NW
{
	/// OpenGL GraphicsApi class : public GraphicsApi_OGL
	/// --Wraps opengl functions
	/// --Draws everything you give him
	/// --Gets data and draws it
	/// --Can draw throught abstraction classes and VBO/VAO/EBO
	class NW_API GraphicsApiOgl : public AGraphicsApi
	{
	public:
		GraphicsApiOgl();
		~GraphicsApiOgl();

		// --getters
		virtual inline const GApiTypes GetType() const override { return m_gapiType; }
		virtual inline const DrawModes GetDrawMode() const override { return DrawModes(m_unDrawModeId); }
		virtual inline const PrimitiveTypes GetPrimitiveType() const override { return PrimitiveTypes(m_unPrimitiveTypeId); }
		// --setters
		virtual inline void SetPrimitiveType(PrimitiveTypes unPrimitiveId) override { m_unPrimitiveTypeId = unPrimitiveId; }
		virtual void SetDrawMode(DrawModes unDrawModeId, FacePlanes unFacePlaneId) override;
		
		virtual void SetModes(bool bEnable, ProcModes unModeId) override;
		virtual void SetClearColor(const Pixel& rpxColor) override;
		virtual void SetClearColor(float nRed, float nGreen, float nBlue, float nAlpha = 1.0f) override;
		virtual void SetViewport(Int32 nX = 0, Int32 nY = 0, Int32 nW = 800, Int32 nH = 600) override;
		virtual void SetLineWidth(UInt16 unPxW) override;
		virtual void SetPixelSize(float unPixelSize) override;

		virtual void SetDepthFunc(DepthConfigs unFuncId) override;
		virtual void SetStencilFunc(StencilConfigs unFuncId, UInt32 unRefValue, UInt8 unBitMask) override;
		virtual void SetBlendFunc(BlendConfigs unSrcFactorId, BlendConfigs unDestFactorId) override;

		// --Drawing
		virtual void DrawIndexed(UInt32 unIndexCount) override;
		virtual void DrawArray(UInt32 unVertexCount) override;
	private:
		AGContext* m_pGContext;
		const GApiTypes m_gapiType;
		UInt32 m_unPrimitiveTypeId;
		UInt32 m_unDrawModeId;
	};

}
#endif	// NW_GRAPHICS
// --==</GraphicsApiOgl>==--

// --==<GraphicsApiCout>==--
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
#include <window/cn_CoutWindow.h>
namespace NW
{
	/// Console GraphicsApi class
	class NW_API GraphicsApiCout : public AGraphicsApi
	{
	public: // Interface Methods
		GraphicsApiCout();
		~GraphicsApiCout();

		// Getters
		virtual inline const GApiTypes GetType() const { return GApiTypes::GRAPHICS_COUT; }
		// Setters
		virtual void SetClearColor(float fRed = 0.0f, float fGreen = 0.0f, float fBlue = 0.0f)
		{}
		virtual void SetClearColor(UInt32 unBytesRGBA)
		{
			m_unClearColor = unBytesRGBA;
		}
		virtual void SetDrawMode(UInt16 unModeID)
		{
			m_ucPattern = unModeID;
		}
		inline void SetCurrBuffer(CoutFrameBuf* pBuffer)
		{
			m_pBuffer = pBuffer;
		}

		// Interface Methods
		// --Drawing
		virtual void DrawIndexed(AVertexArr* pVertexArray) override;
		virtual void DrawArray(AVertexArr* pVertexArray) override;
		virtual void ClearBuffers(UInt32 uiBufBitMask = NW_BUFFER_COLOR_BIT) override;

		void DrawLine(vsi2d xy1, vsi2d xy2, PxInt pxiColor);
		void DrawRect(vsi4d xywhRect, PxInt pxiColor);
		void DrawRect(Int16 nX, Int16 nY, Int16 nW, Int16 unH, PxInt pxiColor);
		void FillRect(vsi4d xywhRect, PxInt pxiColor);
		void FillRect(Int16 nX, Int16 nY, Int16 nW, Int16 nH, PxInt pxiColor);
		void DrawImage(vsi2d xyLeftUpCoord, Image* pImage, Int16 nScale);
		// --Drawing

		// --Configurations
		virtual void SetViewport(int nX = 0, int nY = 0, int nW = 800, int nH = 600) override;
		virtual void SetBlend(bool bEnableBlending,
			UInt32 uiBlendBitS = 0, UInt32 uiBlendBitF = 0) override;
		virtual void SetDepth(bool bEnableDepth,
			UInt32 uiDepthBit = 0) override;
		// --Configurations
	private: // Implementation Methods
		// --Drawing Tools
		inline vsi4d GetAbsRect(UInt16 nX, UInt16 nY, UInt16 nW, UInt16 nH);
		inline void GetAbsRect(vsi4d& xywhRect);
		// --Drawing Tools
	private: // Implementation Fields
		CoutWindow* m_pWindow;
		GContextCout* m_pGContext;
		CoutFrameBuf* m_pBuffer;

		UInt32 m_unClearColor;
		wchar_t m_ucPattern;

		DArray<float> m_nVData;
		DArray<UInt32> m_unIData;
	};
}
#endif	// NW_GRAPHICS
// --==</GraphicsApiCout>==--

#endif // NW_AGRAPHICS_API_H