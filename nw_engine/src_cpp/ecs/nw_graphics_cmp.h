#ifndef ECS_GRAPHICS_COMPONENT_H
#define ECS_GRAPHICS_COMPONENT_H

#include <ecs/nw_entity_cmp.h>

#include <gl/render/nw_drawable.h>
#include <gl/vision/nw_gmaterial.h>

namespace NW
{
	/// Abstract GraphicsComponent class
	class NW_API AGraphicsCmp : public AEntityCmp
	{
	public:
		UInt8 unDrawOrder = 0;
		DrawObjectData DOData;
	public:
		AGraphicsCmp(AEntity& rEntity) :
			AEntityCmp(rEntity, std::type_index(typeid(AGraphicsCmp))) { }
		virtual ~AGraphicsCmp() = default;

		// --getters
		virtual inline ADrawable* GetDrawable() = 0;
		// --setters
		// --core_methods
		virtual void OnUpdate() = 0;
	protected:
	};
	/// DrawPolyLineComponent class
	class NW_API DrawPolyLineCmp : public AGraphicsCmp
	{
	public:
		DrawPolyLineCmp(AEntity& rEntity);
		~DrawPolyLineCmp() = default;

		// --getters
		virtual inline ADrawable* GetDrawable() override { return &m_PolyLine; }
		// --setters

		// --core_methods
		virtual void OnUpdate() override;
	private:
		PolyLine m_PolyLine;
	};
	/// Draw2dPolygonComponent class
	class NW_API DrawPolygonsCmp : public AGraphicsCmp
	{
	public:
		DrawPolygonsCmp(AEntity& rEntity);
		~DrawPolygonsCmp() = default;

		// --getters
		virtual inline ADrawable* GetDrawable() override { return &m_Polygons; }
		// --setters

		// --core_methods
		virtual void OnUpdate() override;
	private:
		Polygons m_Polygons;
	};
	/// Graphics2dComponent class
	class NW_API Graphics2dCmp : public AGraphicsCmp
	{
	public:
		Graphics2dCmp(AEntity& rEntity);
		~Graphics2dCmp() = default;

		// --getters
		virtual inline ADrawable* GetDrawable() override { return &m_Sprite; }
		// --setters

		// --core_methods
		virtual void OnUpdate() override;
	private:
		Sprite m_Sprite;
	};
}

#endif	// ECS_GRAPHICS_COMPONENT_H