#include <nw_pch.hpp>
#include <core/nw_engine_states.h>

#include <core/nw_engine.h>

#include <gfx/gfx_api.h>
#include <gfx/gfx_buffer.h>
#include <gfx/gfx_framebuf.h>
#include <gfx/gfx_shader.h>
#include <gfx/gfx_material.h>
#include <gfx/gfx_texture.h>
#include <gfx/gfx_camera_lad.h>
#include <gfx/gfx_component.h>

namespace NW
{
	GfxState::GfxState() :
		AEngineState("graphics_state"),
		m_rEngine(CoreEngine::Get()),
		m_pShdBuf() {}
	GfxState::~GfxState() { }

	// --==<core_methods>==--
	bool GfxState::Init() {
		m_pShdBuf.MakeRef<ShaderBuf>();
		m_pShdBuf->SetData(1 << 12, nullptr);

		{	// shaders
			Shader* pShader = nullptr;
			pShader = DataSys::GetDR<Shader>(DataSys::NewDR<Shader>("shd_3d_default"));
			if (!pShader->LoadF("D:/dev/native_world/nw_engine/src_glsl/shd_3d_default.glsl")) { return false; }

			pShader = DataSys::GetDR<Shader>(DataSys::NewDR<Shader>("shd_3d_batch"));
			if (!pShader->LoadF("D:/dev/native_world/nw_engine/src_glsl/shd_3d_batch.glsl")) { return false; }
			m_pShdBuf->Remake(pShader->GetShdLayout());
		}
		{	// textures
			Texture* pTex = nullptr;
			pTex = DataSys::GetDR<Texture>(DataSys::NewDR<Texture>("tex_nw_bat", TXT_2D));
			pTex->LoadF(R"(D:\dev\native_world\nw_engine\data\image\nw_bat.png)");
		}
		{	// materials
			GfxMaterial* pgMtl = nullptr;
			pgMtl = DataSys::GetDR<GfxMaterial>(DataSys::NewDR<GfxMaterial>("gmt_3d_default"));
			pgMtl->SetShader(DataSys::GetDR<Shader>("shd_3d_default"));
			pgMtl = DataSys::GetDR<GfxMaterial>(DataSys::NewDR<GfxMaterial>("gmt_3d_batch"));
			pgMtl->SetShader(DataSys::GetDR<Shader>("shd_3d_batch"));
			pgMtl->SetTexture(DataSys::GetDR<Texture>("tex_nw_bat"), "unf_tex[0]");
		}
		{	// framebuffers
		}

		for (UInt32 unId = 0; unId < 100; unId++) {
			UInt32 eId = EntSys::AddEnt();
			CmpSys::AddCmp<Gfx2dCmp>(eId);
			CmpSys::AddCmp<TFormCmp>(eId);
		}

		return true;
	}
	void GfxState::OnQuit() {
		m_pShdBuf.Reset();
	}
	void GfxState::Update()
	{
		GfxCameraLad::Get().whBounds.x = m_rEngine.GetGfx()->GetFrameBuf()->GetWidth();
		GfxCameraLad::Get().whBounds.y = m_rEngine.GetGfx()->GetFrameBuf()->GetHeight();
		GfxCameraLad::Get().UpdateCamera();
		
		m_rEngine.GetGfx()->GetFrameBuf()->SetClearColor({ 0.2f, std::sinf(TimeSys::GetCurrS()), std::cosf(TimeSys::GetCurrS()), 1.0f });

		DrawScene();
	}

	void GfxState::OnEvent(MouseEvent& rmEvt)
	{
		GfxCameraLad::Get().OnEvent(rmEvt);
	}
	void GfxState::OnEvent(KeyboardEvent& rkEvt)
	{
		GfxCameraLad::Get().OnEvent(rkEvt);
	}
	void GfxState::OnEvent(WindowEvent& rwEvt)
	{
		GfxCameraLad::Get().OnEvent(rwEvt);
	}
	// --==</core_methods>==--

	// --==<implementation_methods>==--
	inline void GfxState::DrawScene() {
		FrameBuf* pfmBuf = m_rEngine.GetGfx()->GetFrameBuf();
		pfmBuf->Bind();
		pfmBuf->Clear();
		m_pShdBuf->Bind();

		m_rEngine.GetGfx()->SetModes(true, PM_BLEND);
		//m_rEngine.GetGfx()->SetModes(true, PM_DEPTH_TEST);
		m_rEngine.GetGfx()->SetBlendFunc(BC_SRC_ALPHA, BC_ONE_MINUS_SRC_ALPHA);

		Mat4f mat4Proj = GfxCameraLad::Get().GetGfxCamera()->GetProjMatrix();
		Mat4f mat4View = GfxCameraLad::Get().GetGfxCamera()->GetViewMatrix();
		m_pShdBuf->SetSubData(sizeof(Mat4f), &(mat4Proj[0][0]), 0 * sizeof(Mat4f));
		m_pShdBuf->SetSubData(sizeof(Mat4f), &(mat4View[0][0]), 1 * sizeof(Mat4f));

		auto& rg2dCmps = CmpSys::GetCmps<Gfx2dCmp>();
		for (auto& itCmp : rg2dCmps) {
			if (TFormCmp* tfCmp = CmpSys::GetCmp<TFormCmp>(itCmp->GetEntId())) {
				tfCmp->xyzCrd.x = std::sin(TimeSys::GetCurrS() + tfCmp->GetEntId());
				tfCmp->xyzCrd.y = std::cos(TimeSys::GetCurrS() + tfCmp->GetEntId());
				auto vtxData = MakeVtxRect(tfCmp->GetMatrix());
				BufferData vbData{ &vtxData[0], vtxData.size() * sizeof(Float32), 0 };
				itCmp->m_drw.UploadVtxData(&vbData);
			}
			m_rEngine.GetGfx()->OnDraw(itCmp->m_drw);
		}
		m_pShdBuf->Unbind();
		pfmBuf->Unbind();
	}
	// --==</implementation_methods>==--
}