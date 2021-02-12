#include <nw_pch.hpp>
#include "gfx/gfx_api.h"
#if (defined NW_GAPI)
#include <gfx/gfx_tools.h>
#include <gfx/gfx_framebuf.h>
#include <gfx/gfx_drawable.h>
#include <gfx/gfx_camera.h>
#include <gfx/gfx_shader.h>
#include <gfx/gfx_material.h>
#include <gfx/gfx_texture.h>
#include <gfx/gfx_loader.h>
#include <../src_glsl/shd_screen.hpp>
#include <core/nw_window.h>
#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	GfxApi::GfxApi(CoreWindow& rWindow) :
		m_gInfo(GfxInfo()), m_gConfig(GfxConfig()),
		m_pWindow(&rWindow),
		m_pTarget(nullptr) { }
	GfxApi::~GfxApi() { }

	// --setters
	void GfxApi::SetModes(Bit bEnable, ProcessingModes pModes) {
		
		switch (pModes) {
		case PM_BLEND:
			m_gConfig.Blending.bEnable = bEnable;
			break;
		case PM_DEPTH_TEST:
			m_gConfig.DepthTest.bEnable = bEnable;
			break;
		case PM_CULL_FACE:
			m_gConfig.Culling.bEnable = bEnable;
			break;
		default: return; break;
		}
		if (bEnable) {
			glEnable(pModes);
		}
		else {
			glDisable(pModes);
		}
	}
	void GfxApi::SetViewport(Int32 nX, Int32 nY, Int32 nW, Int32 nH) {
		glViewport(nX, nY, nW, nH);
	}
	void GfxApi::SetDrawMode(DrawModes dMode, FacePlanes facePlane) {
		m_gConfig.General.PolyMode.dMode = dMode;
		m_gConfig.General.PolyMode.facePlane = facePlane;
		glPolygonMode(facePlane, dMode);
	}
	void GfxApi::SetLineWidth(Float32 nLineWidth) {
		m_gConfig.General.nLineWidth = nLineWidth;
		glLineWidth(nLineWidth);
	}
	void GfxApi::SetPixelSize(Float32 nPxSize) {
		m_gConfig.General.nPixelSize = nPxSize;
		glPointSize(nPxSize);
	}
	void GfxApi::SetBlendFunc(BlendConfigs bcSrcFactor, BlendConfigs bcDestFactor) {
		m_gConfig.Blending.FactorSrc = bcSrcFactor;
		m_gConfig.Blending.FactorDest = bcDestFactor;
		glBlendFunc(bcSrcFactor, bcDestFactor);
	}
	void GfxApi::SetDepthFunc(DepthConfigs dcFunc) {
		m_gConfig.DepthTest.Func = dcFunc;
		glDepthFunc(dcFunc);
	}
	void GfxApi::SetStencilFunc(StencilConfigs scFunc, UInt32 unRefValue, UInt8 unBitMask) {
		m_gConfig.StencilTest.Func = scFunc;
		m_gConfig.StencilTest.nBitMask = unBitMask;
		m_gConfig.StencilTest.nRefValue = unRefValue;
		glStencilFunc(scFunc, unRefValue, unBitMask);
	}
	void GfxApi::SetFrameBuf(FrameBuf* pfmBuf) {
		m_pTarget = pfmBuf;
		NWL_ASSERT(pfmBuf->GetAttachment(0) != nullptr, "There must be at least one attachment!");
		m_drbScreen.gMtl->SetTexture(m_pTarget->GetAttachment(0));
	}
	// --==<core_methods>==--
	bool GfxApi::Init()
	{
		if (!gladLoadGL()) { NWL_ERR("Failed to load graphics library!"); return false; }
		{
			String str = ((const char*)glGetString(GL_RENDERER));
			strcpy(&m_gInfo.strRenderer[0], &str[0]);
			str = ((const char*)glGetString(GL_VERSION));
			strcpy(&m_gInfo.strVersion[0], &str[0]);
			str = ((const char*)glGetString(GL_VENDOR));
			strcpy(&m_gInfo.strVendor[0], &str[0]);
			str = ((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
			strcpy(&m_gInfo.strShdLang[0], &str[0]);
		}
		std::cout << m_gInfo;
		{	// textures
			Texture* pTex = nullptr;
			ImageInfo imgInfo;
			TextureInfo texInfo;
			UByte whiteData[] = { 255,	255,	255,	255 };
			imgInfo.pClrData = whiteData;
			imgInfo.nChannels = 1;
			imgInfo.nWidth = imgInfo.nHeight = 1;
			texInfo.bGenMipmap = false;
			texInfo.bGenSubImage= false;
			texInfo.FilterMag = texInfo.FilterMin = TXF_NEAREST;
			texInfo.pxFormat = PXF_UINT32;
			texInfo.texFormat = TXF_RGBA; texInfo.texInterFormat = TXFI_RGBA8;
			texInfo.unSamples = 1;
			texInfo.WrapTypeS = texInfo.WrapTypeT = texInfo.WrapTypeR = TXW_REPEAT;
			pTex = DataSys::GetDR<Texture>(DataSys::NewDR<Texture>("tex_white_solid", TXT_2D));
			pTex->SetInfo(imgInfo);
			pTex->SetInfo(texInfo);
			pTex->Remake();
		}
		{	// screen drawable info
			m_drbScreen.vtxArr.MakeRef<VertexArr>();
			Float32 vtxData[] = {
				-1.0f,	-1.0f,		0.0f,	0.0f,
				-1.0f,	1.0f,		0.0f,	1.0f,
				1.0f,	1.0f,		1.0f,	1.0f,
				1.0f,	-1.0f,		1.0f,	0.0f,
			};
			m_drbScreen.vtxArr->CreateVtxBuffer();
			m_drbScreen.vtxArr->GetVtxBuffer()->SetData(sizeof(vtxData), &vtxData[0]);
			UInt32 idxData[] = { 0,	1,	2,		2,	3,	0 };
			m_drbScreen.vtxArr->CreateIdxBuffer();
			m_drbScreen.vtxArr->GetIdxBuffer()->SetData(sizeof(idxData), &idxData[0]);
			{
				Shader* pShader = DataSys::GetDR<Shader>(DataSys::NewDR<Shader>("shd_screen_0"));
				pShader->SetCode(&strScreenShaderCode[0]);
				if (!pShader->Compile()) { OnQuit(); return false; }

				m_drbScreen.gMtl = DataSys::GetDR<GfxMaterial>(DataSys::NewDR<GfxMaterial>("gmt_screen_0"));
				m_drbScreen.gMtl->SetShader(pShader);
				m_drbScreen.vtxArr->Remake(pShader->GetVtxLayout());
			}
		}
		{	// screen framebuffer
			FrameBufInfo fbInfo;
			fbInfo.rectViewport = { 0, 0, 1200, 800 };

			if (true) {
				FrameBuf* pfmBuf;
				Texture* pTex = nullptr;
				TextureInfo texInfo;
				
				pfmBuf = DataSys::GetDR<FrameBuf>(DataSys::NewDR<FrameBuf>("fmb_draw_0", fbInfo));
				{
					Texture* pTex = DataSys::GetDR<Texture>(DataSys::NewDR<Texture>("fba_draw_color_0", TXT_2D));
					texInfo.bGenMipmap = true; texInfo.bGenSubImage = true;
					texInfo.unSamples = 1;
					texInfo.pxFormat = PXF_UINT8;
					texInfo.texFormat = TXF_RGB; texInfo.texInterFormat = TXFI_RGB;
					pTex->SetInfo(texInfo);
					pfmBuf->AttachTexture(*pTex);
				}
				{
					Texture* pTex = DataSys::GetDR<Texture>(DataSys::NewDR<Texture>("fba_draw_depth_stencil_0", TXT_2D));
					texInfo.bGenMipmap = true; texInfo.bGenSubImage = true;
					texInfo.unSamples = 1;
					texInfo.pxFormat = PXF_UINT24_8;
					texInfo.texFormat = TXF_DEPTH_STENCIL; texInfo.texInterFormat = TXFI_DEPTH24_STENCIL8;
					pTex->SetInfo(texInfo);
					pfmBuf->AttachTexture(*pTex);
				}

				pfmBuf->Remake();
				SetFrameBuf(pfmBuf);
			}
		}
		glClearColor(0.5f, 0.0f, 1.0f, 1.0f);

		return true;
	}
	void GfxApi::OnQuit()
	{
		if (m_pWindow == nullptr) { return; }
		m_drbScreen = Drawable();

		DataSys::GetStorage<Shader>().clear();
		DataSys::GetStorage<GfxMaterial>().clear();
		DataSys::GetStorage<FrameBuf>().clear();
		DataSys::GetStorage<Texture>().clear();
		
		m_pWindow = nullptr;
	}

	void GfxApi::Update()
	{
	}
	void GfxApi::BeginDraw() {
		m_pTarget->Bind();
	}
	void GfxApi::EndDraw() {
		m_pTarget->Unbind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		OnDraw(m_drbScreen);
	}

	void GfxApi::OnDraw(VertexArr& rVtxArr, GfxMaterial& rgMtl) {
		rgMtl.Enable();
		rVtxArr.Bind();
		if (rVtxArr.GetIdxBuffer() != nullptr) {
			glDrawElements(rVtxArr.GetDrawPrimitive(), rVtxArr.GetIdxBuffer()->GetDataSize() / sizeof(UInt32), GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawArrays(rVtxArr.GetDrawPrimitive(), 0, rVtxArr.GetVtxBuffers().size());
		}
		rVtxArr.Unbind();
		rgMtl.Disable();
	}
	void GfxApi::OnDraw(Drawable& rDrb) {
		rDrb.gMtl->Enable();
		rDrb.vtxArr->Bind();
		if (rDrb.vtxArr->GetVtxBuffer() != nullptr) {
			glDrawElements(rDrb.vtxArr->GetDrawPrimitive(), rDrb.vtxArr->GetIdxBuffer()->GetDataSize() / sizeof(UInt32), GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawArrays(rDrb.vtxArr->GetDrawPrimitive(), 0, rDrb.vtxArr->GetVtxBuffers().size());
		}
		rDrb.vtxArr->Unbind();
		rDrb.gMtl->Disable();
	}
	// --==<implementation_methods>==--
	// --==</implementation_methods>==--
}
#endif
#if (NW_GAPI & NW_GAPI_DX)
namespace NW
{
	GfxApi::GfxApi(CoreWindow& rWindow) :
		m_gInfo(GfxInfo()), m_gConfig(GfxConfig()),
		m_pWindow(&rWindow), m_bIsDrawing(false),
		m_pDevice(nullptr), m_pSwap(nullptr) { }
	GfxApi::~GfxApi() { }

	// --setters
	void GfxApi::SetModes(Bit bEnable, ProcessingModes pModes) {
		switch (pModes) {
		case PM_BLEND:
			m_gConfig.Blending.bEnable = bEnable;
			break;
		case PM_DEPTH_TEST:
			m_gConfig.DepthTest.bEnable = bEnable;
			break;
		case PM_CULL_FACE:
			m_gConfig.Culling.bEnable = bEnable;
			break;
		default: return; break;
		}
		if (bEnable) {
		}
		else {
		}
	}
	void GfxApi::SetViewport(Int32 nX, Int32 nY, Int32 nW, Int32 nH) {
	}
	void GfxApi::SetDrawMode(DrawModes dMode, FacePlanes facePlane) {
		m_gConfig.General.PolyMode.dMode = dMode;
		m_gConfig.General.PolyMode.facePlane = facePlane;
	}
	void GfxApi::SetLineWidth(Float32 nLineWidth) {
		m_gConfig.General.nLineWidth = nLineWidth;
	}
	void GfxApi::SetPixelSize(Float32 nPxSize) {
		m_gConfig.General.nPixelSize = nPxSize;
	}
	void GfxApi::SetBlendFunc(BlendConfigs bcSrcFactor, BlendConfigs bcDestFactor) {
		m_gConfig.Blending.FactorSrc = bcSrcFactor;
		m_gConfig.Blending.FactorDest = bcDestFactor;
	}
	void GfxApi::SetDepthFunc(DepthConfigs dcFunc) {
		m_gConfig.DepthTest.Func = dcFunc;
	}
	void GfxApi::SetStencilFunc(StencilConfigs scFunc, UInt32 unRefValue, UInt8 unBitMask) {
		m_gConfig.StencilTest.Func = scFunc;
		m_gConfig.StencilTest.nBitMask = unBitMask;
		m_gConfig.StencilTest.nRefValue = unRefValue;
	}
	void GfxApi::SetFrameBuf(FrameBuf* pfmBuf) {
	}
	// --==<core_methods>==--
	bool GfxApi::Init()
	{
		if (m_pWindow == nullptr) { return false; }

		if (!InitDevice()) { return false; }

		ID3D11Resource* pdrBackBuf = nullptr;
		m_pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<Ptr*>(&pdrBackBuf));
		m_pDevice->CreateRenderTargetView(pdrBackBuf, nullptr, &m_pTarget);
		pdrBackBuf->Release();

		std::cout << m_gInfo;
		return true;
	}
	void GfxApi::OnQuit()
	{
		m_pTarget->Release();
		m_pContext->Release();
		QuitDevice();
	}

	void GfxApi::Update()
	{
		const Float32 rgbaColor[] = { cosf(static_cast<Float32>(TimeSys::GetCurrS())), sinf(static_cast<Float32>(TimeSys::GetCurrS())), 0.3f, 1.0f };
		m_pSwap->Present(1u, 0u);
		m_pContext->ClearRenderTargetView(m_pTarget, rgbaColor);
	}
	void GfxApi::BeginDraw()
	{
		NWL_ASSERT(!m_bIsDrawing, "Is already drawing");
		m_bIsDrawing = true;
	}
	void GfxApi::EndDraw()
	{
		NWL_ASSERT(m_bIsDrawing, "Is not drawing now");
		m_bIsDrawing = false;
	}

	void GfxApi::OnDraw(VertexArr& rVtxArr, GfxMaterial& rgMtl) {
	}
	void GfxApi::OnDraw(Drawable& rDrb) {
	}
	// --==<implementation_methods>==--
	inline bool GfxApi::InitDevice() {
		DXGI_SWAP_CHAIN_DESC swapDesc{ 0 };
		// describe a swapchain
		swapDesc.BufferDesc.Width = 0;
		swapDesc.BufferDesc.Height = 0;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapDesc.BufferDesc.RefreshRate.Denominator = 0;
		swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount = 1;
		swapDesc.OutputWindow = m_pWindow->GetNative();
		swapDesc.Windowed = TRUE;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapDesc.Flags = 0;

		D3D11CreateDeviceAndSwapChain(
			nullptr,					// default adapter
			D3D_DRIVER_TYPE_HARDWARE,	// hardware device
			nullptr,					// the hmodule binary we want to load
			NULL,						// flags
			nullptr,					// any feature levels
			NULL,						// any levels...
			D3D11_SDK_VERSION,			// soft development kit version of the system
			&swapDesc,					// descriptor for the swap chain
			&m_pSwap,					// 
			&m_pDevice,					// 
			nullptr,					// output feature level pointer
			&m_pContext					// prt to prt for the context
		);

		return true;
	}
	inline void GfxApi::QuitDevice() {
		m_pSwap->Release();
		m_pDevice->Release();
	}
	// --==</implementation_methods>==--
}
#endif
#endif	// NW_GAPI

/// OnDraw (AShape)
	/// Description:
	/// -- Shapes have a vertex data, indices and bound AGfxMaterial
	/// -- GfxApi collects all the Vertex and Index data into some arrays from given object
	/// -- AGfxMaterial of the object provides GfxApi's AGfxMaterial with textures
	/// -- Every new texture from new AGfxMaterial will be bound to the current gMtl
	/// -- In the EndDraw it will bind all owned textures and will set their samplerIDs to the own shader
	/// -- There is also default white texture which will be used if we don not have other textures