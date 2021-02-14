#ifndef NW_GFX_ENGINE_H
#define NW_GFX_ENGINE_H

#include <nw_core.hpp>
#include <gfx_core.hpp>
#include <gfx/gfx_tools.h>

#if (defined NW_GAPI)
namespace NW
{
	/// abstract GraphicsContext class
	/// Description:
	/// --This is entire state of graphics API we use
	/// --The entire OpenGL/DirectX state is here
	class NW_API GfxEngine
	{
	public:
		GfxEngine();
		GfxEngine(const GfxEngine& rCpy) = delete;
		virtual ~GfxEngine();
		// --getters
		inline const GfxInfo& GetInfo() const { return m_gInfo; }
		inline const GfxConfig& GetConfigs() { return m_gConfig; }
		// --setters
		virtual void SetPrimitive(GfxPrimitiveTypes gfxPrimitiveTopology) = 0;
		virtual void SetModes(Bit bEnable, ProcessingModes pmMode) = 0;
		virtual void SetViewport(Int32 nX, Int32 nY, Int32 nWidth, Int32 nHeight) = 0;
		virtual void SetDrawMode(DrawModes dmMode, FacePlanes fpFace) = 0;
		virtual void SetLineWidth(Float32 nLineWidth) = 0;
		virtual void SetPixelSize(Float32 nPxSize) = 0;
		virtual void SetBlendFunc(BlendConfigs bcFactorSrc, BlendConfigs bcFactorDest) = 0;
		virtual void SetDepthFunc(DepthConfigs dcFunc) = 0;
		virtual void SetStencilFunc(StencilConfigs scFunc, UInt32 unRefValue, UInt8 unBitMask) = 0;
		// --operators
		inline void operator=(const GfxEngine& rCpy) = delete;
		inline void operator delete(Ptr pBlock) = delete;
		inline void operator delete[](Ptr pBlock) = delete;
		// --core_methods
		virtual void Update() = 0;
		virtual void CreateVtxBuf(RefKeeper<VertexBuf>& rBuf) = 0;
		virtual void CreateIdxBuf(RefKeeper<IndexBuf>& rBuf) = 0;
		virtual void CreateShdBuf(RefKeeper<ShaderBuf>& rBuf) = 0;
		virtual void CreateTexture(RefKeeper<Texture>& rTexture, const char* strName, TextureTypes texType) = 0;
		virtual void CreateFrameBuf(RefKeeper<FrameBuf>& rFrameBuf, const char* strName, FrameBufInfo& rInfo) = 0;
		virtual void CreateShader(RefKeeper<Shader>& rShader, const char* strName) = 0;
		static void Create(RefKeeper<GfxEngine>& rEngine, NativeWindow& rWindow, GfxApiTypes gapiType = GAPI_DEFAULT);
	protected:
		GfxInfo m_gInfo;
		GfxConfig m_gConfig;
	};
}
#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	class NW_API GfxEngineOgl : public GfxEngine
	{
	public:
		GfxEngineOgl(HWND pWindow);
		virtual ~GfxEngineOgl();
		// --getters
		static inline GfxEngineOgl& Get()	{ return *s_pGfx; }
		inline HDC GetDevice()				{ return m_pDevice; }
		inline HGLRC GetContext()			{ return m_pContext; }
		// --setters
		virtual void SetPrimitive(GfxPrimitiveTypes gfxPrimitiveTopology) override;
		virtual void SetModes(Bit bEnable, ProcessingModes pmMode) override;
		virtual void SetViewport(Int32 nX, Int32 nY, Int32 nWidth, Int32 nHeight) override;
		virtual void SetDrawMode(DrawModes dmMode, FacePlanes fpFace) override;
		virtual void SetLineWidth(Float32 nLineWidth) override;
		virtual void SetPixelSize(Float32 nPxSize) override;
		virtual void SetBlendFunc(BlendConfigs bcFactorSrc, BlendConfigs bcFactorDest) override;
		virtual void SetDepthFunc(DepthConfigs dcFunc) override;
		virtual void SetStencilFunc(StencilConfigs scFunc, UInt32 unRefValue, UInt8 unBitMask) override;
		// --core_methods
		virtual void Update() override;
		virtual void CreateVtxBuf(RefKeeper<VertexBuf>& rBuf) override;
		virtual void CreateIdxBuf(RefKeeper<IndexBuf>& rBuf) override;
		virtual void CreateShdBuf(RefKeeper<ShaderBuf>& rBuf) override;
		virtual void CreateFrameBuf(RefKeeper<FrameBuf>& rFrameBuf, const char* strName, FrameBufInfo& rInfo) override;
		virtual void CreateShader(RefKeeper<Shader>& rShader, const char* strName) override;
	private:
		HWND m_pWindow;

		HDC m_pDevice;
		HGLRC m_pContext;
		
		static GfxEngineOgl* s_pGfx;
	};
}
#endif
#if (NW_GAPI & NW_GAPI_DX)
namespace NW
{
	class NW_API GfxEngineDx : public GfxEngine
	{
	public:
		GfxEngineDx(HWND pWindow);
		virtual ~GfxEngineDx();
		// --getters
		static inline GfxEngineDx& Get()		{ return *s_pGfx; }
		inline ID3D11Device* GetDevice()		{ return m_pDevice; }
		inline ID3D11DeviceContext* GetContext(){ return m_pContext; }
		// --setters
		virtual void SetPrimitive(GfxPrimitiveTypes gfxPrimitiveTopology) override;
		virtual void SetModes(Bit bEnable, ProcessingModes pmMode) override;
		virtual void SetViewport(Int32 nX, Int32 nY, Int32 nWidth, Int32 nHeight) override;
		virtual void SetDrawMode(DrawModes dmMode, FacePlanes fpFace) override;
		virtual void SetLineWidth(Float32 nLineWidth) override;
		virtual void SetPixelSize(Float32 nPxSize) override;
		virtual void SetBlendFunc(BlendConfigs bcFactorSrc, BlendConfigs bcFactorDest) override;
		virtual void SetDepthFunc(DepthConfigs dcFunc) override;
		virtual void SetStencilFunc(StencilConfigs scFunc, UInt32 unRefValue, UInt8 unBitMask) override;
		// --core_methods
		virtual void Update() override;
		virtual void CreateVtxBuf(RefKeeper<VertexBuf>& rBuf) override;
		virtual void CreateIdxBuf(RefKeeper<IndexBuf>& rBuf) override;
		virtual void CreateShdBuf(RefKeeper<ShaderBuf>& rBuf) override;
		virtual void CreateTexture(RefKeeper<Texture>& rTex, const char* strName, TextureTypes texType) override;
		virtual void CreateFrameBuf(RefKeeper<FrameBuf>& rFrameBuf, const char* strName, FrameBufInfo& rInfo) override;
		virtual void CreateShader(RefKeeper<Shader>& rShader, const char* strName) override;
	private:
		HWND m_pWindow;

		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pContext;
		IDXGISwapChain* m_pSwap;
		ID3D11RenderTargetView* m_pTarget;

		ID3D11Buffer* m_pVtxBuf;
		ID3D11InputLayout* m_pVtxLayout;
		ID3D11VertexShader* m_pVtxShd;
		ID3D11PixelShader* m_pPxlShd;
		
		static GfxEngineDx* s_pGfx;
	};
}
#endif
#endif	// NW_GAPI
#endif	// NW_GFX_ENGINE_H