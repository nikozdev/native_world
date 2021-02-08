#include <gfx_pch.hpp>
#include "gfx_engine_states.h"

#include <gfx_engine.h>
#include <gfx_buffer.h>
#include <gfx_framebuf.h>
#include <gfx_shader.h>
#include <gfx_material.h>
#include <gfx_texture.h>
#include <gfx_camera_lad.h>
#include <gfx_component.h>

namespace NW
{
	GfxState::GfxState() :
		AEngineState("graphics_state"),
		m_rGraph(GfxEngine::Get()),
		m_pShdBuf() {}
	GfxState::~GfxState() { }

	// --==<core_methods>==--
	bool GfxState::Init() {
		ShaderBuf::Create(m_pShdBuf);
		m_pShdBuf->SetData(1 << 12, nullptr);

		{	// shaders
			Shader* pShader = nullptr;

			pShader = Shader::Create("shd_3d_default");
			if (!pShader->LoadF("D:/dev/native_world/nw_glib/src_glsl/shd_3d_default.glsl")) { return false; }

			pShader = Shader::Create("shd_3d_batch");
			if (!pShader->LoadF("D:/dev/native_world/nw_glib/src_glsl/shd_3d_batch.glsl")) { return false; }
			m_pShdBuf->Remake(pShader->GetShdLayout());
		}
		{	// textures
			Texture* pTex = nullptr;
			pTex = Texture::Create("tex_nw_bat", TXT_2D);
			pTex->LoadF(R"(D:\dev\native_world\nw_glib\data\ico\nw_bat.png)");
		}
		{	// materials
			GMaterial* pgMtl = GMaterial::Create("gmt_3d_default");
			pgMtl->SetShader(TDataRes<Shader>::GetDataRes("shd_3d_default"));
			pgMtl = GMaterial::Create("gmt_3d_batch");
			pgMtl->SetShader(TDataRes<Shader>::GetDataRes("shd_3d_batch"));
			pgMtl->SetTexture(TDataRes<Texture>::GetDataRes("tex_nw_bat"), "unf_tex[0]");
		}
		{	// framebuffers
		}

		return true;
	}
	void GfxState::OnQuit() {
		m_pShdBuf.Reset();
	}
	void GfxState::Update()
	{
		GCameraLad::Get().whBounds.x = m_rGraph.GetFrameBuf()->GetWidth();
		GCameraLad::Get().whBounds.y = m_rGraph.GetFrameBuf()->GetHeight();
		GCameraLad::Get().UpdateCamera();

		DrawScene();
	}

	void GfxState::OnEvent(MouseEvent& rmEvt)
	{
		GCameraLad::Get().OnEvent(rmEvt);
	}
	void GfxState::OnEvent(KeyboardEvent& rkEvt)
	{
		GCameraLad::Get().OnEvent(rkEvt);
	}
	void GfxState::OnEvent(WindowEvent& rwEvt)
	{
		GCameraLad::Get().OnEvent(rwEvt);
	}
	// --==</core_methods>==--

	// --==<implementation_methods>==--
	inline void GfxState::DrawScene() {
		FrameBuf* pfmBuf = m_rGraph.GetFrameBuf();
		pfmBuf->Bind();
		pfmBuf->Clear();
		m_pShdBuf->Bind();

		m_rGraph.SetModes(true, PM_BLEND);
		//m_rGraph.SetModes(true, PM_DEPTH_TEST);
		m_rGraph.SetBlendFunc(BC_SRC_ALPHA, BC_ONE_MINUS_SRC_ALPHA);

		Mat4f mat4Proj = GCameraLad::Get().GetGCamera()->GetProjMatrix();
		Mat4f mat4View = GCameraLad::Get().GetGCamera()->GetViewMatrix();
		m_pShdBuf->SetSubData(sizeof(Mat4f), &(mat4Proj[0][0]), 0 * sizeof(Mat4f));
		m_pShdBuf->SetSubData(sizeof(Mat4f), &(mat4View[0][0]), 1 * sizeof(Mat4f));

		auto& rg2dCmps = CmpSys::GetCmps<Graphics2dCmp>();
		for (auto& itCmp : rg2dCmps) {
			if (TransformCmp* tfCmp = CmpSys::GetCmp<TransformCmp>(itCmp->GetEntId())) {
				auto vtxData = MakeVtxRect(tfCmp->GetMatrix());
				BufferData vbData{ &vtxData[0], vtxData.size() * sizeof(Float32), 0 };
				itCmp->m_drw.UploadVtxData(&vbData);
			}
			m_rGraph.OnDraw(itCmp->m_drw);
		}
		m_pShdBuf->Unbind();
		pfmBuf->Unbind();
	}
	// --==</implementation_methods>==--
}

namespace NW
{
	GfxTestState::GfxTestState() :
		AEngineState("graphics_test_state"),
		m_rGraph(GfxEngine::Get()) {}
	GfxTestState::~GfxTestState() { }

	// --==<core_methods>==--
	bool GfxTestState::Init() {
		return true;
	}
	void GfxTestState::OnQuit() {
	}
	void GfxTestState::Update()
	{
		DrawScene();
	}

	void GfxTestState::OnEvent(MouseEvent& rmEvt)
	{
	}
	void GfxTestState::OnEvent(KeyboardEvent& rkEvt)
	{
	}
	void GfxTestState::OnEvent(WindowEvent& rwEvt)
	{
	}
	// --==</core_methods>==--

	// --==<implementation_methods>==--
	inline void GfxTestState::DrawScene() {
	}
	// --==</implementation_methods>==--
}