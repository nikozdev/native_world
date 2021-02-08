#include <gfx_pch.hpp>
#include "gfx_engine.h"

#include <gfx_tools.h>
#include <gfx_framebuf.h>
#include <gfx_drawable.h>

#include <gfx_camera.h>
#include <gfx_shader.h>
#include <gfx_material.h>
#include <gfx_texture.h>

#include <../src_glsl/shd_screen.hpp>

#if (defined GFX_GAPI)
#include <gfx_loader.h>
#if (GFX_GAPI & GFX_GAPI_OGL)
namespace NW
{
	GfxEngine::GfxEngine() :
		AEngine(),
		m_Info(GfxInfo()), m_Config(GfxConfig()),
		m_pfmBuf(nullptr) { }
	GfxEngine::~GfxEngine() { }

	// --setters
	void GfxEngine::SetModes(Bit bEnable, ProcessingModes pModes) {
		switch (pModes) {
		case PM_BLEND:
			m_Config.Blending.bEnable = bEnable;
			break;
		case PM_DEPTH_TEST:
			m_Config.DepthTest.bEnable = bEnable;
			break;
		case PM_CULL_FACE:
			m_Config.Culling.bEnable = bEnable;
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
	void GfxEngine::SetViewport(Int32 nX, Int32 nY, Int32 nW, Int32 nH) {
		glViewport(nX, nY, nW, nH);
	}
	void GfxEngine::SetDrawMode(DrawModes dMode, FacePlanes facePlane) {
		m_Config.General.PolyMode.dMode = dMode;
		m_Config.General.PolyMode.facePlane = facePlane;
		glPolygonMode(facePlane, dMode);
	}
	void GfxEngine::SetLineWidth(Float32 nLineWidth) {
		m_Config.General.nLineWidth = nLineWidth;
		glLineWidth(nLineWidth);
	}
	void GfxEngine::SetPixelSize(Float32 nPxSize) {
		m_Config.General.nPixelSize = nPxSize;
		glPointSize(nPxSize);
	}
	void GfxEngine::SetBlendFunc(BlendConfigs bcSrcFactor, BlendConfigs bcDestFactor) {
		m_Config.Blending.FactorSrc = bcSrcFactor;
		m_Config.Blending.FactorDest = bcDestFactor;
		glBlendFunc(bcSrcFactor, bcDestFactor);
	}
	void GfxEngine::SetDepthFunc(DepthConfigs dcFunc) {
		m_Config.DepthTest.Func = dcFunc;
		glDepthFunc(dcFunc);
	}
	void GfxEngine::SetStencilFunc(StencilConfigs scFunc, UInt32 unRefValue, UInt8 unBitMask) {
		m_Config.StencilTest.Func = scFunc;
		m_Config.StencilTest.nBitMask = unBitMask;
		m_Config.StencilTest.nRefValue = unRefValue;
		glStencilFunc(scFunc, unRefValue, unBitMask);
	}
	void GfxEngine::SetFrameBuf(FrameBuf* pfmBuf) {
		m_pfmBuf = pfmBuf;
		NW_ASSERT(pfmBuf->GetAttachment(0) != nullptr, "There must be at least one attachment!");
		m_drbScreen.gMtl->SetTexture(m_pfmBuf->GetAttachment(0));
	}
	// --==<core_methods>==--
	void GfxEngine::Run()
	{
		Init();
		if (!m_bIsRunning) { return; }

		auto& fnUpdate = [this]()->void {
			while (m_bIsRunning) { Update(); }
			Quit();
		};
		m_thrRun = Thread(fnUpdate);
	}
	bool GfxEngine::Init()
	{
		if (m_bIsRunning) { return false; }
		GetMemory() = MemArena(new Byte[1 << 18], 1 << 18);

#if (defined GFX_PLATFORM_WINDOWS && false)
		m_pModule = LoadLibraryA("opengl32.dll");
		if (m_pModule == nullptr) {
			NW_ERR("Failed to load graphics library!");
			delete[] m_Memory.GetDataBeg();
			m_Memory = MemArena(nullptr, 0);
			return false;
		}
#else
		if (!gladLoadGL()) {
			NW_ERR("Failed to load graphics library!");
			delete[] m_Memory.GetDataBeg();
			m_Memory = MemArena(nullptr, 0);
			return false;
		}
#endif
		{
			String str = ((const char*)glGetString(GL_RENDERER));
			strcpy(&m_Info.strRenderer[0], &str[0]);
			str = ((const char*)glGetString(GL_VERSION));
			strcpy(&m_Info.strVersion[0], &str[0]);
			str = ((const char*)glGetString(GL_VENDOR));
			strcpy(&m_Info.strVendor[0], &str[0]);
			str = ((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
			strcpy(&m_Info.strShdLang[0], &str[0]);
		}
		std::cout << m_Info;
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
			pTex = Texture::Create("tex_white_solid", TXT_2D);
			pTex->SetInfo(imgInfo);
			pTex->SetInfo(texInfo);
			pTex->Remake();
		}
		{	// screen drawable info
			VertexArr::Create(m_drbScreen.vtxArr);
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
				Shader* pShader = Shader::Create("shd_screen_0");
				pShader->SetCode(&strScreenShaderCode[0]);
				if (!pShader->Compile()) { Quit(); return false; }

				m_drbScreen.gMtl = GMaterial::Create("gmt_screen_0");
				m_drbScreen.gMtl->SetShader(pShader);
				m_drbScreen.vtxArr->Remake(pShader->GetVtxLayout());
			}
		}
		{	// screen framebuffer
			FrameBufInfo fbInfo;
			fbInfo.rectViewport = { 0, 0, 1200, 800 };

			if (true) {
				FrameBuf* pfmBuf = FrameBuf::Create("fmb_draw_0", fbInfo);
				RefKeeper<Texture> rTex;
				TextureInfo texInfo;
				texInfo.bGenMipmap = true; texInfo.bGenSubImage = true;
				texInfo.unSamples = 1;

				Texture::Create("fba_draw_color_0", TXT_2D, rTex);
				texInfo.pxFormat = PXF_UINT8;
				texInfo.texFormat = TXF_RGB; texInfo.texInterFormat = TXFI_RGB;
				rTex->SetInfo(texInfo);
				pfmBuf->AttachTexture(rTex);

				Texture::Create("fba_draw_depth_stencil_0", TXT_2D, rTex);
				texInfo.pxFormat = PXF_UINT24_8;
				texInfo.texFormat = TXF_DEPTH_STENCIL; texInfo.texInterFormat = TXFI_DEPTH24_STENCIL8;
				rTex->SetInfo(texInfo);
				pfmBuf->AttachTexture(rTex);

				pfmBuf->Remake();
				GfxEngine::Get().SetFrameBuf(pfmBuf);
			}
		}

		for (auto& itState : m_States) { if (!itState->Init()) { return false; } }

		return (m_bIsRunning = true);
	}
	void GfxEngine::Quit()
	{
		if (!m_bIsRunning) { return; }
		m_bIsRunning = false;

		for (auto& itState : m_States) { itState->OnQuit(); }
		
		m_drbScreen = Drawable();

		auto& rFbs = TDataRes<FrameBuf>::GetStorage();
		for (auto& itFb : rFbs) { if (itFb != nullptr) itFb->~FrameBuf(); }
		auto& rTxs = TDataRes<Texture>::GetStorage();
		for (auto& itTex : rTxs) { if (itTex != nullptr) itTex->~Texture(); }
		auto& rShds = TDataRes<Shader>::GetStorage();
		for (auto& itShd : rShds){ if(itShd != nullptr) itShd->~Shader(); }
		auto& rgMtls = TDataRes<GMaterial>::GetStorage();
		for (auto& itgMtl : rgMtls) { if (itgMtl != nullptr) itgMtl->~GMaterial(); }
#if (defined GFX_PLATFORM_WINDOWS && false)
		FreeLibrary(m_pModule);
#endif

		delete[] GetMemory().GetDataBeg();
		GetMemory() = MemArena(nullptr, 0);
	}

	void GfxEngine::Update()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		OnDraw(m_drbScreen);
		for (auto& itState : m_States) { itState->Update(); }
	}

	void GfxEngine::OnEvent(AEvent& rEvt)
	{
		if (rEvt.IsInCategory(EC_MOUSE)) {
			MouseEvent* pmEvt = static_cast<MouseEvent*>(&rEvt);
			switch (pmEvt->evType) {
			case ET_MOUSE_MOVE:
				break;
			case ET_MOUSE_SCROLL:
				break;
			case ET_MOUSE_RELEASE:
				break;
			case ET_MOUSE_PRESS:
				break;
			}
			if (rEvt.bIsHandled) return;
			for (auto& itState : m_States) { itState->OnEvent(*pmEvt); }
		}
		else if (rEvt.IsInCategory(EC_KEYBOARD)) {
			KeyboardEvent* pkEvt = static_cast<KeyboardEvent*>(&rEvt);
			switch (pkEvt->evType) {
			case ET_KEY_RELEASE:
				switch (pkEvt->unKeyCode) {
				case KC_ENTER: break;
				default: break;
				}
				break;
			case ET_KEY_PRESS:
				switch (pkEvt->unKeyCode) {
				case KC_ENTER: break;
				default: break;
				}
				break;
			case ET_KEY_CHAR:
				break;
				if (rEvt.bIsHandled) { return; }
				for (auto& itState : m_States) { itState->OnEvent(*pkEvt); }
			}
		}
		else if (rEvt.IsInCategory(EC_WINDOW)) {
			WindowEvent* pwEvt = static_cast<WindowEvent*>(&rEvt);
			switch (pwEvt->evType) {
			case ET_WINDOW_RESIZE:
				m_pfmBuf->SetViewport({ 0, 0, pwEvt->nX, pwEvt->nY });
				m_pfmBuf->Remake();
				break;
			case ET_WINDOW_MOVE:
				break;
			case ET_WINDOW_FOCUS:
				break;
			case ET_WINDOW_CLOSE:
				StopRunning();
				rEvt.bIsHandled = true;
				break;
			}
			if (rEvt.bIsHandled) { return; }
			for (auto& itState : m_States) { itState->OnEvent(*pwEvt); }
		}
	}
	// --==</core_methods>==--

	// --==<drawing_methods>==--
	void GfxEngine::OnDraw(VertexArr& rVtxArr, GMaterial& rgMtl) {
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
	void GfxEngine::OnDraw(Drawable& rDrb) {
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
	// --==</drawing_methods>==--

	// --==<implementation_methods>==--
	GLibProc GfxEngine::LoadProc(const char* strName)
	{
		if (m_pModule != nullptr) { Quit(); return nullptr; }
		return GetProcAddress(m_pModule, strName);
	}
	// --==</implementation_methods>==--
}
#endif
#if (GFX_GAPI & GFX_GAPI_DX)
namespace NW
{
	GfxEngine::GfxEngine() :
		AEngine(),
		m_Info(GfxInfo()), m_Config(GfxConfig()),
		m_pfmBuf(nullptr),
		m_pContext(nullptr), m_pDevice(nullptr), m_pSwap(nullptr)
	{ }
	GfxEngine::~GfxEngine() { }

	// --setters
	void GfxEngine::SetModes(Bit bEnable, ProcessingModes pModes) {
		if (bEnable) {
		}
		else {
		}
	}
	void GfxEngine::SetViewport(Int32 nX, Int32 nY, Int32 nW, Int32 nH) {
	}
	void GfxEngine::SetDrawMode(DrawModes dMode, FacePlanes facePlane) {
		m_Config.General.PolyMode.dMode = dMode;
	}
	void GfxEngine::SetLineWidth(Float32 nLineWidth) {
	}
	void GfxEngine::SetPixelSize(Float32 nPxSize) {
	}
	void GfxEngine::SetBlendFunc(BlendConfigs unSrcFactorId, BlendConfigs unDestFactorId) {
	}
	void GfxEngine::SetDepthFunc(DepthConfigs unDepthFuncId) {
	}
	void GfxEngine::SetStencilFunc(StencilConfigs unFuncId, UInt32 unRefValue, UInt8 unBitMask) {
	}
	void GfxEngine::SetFrameBuf(FrameBuf* pfmBuf) {
		m_pfmBuf = pfmBuf;
		NW_ASSERT(pfmBuf->GetAttachment(0) != nullptr, "There must be at least one attachment!");
		m_drbScreen.gMtl->SetTexture(m_pfmBuf->GetAttachment(0));
	}
	void GfxEngine::SetWindow(HWND pWindow) {
		m_pWindow = pWindow;
	}

	// --==<core_methods>==--
	void GfxEngine::Run()
	{
		Init();
		if (!m_bIsRunning) { return; }

		auto& fnUpdate = [this]()->void {
			while (m_bIsRunning) { Update(); }
			Quit();
		};
		m_thrRun = Thread(fnUpdate);
	}
	bool GfxEngine::Init()
	{
		if (m_bIsRunning) { return false; }
		GetMemory() = MemArena(new Byte[1 << 18], 1 << 18);

		{
			DXGI_SWAP_CHAIN_DESC descSwap = { };
			descSwap.BufferDesc.Width = 0;
			descSwap.BufferDesc.Height = 0;
			descSwap.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			descSwap.BufferDesc.RefreshRate.Numerator = 0;
			descSwap.BufferDesc.RefreshRate.Denominator = 0;
			descSwap.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			descSwap.SampleDesc.Count = 1;
			descSwap.SampleDesc.Quality = 0;
			descSwap.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			descSwap.BufferCount = 1;
			descSwap.OutputWindow = m_pWindow;
			descSwap.Windowed = TRUE;
			descSwap.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			descSwap.Flags = 0;

			D3D11CreateDeviceAndSwapChain(
				nullptr,	// graphical adapter
				D3D_DRIVER_TYPE_HARDWARE,	// driver type
				nullptr,	// software module
				0,	// flags
				nullptr,	// feature levels
				0,	// feature levels count
				D3D11_SDK_VERSION,	// sdk version
				&descSwap,
				&m_pSwap,	// swapchain
				&m_pDevice,	// device
				nullptr,	// feature level
				&m_pContext	// device context
			);
		}

		for (auto& itState : m_States) { if (!itState->Init()) { return false; } }

		return (m_bIsRunning = true);
	}
	void GfxEngine::Quit()
	{
		if (!m_bIsRunning) { return; }
		m_bIsRunning = false;

		for (auto& itState : m_States) { itState->OnQuit(); }

		m_drbScreen = Drawable();

		auto& rFbs = TDataRes<FrameBuf>::GetStorage();
		for (auto& itFb : rFbs) { if (itFb != nullptr) itFb->~FrameBuf(); }
		auto& rTxs = TDataRes<Texture>::GetStorage();
		for (auto& itTex : rTxs) { if (itTex != nullptr) itTex->~Texture(); }
		auto& rShds = TDataRes<Shader>::GetStorage();
		for (auto& itShd : rShds) { if (itShd != nullptr) itShd->~Shader(); }
		auto& rgMtls = TDataRes<GMaterial>::GetStorage();
		for (auto& itgMtl : rgMtls) { if (itgMtl != nullptr) itgMtl->~GMaterial(); }

		if (m_pContext != nullptr) { m_pContext->Release(); }
		if (m_pSwap != nullptr) { m_pSwap->Release(); }
		if (m_pDevice != nullptr) { m_pDevice->Release(); }

		delete[] GetMemory().GetDataBeg();
		GetMemory() = MemArena(nullptr, 0);
	}

	void GfxEngine::Update()
	{
		OnDraw(m_drbScreen);
		for (auto& itState : m_States) { itState->Update(); }
		//m_pSwap->Present(1u, 0u);
	}

	void GfxEngine::OnEvent(AEvent& rEvt)
	{
		if (rEvt.IsInCategory(EC_MOUSE)) {
			MouseEvent* pmEvt = static_cast<MouseEvent*>(&rEvt);
			switch (pmEvt->evType) {
			case ET_MOUSE_MOVE:
				break;
			case ET_MOUSE_SCROLL:
				break;
			case ET_MOUSE_RELEASE:
				break;
			case ET_MOUSE_PRESS:
				break;
			}
			if (rEvt.bIsHandled) return;
			for (auto& itState : m_States) { itState->OnEvent(*pmEvt); }
		}
		else if (rEvt.IsInCategory(EC_KEYBOARD)) {
			KeyboardEvent* pkEvt = static_cast<KeyboardEvent*>(&rEvt);
			switch (pkEvt->evType) {
			case ET_KEY_RELEASE:
				switch (pkEvt->unKeyCode) {
				case KC_ENTER: break;
				default: break;
				}
				break;
			case ET_KEY_PRESS:
				switch (pkEvt->unKeyCode) {
				case KC_ENTER: break;
				default: break;
				}
				break;
			case ET_KEY_CHAR:
				break;
				if (rEvt.bIsHandled) { return; }
				for (auto& itState : m_States) { itState->OnEvent(*pkEvt); }
			}
		}
		else if (rEvt.IsInCategory(EC_WINDOW)) {
			WindowEvent* pwEvt = static_cast<WindowEvent*>(&rEvt);
			switch (pwEvt->evType) {
			case ET_WINDOW_RESIZE:
				m_pfmBuf->SetViewport({ 0, 0, pwEvt->nX, pwEvt->nY });
				m_pfmBuf->Remake();
				break;
			case ET_WINDOW_MOVE:
				break;
			case ET_WINDOW_FOCUS:
				break;
			case ET_WINDOW_CLOSE:
				StopRunning();
				rEvt.bIsHandled = true;
				break;
			}
			if (rEvt.bIsHandled) { return; }
			for (auto& itState : m_States) { itState->OnEvent(*pwEvt); }
		}
	}
	// --==</core_methods>==--

	// --==<drawing_methods>==--
	void GfxEngine::OnDraw(VertexArr& rVtxArr, GMaterial& rgMtl) {
	}
	void GfxEngine::OnDraw(Drawable& rDrb) {
	}
	// --==</drawing_methods>==--

	// --==<implementation_methods>==--
	// --==</implementation_methods>==--
}
#endif
#endif	// GFX_GAPI

/// OnDraw (AShape)
	/// Description:
	/// -- Shapes have a vertex data, indices and bound AGMaterial
	/// -- GfxEngine collects all the Vertex and Index data into some arrays from given object
	/// -- AGMaterial of the object provides GfxEngine's AGMaterial with textures
	/// -- Every new texture from new AGMaterial will be bound to the current gMtl
	/// -- In the EndDraw it will bind all owned textures and will set their samplerIDs to the own shader
	/// -- There is also default white texture which will be used if we don not have other textures