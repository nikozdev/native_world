#include <nw_pch.hpp>
#include <ecs/nw_scene.h>

#include <glib/control/nw_graph_engine.h>
#include <glib/vision/nw_gcamera.h>

#include <sys/nw_data_sys.h>

namespace NW
{
	// --==<DrawPolyLineCmp>==--
	DrawPolyLineCmp::DrawPolyLineCmp(AEntity& rEntity) :
		AGraphicsCmp(rEntity) { }
	
	// --core_methods
	void DrawPolyLineCmp::OnUpdate() { }
	// --==</DrawPolyLineCmp>==--

	// --==<DrawPolygonsCmp>==--
	DrawPolygonsCmp::DrawPolygonsCmp(AEntity& rEntity) :
		AGraphicsCmp(rEntity) { }

	// --core_methods
	void DrawPolygonsCmp::OnUpdate() { }
	// --==</DrawPolygonsCmp>==--

	// --==<Graphics2dCmp>==--
	Graphics2dCmp::Graphics2dCmp(AEntity& rEntity) :
		AGraphicsCmp(rEntity)
	{
		pGState = GraphEngine::Get().GetState(Scene::Get().GetGraphState()->GetName());
	}
	Graphics2dCmp::~Graphics2dCmp(){}

	// --core_methods
	void Graphics2dCmp::OnUpdate()
	{
		pGState->Config.DPrimitive = PT_TRIANGLES;
		pGState->Config.DMode = DM_FILL;
		pGState->Blending.bEnable = true;
		pGState->Blending.FactorDest = BC_SRC_ALPHA;
		pGState->Blending.FactorDest = BC_ONE_MINUS_SRC_ALPHA;

		pGState->AddDrawable(&m_Sprite);
	}
	// --==</Graphics2dCmp>==--

		// --==<TileMapCmp>==--
	TileMapCmp::TileMapCmp(AEntity& rEntity) :
		AGraphicsCmp(rEntity)
	{
		pGState = GraphEngine::Get().GetState(Scene::Get().GetGraphState()->GetName());

		m_TileMap.pGMtl = DataSys::GetDataRes<GMaterial>("gmt_default_tile");
		m_TileMap.SetTilemap(DataSys::GetDataRes<ATexture2d>("spt_nw_hero"));
	}
	TileMapCmp::~TileMapCmp() {}

	// --core_methods
	void TileMapCmp::OnUpdate()
	{
		pGState->Config.DPrimitive = PT_TRIANGLES;
		pGState->Config.DMode = DM_FILL;
		pGState->Blending.bEnable = true;
		pGState->Blending.FactorDest = BC_SRC_ALPHA;
		pGState->Blending.FactorDest = BC_ONE_MINUS_SRC_ALPHA;

		pGState->AddDrawable(&m_TileMap);
	}
	// --==</TileMapCmp>==--
}