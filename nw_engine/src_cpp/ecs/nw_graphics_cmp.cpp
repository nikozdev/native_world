#include <nw_pch.hpp>
#include <ecs/nw_scene.h>

#include <gl/control/nw_draw_engine.h>
#include <gl/vision/nw_gcamera.h>

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
	}
	Graphics2dCmp::~Graphics2dCmp(){}

	// --core_methods
	void Graphics2dCmp::OnUpdate()
	{
		DrawEngine::OnDraw(&m_Sprite, "des_scene");
	}
	// --==</Graphics2dCmp>==--
}