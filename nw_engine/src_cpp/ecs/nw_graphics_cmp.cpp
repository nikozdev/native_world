#include <nw_pch.hpp>
#include <ecs/nw_scene.h>

#include <gl/control/nw_drawer.h>

#include <sys/nw_data_sys.h>

namespace NW
{
	// ========<DrawPolyLineCmp>========
	DrawPolyLineCmp::DrawPolyLineCmp(AEntity& rEntity) :
		AGraphicsCmp(rEntity)
	{
		m_PolyLine.vtxCrds.push_back(V3f{ -1.0f, 0.0f, 0.0f });
		m_PolyLine.vtxCrds.push_back(V3f{ 0.0f, 1.0f, 0.0f });
		m_PolyLine.vtxCrds.push_back(V3f{ 1.0f, 0.0f, 0.0f });
		m_PolyLine.GetGMaterial()->SetTexture(DataSys::GetDataRes<ATexture1d>("tex_white_solid"));
		m_PolyLine.GetGMaterial()->SetShader(DataSys::GetDataRes<AShader>("shd_batch_lines"));
	}
	
	// -- Interface Methods
	void DrawPolyLineCmp::OnUpdate()
	{
	}
	// ========</DrawPolyLineCmp>========

	// ========<DrawPolygonsCmp>========
	DrawPolygonsCmp::DrawPolygonsCmp(AEntity& rEntity) :
		AGraphicsCmp(rEntity)
	{
		m_Polygons.vtxCrds.push_back(V3f{ -0.5f, -0.5f, 0.0f });
		m_Polygons.vtxCrds.push_back(V3f{ 0.0f, 0.5f, 0.0f });
		m_Polygons.vtxCrds.push_back(V3f{ 0.5f, 0.5f, 0.0f });
		m_Polygons.GetGMaterial()->SetTexture(DataSys::GetDataRes<ATexture2d>("tex_white_solid"));
		m_Polygons.GetGMaterial()->SetShader(DataSys::GetDataRes<AShader>("shd_batch_lines"));
	}

	// -- Interface Methods
	void DrawPolygonsCmp::OnUpdate()
	{
	}
	// ========</DrawPolygonsCmp>========

	// ========<Graphics2dCmp>========
	Graphics2dCmp::Graphics2dCmp(AEntity& rEntity) :
		AGraphicsCmp(rEntity)
	{
		m_Sprite.GetGMaterial()->SetTexture(DataSys::GetDataRes<ATexture2d>("tex_white_solid"));
		m_Sprite.GetGMaterial()->SetShader(DataSys::GetDataRes<AShader>("shd_batch_3d"));
		DOData.pDrawable = &m_Sprite;
		DOData.DrawPrimitive = PT_TRIANGLES;
	}

	// -- Interface Methods
	void Graphics2dCmp::OnUpdate()
	{
		Drawer::BeginDraw();
		DOData.pShader = m_Sprite.GetGMaterial()->GetShader();
		DOData.unDrawOrder = unDrawOrder;
		Drawer::GetGApi()->SetModes(true, PM_BLEND);
		Drawer::GetGApi()->SetBlendFunc(BC_SRC_ALPHA, BC_ONE_MINUS_SRC_ALPHA);
		Drawer::OnDraw(DOData);
		Drawer::EndDraw();
	}
	// ========</Graphics2dCmp>========
}