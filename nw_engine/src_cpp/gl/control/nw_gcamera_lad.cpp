#include <nw_pch.hpp>
#include "nw_gcamera_lad.h"

#include <core/nw_event.h>

#include <sys/nw_time_sys.h>
#include <sys/nw_io_sys.h>

namespace NW
{
	GCameraLad::GCameraLad() :
		nZoomSpeed(1000.0f),
		nMoveSpeed(2.0f),
		nRtnSpeed(200.0f),
		whBounds{ 800.0f, 600.0f } { }
	GCameraLad::~GCameraLad() = default;

	// --==<core_methods>==--
	void GCameraLad::UpdateCamera(GCamera* pGCamera)
	{
		float MoveSpeed = this->nMoveSpeed * TimeSys::GetRealDelta();
		if (!IOSys::Cursor.bBlocked) return;
		pGCamera->nAspectRatio = whBounds.x / whBounds.y;
		if (pGCamera->GetMode() == GCamera::GCM_2D)
		{
			if (IOSys::GetKeyHeld(NW_KEY_W_87))
				pGCamera->yCrd += MoveSpeed;
			if (IOSys::GetKeyHeld(NW_KEY_S_83))
				pGCamera->yCrd -= MoveSpeed;
			if (IOSys::GetKeyHeld(NW_KEY_D_68))
				pGCamera->xCrd += MoveSpeed;
			if (IOSys::GetKeyHeld(NW_KEY_A_65))
				pGCamera->xCrd -= MoveSpeed;
		}
		else if (pGCamera->GetMode() == GCamera::GCM_3D) {
			MoveSpeed = -MoveSpeed;
			if (IOSys::GetKeyHeld(NW_KEY_W_87)) {	// Move Forward
				pGCamera->xCrd += pGCamera->dirFront.x * MoveSpeed;
				//pGCamera->yCrd += pGCamera->dirFront.y * MoveSpeed;
				pGCamera->zCrd += pGCamera->dirFront.z * MoveSpeed;
			} if (IOSys::GetKeyHeld(NW_KEY_S_83)) {	// Move Back
				pGCamera->xCrd -= pGCamera->dirFront.x * MoveSpeed;
				//pGCamera->yCrd -= pGCamera->dirFront.y * MoveSpeed;
				pGCamera->zCrd -= pGCamera->dirFront.z * MoveSpeed;
			} if (IOSys::GetKeyHeld(NW_KEY_D_68)) {	// Move Right
				pGCamera->xCrd += pGCamera->dirRight.x * MoveSpeed;
				pGCamera->yCrd += pGCamera->dirRight.y * MoveSpeed;
				pGCamera->zCrd += pGCamera->dirRight.z * MoveSpeed;
			} if (IOSys::GetKeyHeld(NW_KEY_A_65)) {	// Move Left
				pGCamera->xCrd -= pGCamera->dirRight.x * MoveSpeed;
				pGCamera->yCrd -= pGCamera->dirRight.y * MoveSpeed;
				pGCamera->zCrd -= pGCamera->dirRight.z * MoveSpeed;
			} if (IOSys::GetKeyHeld(NW_KEY_SPACE_32)) {		// Move Up
				pGCamera->yCrd -= MoveSpeed;
			} if (IOSys::GetKeyHeld(NW_KEY_SHIFT_LEFT)) {	// Move Down
				pGCamera->yCrd += MoveSpeed;
			}
		}
	}
	// --==</core_methods>==--

	// --==<--on_event_methods>==--
	void GCameraLad::OnEvent(MouseEvent& rmEvt)
	{
		switch (rmEvt.EvtType) {
		case ET_MOUSE_MOVE:
			if (!IOSys::Cursor.bBlocked) return;
			if (m_GCamera.GetMode() == GCamera::GCM_2D) {
				if (IOSys::GetMsButtonHeld(NW_MS_BTN_2)) {
					m_GCamera.xyzCrd.x += -IOSys::Cursor.xMoveDelta * TimeSys::GetRealDelta() * nMoveSpeed;
					m_GCamera.xyzCrd.y += IOSys::Cursor.yMoveDelta * TimeSys::GetRealDelta() * nMoveSpeed;
				}
				float nRoll_deg = m_GCamera.nRoll + IOSys::Cursor.xMoveDelta * nRtnSpeed * TimeSys::GetRealDelta();
				if (nRoll_deg < -nMaxRoll)
					m_GCamera.nRoll = nMaxRoll;
				else if (nRoll_deg > nMaxRoll)
					m_GCamera.nRoll = -nMaxRoll;
				else
					m_GCamera.nRoll = nRoll_deg;
			}
			else if (m_GCamera.GetMode() == GCamera::GCM_3D) {
				float nYaw_deg = m_GCamera.nYaw - IOSys::Cursor.xMoveDelta * nRtnSpeed * TimeSys::GetRealDelta();
				float nPitch_deg = m_GCamera.nPitch - IOSys::Cursor.yMoveDelta * nRtnSpeed * TimeSys::GetRealDelta();

				if (nYaw_deg < -nMaxYaw)
					m_GCamera.nYaw = nMaxYaw;
				else if (nYaw_deg > nMaxYaw)
					m_GCamera.nYaw = -nMaxYaw;
				else
					m_GCamera.nYaw = nYaw_deg;

				if (nPitch_deg > nMaxPitch)
					m_GCamera.nPitch = nMaxPitch;
				else if (nPitch_deg < -nMaxPitch)
					m_GCamera.nPitch = -nMaxPitch;
				else
					m_GCamera.nPitch = nPitch_deg;
			}
			break;
		case ET_MOUSE_SCROLL:
			if (!IOSys::Cursor.bBlocked) return;
			float nZoom = -rmEvt.nY * TimeSys::GetRealDelta() * nZoomSpeed;
			if (m_GCamera.GetType() == GCamera::GCT_ORTHO) {
				float nScale = m_GCamera.nViewScale + nZoom * m_GCamera.nViewScale / 40.0f + 0.01f;
				if (nScale > 0.0f) m_GCamera.nViewScale = nScale;
			}
			else if (m_GCamera.GetType() == GCamera::GCT_PERSPECT) {
				float nViewField = m_GCamera.nViewField + nZoom;
				if (nViewField >= 0.01f && nViewField <= 180.0f) m_GCamera.nViewField = nViewField;
			}
			break;
		}
	}
	void GCameraLad::OnEvent(KeyboardEvent& rkEvt)
	{
		switch (rkEvt.EvtType) {
		case ET_KEY_PRESS:
			switch (rkEvt.unKeyCode) {
			case NW_KEY_0_48:
				if (IOSys::GetKeyHeld(NW_KEY_C_67)) {
					m_GCamera = GCamera();
				}
				break;
			case NW_KEY_E_69:
				m_GCamera.nRoll += 10.0f;
				break;
			case NW_KEY_Q_81:
				m_GCamera.nRoll -= 10.0f;
				break;
			}
			break;
		}
	}
	void GCameraLad::OnEvent(WindowEvent& rwEvt)
	{
		switch (rwEvt.EvtType) {
		case ET_WINDOW_RESIZE:
			whBounds = V2f{ rwEvt.nX, rwEvt.nY };
			break;
		}
	}
	// --==</--on_event_methods>==--
}