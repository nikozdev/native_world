#include <nw_pch.hpp>
#include <ecs/nw_scene.h>

namespace NW
{
	// --==<Transform2dCmp>==--
	Transform2dCmp::Transform2dCmp(AEntity& rEntity) :
		ATransformCmp(rEntity),
		m_xyCrd{ 0.0f, 0.0f }, m_zRtn{ 0.0f }, m_xyScl{ 1.0f, 1.0f } {}

	// --getters
	inline Mat4f Transform2dCmp::GetTransformMatrix() const {
		Mat4f m4TForm = Mat4f(1.0f);
		m4TForm = glm::translate(m4TForm, V3f{ m_xyCrd.x, m_xyCrd.y, 0.0f });
		m4TForm = glm::rotate(m4TForm, m_zRtn * 3.14f / 180.0f, V3f{ 0.0f, 0.0f, 1.0f });
		m4TForm = glm::scale(m4TForm, V3f{ m_xyScl.x, m_xyScl.y, 1.0f });
		return m4TForm;
	}

	// --core_methods
	void Transform2dCmp::OnUpdate()
	{
		Mat4f m4TForm = GetTransformMatrix();
		if (AEntity* pOverEnt = GetEntity()) {
			while (pOverEnt = pOverEnt->GetOverEnt()) {
				if (ATransformCmp* pTFCmp = pOverEnt->GetComponent<ATransformCmp>()) {
					m4TForm = glm::translate(m4TForm, V3f{ pTFCmp->GetCoord().x, pTFCmp->GetCoord().y, 0.0f });
					m4TForm = glm::rotate(m4TForm, pTFCmp->GetRotation().z * 3.14f / 180.0f, V3f(0.0f, 0.0f, 1.0f));
					m4TForm = glm::scale(m4TForm, V3f{ pTFCmp->GetScale().x, pTFCmp->GetScale().y, 1.0f });
				}
			}
		}
		if (AGraphicsCmp* pGCmp = GetEntity()->GetComponent<AGraphicsCmp>())
			pGCmp->GetDrawable()->m4Transform = m4TForm;
	}
	// --==</Transform2dCmp>==--

	// --==<Transform3dCmp>==--
	Transform3dCmp::Transform3dCmp(AEntity& rEntity) :
		ATransformCmp(rEntity),
		m_xyzCrd{ 0.0f, 0.0f, 0.0f }, m_xyzRtn{ 0.0f, 0.0f, 0.0f }, m_xyzScl{ 1.0f, 1.0f, 1.0f } {}

	// --getters
	inline Mat4f Transform3dCmp::GetTransformMatrix() const {
		Mat4f m4TForm = Mat4f(1.0f);
		m4TForm = glm::translate(m4TForm, m_xyzCrd);
		m4TForm = glm::rotate(m4TForm, m_xyzRtn.x * 3.14f / 180.0f, V3f{ 1.0f, 0.0f, 0.0f });
		m4TForm = glm::rotate(m4TForm, m_xyzRtn.y * 3.14f / 180.0f, V3f{ 0.0f, 1.0f, 0.0f });
		m4TForm = glm::rotate(m4TForm, m_xyzRtn.z * 3.14f / 180.0f, V3f{ 0.0f, 0.0f, 1.0f });
		m4TForm = glm::scale(m4TForm, m_xyzScl);
		return m4TForm;
	}

	// --core_methods
	void Transform3dCmp::OnUpdate()
	{
		Mat4f m4TForm = GetTransformMatrix();
		if (AEntity* pOverEnt = GetEntity()) {
			while (pOverEnt = pOverEnt->GetOverEnt()) {
				if (ATransformCmp* pTFCmp = pOverEnt->GetComponent<ATransformCmp>()) {
					m4TForm = glm::translate(m4TForm, V3f{ pTFCmp->GetCoord().x, pTFCmp->GetCoord().y, 0.0f });
					m4TForm = glm::rotate(m4TForm, pTFCmp->GetRotation().x * 3.14f / 180.0f, V3f(1.0f, 0.0f, 0.0f));
					m4TForm = glm::rotate(m4TForm, pTFCmp->GetRotation().y * 3.14f / 180.0f, V3f(0.0f, 1.0f, 0.0f));
					m4TForm = glm::rotate(m4TForm, pTFCmp->GetRotation().z * 3.14f / 180.0f, V3f(0.0f, 0.0f, 1.0f));
					m4TForm = glm::scale(m4TForm, V3f{ pTFCmp->GetScale().x, pTFCmp->GetScale().y, 1.0f });
				}
			}
		}
		if (AGraphicsCmp* pGCmp = GetEntity()->GetComponent<AGraphicsCmp>())
			pGCmp->GetDrawable()->m4Transform = m4TForm;
	}
	// --==</Transform3dCmp>==--

}