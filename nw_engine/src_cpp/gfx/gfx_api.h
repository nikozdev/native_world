#ifndef NW_GFX_API_H
#define NW_GFX_API_H

#include <gfx/gfx_tools.h>
#include <gfx/gfx_drawable.h>
#include <gfx/gfx_material.h>
#include <gfx_core.hpp>

#if (defined NW_GAPI)
namespace NW
{
	struct NW_API GfxConfig {
		struct {
			struct {
				DrawModes dMode = DM_FILL;
				FacePlanes facePlane = FACE_DEFAULT;
			} PolyMode;
			Float32 nLineWidth = 0.5f;
			Float32 nPixelSize = 0.5f;
		} General;
		struct {
			Bit bEnable = false;
			BlendConfigs FactorSrc = BC_SRC_ALPHA;
			BlendConfigs FactorDest = BC_ONE_MINUS_SRC_ALPHA;
		} Blending;
		struct {
			Bit bEnable = false;
			CullFaceConfigs CullFactor = CFC_DEFAULT;
		} Culling;
		struct {
			Bit bEnable = false;
			DepthConfigs Func = DTC_DEFAULT;
		} DepthTest;
		struct {
			Bit bEnable = false;
			StencilConfigs Func = STC_DEFAULT;
			UInt8 nBitMask = 0x0;
			UInt32 nRefValue = 0x0;
		} StencilTest;
	public:
		inline void Reset() {
			General.PolyMode.dMode = DM_FILL;
			General.PolyMode.facePlane = FACE_DEFAULT;

			General.nLineWidth = 0.5f;
			General.nPixelSize = 0.5f;

			Blending.bEnable = false;
			Blending.FactorSrc = BC_SRC_ALPHA;
			Blending.FactorDest = BC_ONE_MINUS_SRC_ALPHA;

			DepthTest.bEnable = false;
			DepthTest.Func = DTC_GREATER;
		}
	};
	/// GraphicsApiInfo struct
	struct NW_API GfxInfo
	{
	public:
		Char strRenderer[256], strVersion[256], strVendor[256], strShdLang[256];
		Int32 nMaxVertexAttribs = 0;
		Int32 nActiveTextureId = 0;
		Int32 nMaxTextures = 0;
		// --counters
		Size szVtx = 0;
		Size unVtx = 0;
		Size szIdx = 0;
		Size unIdx = 0;
		Size szShd = 0;
		UInt32 unTex = 0;
		// --drawing
		UInt16 unDrawCalls = 0;
	public:
		GfxInfo() : strRenderer("none"), strVersion("none"), strVendor("none"), strShdLang("none") {}
		// --setters
		inline void Reset() {
			szVtx = szIdx = szShd = 0;
			unVtx = unIdx = unTex = 0;
			unDrawCalls = 0;
		}
		// --operators
		inline OutStream& operator<<(OutStream& rStream);
	};
	inline OutStream& GfxInfo::operator<<(OutStream& rStream) {
		rStream << "--==<graphics_info>==--" << std::endl <<
			"graphics context: " << &strVersion[0] << std::endl <<
			"renderer: " << &strRenderer[0] << std::endl <<
			"version: " << &strVersion[0] << std::endl <<
			"vendor: " << &strVendor[0] << std::endl <<
			"shading language: " << &strShdLang[0] << std::endl <<
			"--==</graphics_info>==--" << std::endl;
		return rStream;
	}
	inline OutStream& operator<<(OutStream& rStream, GfxInfo& rgInfo) { return rgInfo.operator<<(rStream); }
}
namespace NW
{
	/// GraphicsApi class
	class NW_API GfxApi
	{
	public:
		GfxApi(CoreWindow& rWindow);
		GfxApi(const GfxApi& rCpy) = delete;
		~GfxApi();
		// --getters
		inline const GfxInfo& GetInfo() const { return m_gInfo; }
		inline const GfxConfig& GetConfigs() { return m_gConfig; }
		inline FrameBuf* GetFrameBuf() { return m_pfb; }
		// --setters
		void SetModes(Bit bEnable, ProcessingModes pModes);
		void SetViewport(Int32 nX, Int32 nY, Int32 nWidth, Int32 nHeight);
		void SetDrawMode(DrawModes dMode, FacePlanes facePlane);
		void SetLineWidth(Float32 nLineWidth);
		void SetPixelSize(Float32 nPxSize);
		void SetBlendFunc(BlendConfigs factorSrc, BlendConfigs factorDest);
		void SetDepthFunc(DepthConfigs funcId);
		void SetStencilFunc(StencilConfigs funcId, UInt32 unRefValue, UInt8 unBitMask);
		void SetFrameBuf(FrameBuf* pfmBuf);
		// --predicates
		inline bool IsDrawing() { return m_bIsDrawing; }
		// --operators
		inline void operator=(const GfxApi& rCpy) = delete;
		inline void operator delete(Ptr pBlock) = delete;
		inline void operator delete[](Ptr pBlock) = delete;
		// --core_methods
		bool Init();
		void OnQuit();
		void Update();
		void BeginDraw();
		void EndDraw();
		void OnDraw(VertexArr& rVtxArray, GfxMaterial& rgMtl);
		void OnDraw(Drawable& rDrb);
	private:
		inline bool InitDevice();
		inline void QuitDevice();
	private:
		GfxInfo m_gInfo;
		GfxConfig m_gConfig;
		CoreWindow* m_pWindow;
		Bit m_bIsDrawing;

		FrameBuf* m_pfb;
#if (NW_GAPI & NW_GAPI_OGL)
		Drawable m_drbScreen;
#endif
#if (NW_GAPI & NW_GAPI_DX)
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pContext;
		IDXGISwapChain* m_pSwap;
		ID3D11RenderTargetView* m_pTarget;
#endif
	};
}
#endif	// NW_GAPI
#endif // NW_GFX_API_H