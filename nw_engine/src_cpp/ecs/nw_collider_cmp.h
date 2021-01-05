#ifndef ECS_COLLIDER_COMPONENT_H
#define ECS_COLLIDER_COMPONENT_H
#include <ecs/nw_entity_cmp.h>

#include <gl/render/nw_drawable.h>

namespace NW
{
	/// Abstract Collider2dComponent class
	class NW_API ACollider2dCmp : public AEntityCmp
	{
	public:
		ACollider2dCmp(AEntity& rEntity) : AEntityCmp(rEntity, std::type_index(typeid(ACollider2dCmp))) { }
		virtual ~ACollider2dCmp() = default;

		// -- Getters
		virtual inline ADrawable* GetShape() = 0;
		virtual inline bool GetCollision() = 0;
		// -- Setters
		virtual inline void SetCollision(bool bIsCollided) = 0;

		// -- Interface Methods
		virtual void OnUpdate() = 0;
	};
	/// Collider2dRectComponent class
	/// Description:
	/// -- More optimized because uses Axis-Aligned-Bounding-Box for collision detection
	class NW_API Collider2dRectCmp : public ACollider2dCmp
	{
	public:
		Collider2dRectCmp(AEntity& rEntity);
		~Collider2dRectCmp() = default;

		// -- Getters
		virtual inline ADrawable* GetShape() override { return &m_Rect; }
		virtual inline bool GetCollision() override { return m_bCollision; }
		// -- Setters
		virtual inline void SetCollision(bool bIsCollided) override { m_bCollision = bIsCollided; }

		// -- Interface Methods
		virtual void OnUpdate() override;
	private:
		Rectangle m_Rect = Rectangle(V2f{ 1.0f, 1.0f });
		bool m_bCollision = false;
	};
	/// Collider2dPolygonCmp class
	class NW_API Collider2dPolyCmp : public ACollider2dCmp
	{
	public:
		Collider2dPolyCmp(AEntity& rEntity);
		~Collider2dPolyCmp() = default;

		// -- Getters
		virtual inline ADrawable* GetShape() override { return &m_Polygons; }
		virtual inline bool GetCollision() override { return m_bCollision; }
		// -- Setters
		virtual inline void SetCollision(bool bIsCollided) override { m_bCollision = bIsCollided; }

		// -- Interface Methods
		virtual void OnUpdate() override;
	private:
		Polygons m_Polygons = Polygons();
		bool m_bCollision = false;
	};
}

#endif	// ECS_COLLIDER_COMPONENT_H