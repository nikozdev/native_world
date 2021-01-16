#include <nw_pch.hpp>
#include <ecs/nw_graphics_cmp.h>
#include <ecs/nw_scene.h>

#include <glib/core/nw_gengine.h>
#include <glib/vision/nw_gcamera.h>

#include <sys/nw_data_sys.h>

namespace NW
{
	// --==<AGraphicsCmp>==--
	AGraphicsCmp::AGraphicsCmp(AEntity& rEntity) :
		AEntityCmp(rEntity, std::type_index(typeid(AGraphicsCmp))),
		pGLayer(nullptr), DOData(DrawObjectData())
	{
		DataSys::AddDataRes<AGraphicsCmp>(this);
		pGLayer = GEngine::Get().GetLayer();
	}
	AGraphicsCmp::AGraphicsCmp(AGraphicsCmp& rCpy) :
		AEntityCmp(rCpy),
		pGLayer(rCpy.pGLayer), DOData(rCpy.DOData)
	{
		DataSys::AddDataRes<AGraphicsCmp>(this);
	}
	AGraphicsCmp::~AGraphicsCmp() { DataSys::RmvDataRes<AGraphicsCmp>(GetId()); }
	// --==<AGraphicsCmp>==--
	
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
		DOData.pDrawable = &m_Sprite;
		DOData.unId = m_unId;
		pGLayer->AddDrawData(DOData);
	}
	Graphics2dCmp::~Graphics2dCmp()
	{
		pGLayer->RmvDrawData(DOData.unId);
	}

	// --core_methods
	void Graphics2dCmp::OnUpdate()
	{
		pGLayer->DConfig.General.GPrimitive = PT_TRIANGLES;
		pGLayer->DConfig.General.DMode = DM_FILL;
		pGLayer->DConfig.Blending.bEnable = true;
		pGLayer->DConfig.Blending.FactorDest = BC_SRC_ALPHA;
		pGLayer->DConfig.Blending.FactorDest = BC_ONE_MINUS_SRC_ALPHA;
	}
	// --==</Graphics2dCmp>==--

		// --==<TileMapCmp>==--
	TileMapCmp::TileMapCmp(AEntity& rEntity) :
		AGraphicsCmp(rEntity)
	{
		m_TileMap.pGMtl = DataSys::GetDataRes<GMaterial>("gmt_default_tile");
		m_TileMap.pTileMap = DataSys::GetDataRes<ATexture2d>("spt_nw_hero");
	}
	TileMapCmp::~TileMapCmp() {}

	// --core_methods
	void TileMapCmp::OnUpdate()
	{
	}
	// --==</TileMapCmp>==--
}