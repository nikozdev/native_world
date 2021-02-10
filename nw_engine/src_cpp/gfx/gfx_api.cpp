#include <nw_pch.hpp>
#include "gfx/gfx_api.h"

#include <gfx/gfx_tools.h>
#include <gfx/gfx_framebuf.h>
#include <gfx/gfx_drawable.h>

#include <gfx/gfx_camera.h>
#include <gfx/gfx_shader.h>
#include <gfx/gfx_material.h>
#include <gfx/gfx_texture.h>

#include <../src_glsl/shd_screen.hpp>

#if (defined NW_GAPI)
#include <gfx/gfx_loader.h>
#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	GfxApi::GfxApi() :
		m_Info(GfxInfo()), m_Config(GfxConfig()),
		m_pfmBuf(nullptr) { }
	GfxApi::~GfxApi() { }

	// --setters
	void GfxApi::SetModes(Bit bEnable, ProcessingModes pModes) {
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
	void GfxApi::SetViewport(Int32 nX, Int32 nY, Int32 nW, Int32 nH) {
		glViewport(nX, nY, nW, nH);
	}
	void GfxApi::SetDrawMode(DrawModes dMode, FacePlanes facePlane) {
		m_Config.General.PolyMode.dMode = dMode;
		m_Config.General.PolyMode.facePlane = facePlane;
		glPolygonMode(facePlane, dMode);
	}
	void GfxApi::SetLineWidth(Float32 nLineWidth) {
		m_Config.General.nLineWidth = nLineWidth;
		glLineWidth(nLineWidth);
	}
	void GfxApi::SetPixelSize(Float32 nPxSize) {
		m_Config.General.nPixelSize = nPxSize;
		glPointSize(nPxSize);
	}
	void GfxApi::SetBlendFunc(BlendConfigs bcSrcFactor, BlendConfigs bcDestFactor) {
		m_Config.Blending.FactorSrc = bcSrcFactor;
		m_Config.Blending.FactorDest = bcDestFactor;
		glBlendFunc(bcSrcFactor, bcDestFactor);
	}
	void GfxApi::SetDepthFunc(DepthConfigs dcFunc) {
		m_Config.DepthTest.Func = dcFunc;
		glDepthFunc(dcFunc);
	}
	void GfxApi::SetStencilFunc(StencilConfigs scFunc, UInt32 unRefValue, UInt8 unBitMask) {
		m_Config.StencilTest.Func = scFunc;
		m_Config.StencilTest.nBitMask = unBitMask;
		m_Config.StencilTest.nRefValue = unRefValue;
		glStencilFunc(scFunc, unRefValue, unBitMask);
	}
	void GfxApi::SetFrameBuf(FrameBuf* pfmBuf) {
		m_pfmBuf = pfmBuf;
		NWL_ASSERT(pfmBuf->GetAttachment(0) != nullptr, "There must be at least one attachment!");
		m_drbScreen.gMtl->SetTexture(m_pfmBuf->GetAttachment(0));
	}
	// --==<core_methods>==--
	bool GfxApi::Init()
	{
		
#if (defined NWL_PLATFORM_WINDOWS && false)
		m_pModule = LoadLibraryA("opengl32.dll");
		if (m_pModule == nullptr) {
			NWL_ERR("Failed to load graphics library!");
			delete[] m_Memory.GetDataBeg();
			m_Memory = MemArena(nullptr, 0);
			return false;
		}
#else
		if (!gladLoadGL()) { NWL_ERR("Failed to load graphics library!"); return false; }
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
		return true;
	}
	void GfxApi::OnQuit()
	{
		m_drbScreen = Drawable();

		DataSys::GetStorage<Shader>().clear();
		DataSys::GetStorage<GfxMaterial>().clear();
		DataSys::GetStorage<FrameBuf>().clear();
		DataSys::GetStorage<Texture>().clear();
#if (defined NW_PLATFORM_WINDOWS && false)
		FreeLibrary(m_pModule);
#endif
	}

	void GfxApi::Update()
	{
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