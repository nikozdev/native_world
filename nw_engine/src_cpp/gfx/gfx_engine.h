#ifndef GFX_ENGINE_H
#define GFX_ENGINE_H

#include <nwlib/nwl_engine.h>

#include <gfx_tools.h>
#include <gfx_drawable.h>
#include <gfx_material.h>
#include <gfx_core.hpp>

namespace NW
{
	struct GFX_API GfxConfig {
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
	/// GraphicsEngineInfo struct
	struct GFX_API GfxInfo
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
	/// GraphicsEngine class
	/// -- Depending on the specification during the build
	class GFX_API GfxEngine : public AEngine<GfxEngine, AEngineState>
	{
	public:
		GfxEngine();
		~GfxEngine();
		
		// --getters
		inline const GfxInfo& GetInfo() const { return m_Info; }
		inline const GfxConfig& GetConfigs() { return m_Config; }
		inline FrameBuf* GetFrameBuf() { return m_pfmBuf; }
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
		// --core_methods
		virtual void Run() override;
		virtual bool Init() override;
		virtual void Quit() override;
		virtual void Update() override;
		virtual void OnEvent(AEvent& rEvt) override;
		// --drawing_methods
		void OnDraw(VertexArr& rVtxArray, GMaterial& rgMtl);
		void OnDraw(Drawable& rDrb);

		void SetWindow(HWND pWindow);
	private:
		GfxInfo m_Info;
		GfxConfig m_Config;
		
		FrameBuf* m_pfmBuf;
		Drawable m_drbScreen;
#if (GFX_GAPI_DX)
		HWND m_pWindow;
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pContext;
		IDXGISwapChain* m_pSwap;
#endif	// GFX_GAPI
	};
}

#endif // GFX_ENGINE_H