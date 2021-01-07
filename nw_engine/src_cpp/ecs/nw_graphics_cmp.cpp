#include <nw_pch.hpp>
#include <ecs/nw_scene.h>

#include <core/nw_graph_engine.h>

#include <sys/nw_data_sys.h>

namespace NW
{
	// --==<DrawPolyLineCmp>==--
	DrawPolyLineCmp::DrawPolyLineCmp(AEntity& rEntity) :
		AGraphicsCmp(rEntity)
	{
		m_PolyLine.vtxCrds.push_back(V3f{ -1.0f, 0.0f, 0.0f });
		m_PolyLine.vtxCrds.push_back(V3f{ 0.0f, 1.0f, 0.0f });
		m_PolyLine.vtxCrds.push_back(V3f{ 1.0f, 0.0f, 0.0f });
		m_PolyLine.pGMtl->SetTexture(DataSys::GetDataRes<ATexture1d>("tex_white_solid"));
		m_PolyLine.pGMtl->SetShader(DataSys::GetDataRes<AShader>("shd_batch_lines"));
	}
	
	// --core_methods
	void DrawPolyLineCmp::OnUpdate()
	{
	}
	// --==</DrawPolyLineCmp>==--

	// --==<DrawPolygonsCmp>==--
	DrawPolygonsCmp::DrawPolygonsCmp(AEntity& rEntity) :
		AGraphicsCmp(rEntity)
	{
		m_Polygons.vtxCrds.push_back(V3f{ -0.5f, -0.5f, 0.0f });
		m_Polygons.vtxCrds.push_back(V3f{ 0.0f, 0.5f, 0.0f });
		m_Polygons.vtxCrds.push_back(V3f{ 0.5f, 0.5f, 0.0f });
		m_Polygons.pGMtl->SetTexture(DataSys::GetDataRes<ATexture2d>("tex_white_solid"));
		m_Polygons.pGMtl->SetShader(DataSys::GetDataRes<AShader>("shd_batch_lines"));
	}

	// --core_methods
	void DrawPolygonsCmp::OnUpdate()
	{
	}
	// --==</DrawPolygonsCmp>==--

	// --==<Graphics2dCmp>==--
	Graphics2dCmp::Graphics2dCmp(AEntity& rEntity) :
		AGraphicsCmp(rEntity)
	{
		m_Sprite.pGMtl->SetTexture(DataSys::GetDataRes<ATexture2d>("tex_white_solid"));
		m_Sprite.pGMtl->SetShader(DataSys::GetDataRes<AShader>("shd_batch_3d"));
		DOData.pDrawable = &m_Sprite;
		DOData.DrawPrimitive = PT_TRIANGLES;
	}

	// --core_methods
	void Graphics2dCmp::OnUpdate()
	{
		GraphEngine::BeginDraw({ Scene::Get().GetGCamera() });
		DOData.unDrawOrder = unDrawOrder;
		GraphEngine::GetGApi()->SetModes(true, PM_BLEND);
		GraphEngine::GetGApi()->SetBlendFunc(BC_SRC_ALPHA, BC_ONE_MINUS_SRC_ALPHA);
		GraphEngine::OnDraw(DOData);
		GraphEngine::EndDraw();
	}
	// --==</Graphics2dCmp>==--
}