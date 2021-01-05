#ifndef ECS_TRANSFORM_COMPONENT_H
#define ECS_TRANSFORM_COMPONENT_H
#include <ecs/nw_entity_cmp.h>

#include <lib/utils/math_transform.h>

namespace NW
{
	/// Abstract TransformComponent class
	class NW_API ATransformCmp : public AEntityCmp
	{
	public:
		ATransformCmp(AEntity& rEntity) : AEntityCmp(rEntity, std::type_index(typeid(ATransformCmp))) { }
		~ATransformCmp() = default;

		// -- Getters
		virtual inline Mat4f GetTransformMatrix() const = 0;
		virtual inline V3f GetCoord() const = 0;
		virtual inline V3f GetRotation() const = 0;
		virtual inline V3f GetScale() const = 0;
		// -- Setters
		virtual inline void SetCoord(const V3f& xyzCrd) = 0;
		virtual inline void SetRotation(const V3f& xyzRtn) = 0;
		virtual inline void SetScale(const V3f& xyzScl) = 0;

		virtual inline void Reset() = 0;

		// -- Interface Methods
		virtual void OnUpdate() = 0;
	};
	/// Transform2dComponent class
	class NW_API Transform2dCmp : public ATransformCmp
	{
	public:
		Transform2dCmp(AEntity& rEntity);
		~Transform2dCmp() = default;

		// -- Getters
		virtual inline Mat4f GetTransformMatrix() const override;
		virtual inline V3f GetCoord() const override { return V3f{ m_xyCrd.x, m_xyCrd.y, 0.0f }; }
		virtual inline V3f GetRotation() const override { return V3f{ 0.0f, 0.0f, m_zRtn }; }
		virtual inline V3f GetScale() const override { return V3f{ m_xyScl.x, m_xyScl.y, 1.0f }; }
		// -- Setters
		virtual inline void SetCoord(const V3f& xyzCrd) override { m_xyCrd = V2f{ xyzCrd.x, xyzCrd.y }; }
		virtual inline void SetRotation(const V3f& xyzRtn)override { m_zRtn = xyzRtn.z; }
		virtual inline void SetScale(const V3f& xyzScl) override { m_xyScl = V2f{ xyzScl.x, xyzScl.y }; }
		virtual inline void Reset() override { m_xyCrd = V2f(0.0f); m_zRtn = 0.0f; m_xyScl = V2f{ 1.0f, 1.0f }; }

		// -- Interface Methods
		virtual void OnUpdate() override;
	private:
		V2f m_xyCrd;
		float m_zRtn;
		V2f m_xyScl;
	};
	/// Transform3dComponent class
	class NW_API Transform3dCmp : public ATransformCmp
	{
	public:
		Transform3dCmp(AEntity& rEntity);
		~Transform3dCmp() = default;

		// -- Getters
		virtual inline Mat4f GetTransformMatrix() const override;
		virtual inline V3f GetCoord() const override { return m_xyzCrd; }
		virtual inline V3f GetRotation() const override { return m_xyzRtn; }
		virtual inline V3f GetScale() const override { return m_xyzScl; }
		// -- Setters
		virtual inline void SetCoord(const V3f& xyzCrd) override { m_xyzCrd = xyzCrd; }
		virtual inline void SetRotation(const V3f& xyzRtn) override { m_xyzRtn = xyzRtn; }
		virtual inline void SetScale(const V3f& xyzScl) override { m_xyzScl = xyzScl; }
		virtual inline void Reset() override {
			m_xyzCrd = V3f{ 0.0f, 0.0f, 0.0f };
			m_xyzRtn = V3f{ 0.0f, 0.0f, 0.0f };
			m_xyzScl = V3f{ 1.0f, 1.0f, 1.0f };
		}

		// -- Interface Methods
		virtual void OnUpdate() override;
	private:
		V3f m_xyzCrd;
		V3f m_xyzRtn;
		V3f m_xyzScl;
	};
}

#endif	// ECS_TRANSFORM_COMPONENT_H