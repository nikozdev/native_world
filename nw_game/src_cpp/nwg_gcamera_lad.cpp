#include <nwg_pch.hpp>
#include "nwg_gcamera_lad.h"

#include <sys/nw_io_sys.h>

namespace NWG
{
	GCameraLad::GCameraLad() :
		nZoomSpeed(1000.0f),
		nMoveSpeed(2.0f),
		nRtnSpeed(200.0f),
		whBounds{ 800.0f, 600.0f } { }
	GCameraLad::~GCameraLad() = default;

	// --==<core_methods>==--
	void GCameraLad::UpdateCamera(GCamera* pCamera)
	{
		pCamera->nAspectRatio = whBounds.x / whBounds.y;
		if (!(IOSys::GetMouseIMode() & IM_CURSOR_DISABLED)) { return; }
		float MoveSpeed = this->nMoveSpeed * TimeSys::GetDeltaS();
		if (pCamera->GetMode() == GCM_2D)
		{
			if (IOSys::GetKeyHeld(NW_KEY_W_87))
				pCamera->yCrd += MoveSpeed;
			if (IOSys::GetKeyHeld(NW_KEY_S_83))
				pCamera->yCrd -= MoveSpeed;
			if (IOSys::GetKeyHeld(NW_KEY_D_68))
				pCamera->xCrd += MoveSpeed;
			if (IOSys::GetKeyHeld(NW_KEY_A_65))
				pCamera->xCrd -= MoveSpeed;
		}
		else if (pCamera->GetMode() == GCM_3D) {
			MoveSpeed = -MoveSpeed;
			if (IOSys::GetKeyHeld(NW_KEY_W_87)) {	// Move Forward
				pCamera->xCrd += pCamera->dirFront.x * MoveSpeed;
				//pCamera->yCrd += pCamera->dirFront.y * MoveSpeed;
				pCamera->zCrd += pCamera->dirFront.z * MoveSpeed;
			} if (IOSys::GetKeyHeld(NW_KEY_S_83)) {	// Move Back
				pCamera->xCrd -= pCamera->dirFront.x * MoveSpeed;
				//pCamera->yCrd -= pCamera->dirFront.y * MoveSpeed;
				pCamera->zCrd -= pCamera->dirFront.z * MoveSpeed;
			} if (IOSys::GetKeyHeld(NW_KEY_D_68)) {	// Move Right
				pCamera->xCrd += pCamera->dirRight.x * MoveSpeed;
				pCamera->yCrd += pCamera->dirRight.y * MoveSpeed;
				pCamera->zCrd += pCamera->dirRight.z * MoveSpeed;
			} if (IOSys::GetKeyHeld(NW_KEY_A_65)) {	// Move Left
				pCamera->xCrd -= pCamera->dirRight.x * MoveSpeed;
				pCamera->yCrd -= pCamera->dirRight.y * MoveSpeed;
				pCamera->zCrd -= pCamera->dirRight.z * MoveSpeed;
			} if (IOSys::GetKeyHeld(NW_KEY_SPACE_32)) {		// Move Up
				pCamera->yCrd -= MoveSpeed;
			} if (IOSys::GetKeyHeld(NW_KEY_SHIFT_LEFT)) {	// Move Down
				pCamera->yCrd += MoveSpeed;
			}
		}
		if (IOSys::GetKeyHeld(NW_KEY_CONTROL_LEFT)) {
			if (IOSys::GetKeyHeld(NW_KEY_C_67)) {
				if (IOSys::GetKeyHeld(NW_KEY_2_50)) { pCamera->SetMode(GCM_2D); }
				else if (IOSys::GetKeyHeld(NW_KEY_3_51)) { pCamera->SetMode(GCM_3D); }
				if (IOSys::GetKeyHeld(NW_KEY_P_80)) { pCamera->SetType(GCT_PERSPECT); }
				else if (IOSys::GetKeyHeld(NW_KEY_O_79)) { pCamera->SetType(GCT_ORTHO); }
			}
		}
	}
	// --==</core_methods>==--

	// --==<--on_event_methods>==--
	void GCameraLad::OnEvent(MouseEvent& rmEvt, GCamera* pCamera)
	{
		if (!(IOSys::GetMouseIMode() & IM_CURSOR_DISABLED)) { return; }
		switch (rmEvt.evType) {
		case ET_MOUSE_MOVE:
			if (pCamera->GetMode() == GCM_2D) {
				if (IOSys::GetMouseHeld(NW_MS_BTN_2)) {
					pCamera->xyzCrd.x += -IOSys::GetMouseMoveDeltaX() * TimeSys::GetDeltaS() * nMoveSpeed;
					pCamera->xyzCrd.y += IOSys::GetMouseMoveDeltaY() * TimeSys::GetDeltaS() * nMoveSpeed;
				}
				float nRoll_deg = pCamera->nRoll + IOSys::GetMouseMoveDeltaX() * nRtnSpeed * TimeSys::GetDeltaS();
				if (nRoll_deg < -nMaxRoll) { pCamera->nRoll = nMaxRoll; }
				else if (nRoll_deg > nMaxRoll) { pCamera->nRoll = -nMaxRoll; }
				else { pCamera->nRoll = nRoll_deg; }
			}
			else if (pCamera->GetMode() == GCM_3D) {
				float nYaw_deg = pCamera->nYaw - IOSys::GetMouseMoveDeltaX() * nRtnSpeed * TimeSys::GetDeltaS();
				float nPitch_deg = pCamera->nPitch - IOSys::GetMouseMoveDeltaY() * nRtnSpeed * TimeSys::GetDeltaS();

				if (nYaw_deg < -nMaxYaw) { pCamera->nYaw = nMaxYaw; }
				else if (nYaw_deg > nMaxYaw) { pCamera->nYaw = -nMaxYaw; }
				else { pCamera->nYaw = nYaw_deg; }

				if (nPitch_deg > nMaxPitch) { pCamera->nPitch = nMaxPitch; }
				else if (nPitch_deg < -nMaxPitch) { pCamera->nPitch = -nMaxPitch; }
				else { pCamera->nPitch = nPitch_deg; }
			}
			break;
		case ET_MOUSE_SCROLL:
			if (!(IOSys::GetMouseIMode() & IM_CURSOR_DISABLED)) return;
			float nZoom = -rmEvt.nY * nZoomSpeed * TimeSys::GetDeltaS();
			if (pCamera->GetType() == GCT_ORTHO) {
				float nScale = pCamera->nViewScale + nZoom * pCamera->nViewScale / 40.0f + 0.01f;
				if (nScale > 0.0f) pCamera->nViewScale = nScale;
			}
			else if (pCamera->GetType() == GCT_PERSPECT) {
				float nViewField = pCamera->nViewField + nZoom;
				if (nViewField >= 0.01f && nViewField <= 180.0f) pCamera->nViewField = nViewField;
			}
			break;
		}
	}
	void GCameraLad::OnEvent(KeyboardEvent& rkEvt, GCamera* pCamera)
	{
		if (!(IOSys::GetMouseIMode() & IM_CURSOR_DISABLED)) { return; }
		switch (rkEvt.evType) {
		case ET_KEY_RELEASE:
			switch (rkEvt.unKeyCode) {
			case NW_KEY_0_48:
				if (IOSys::GetKeyHeld(NW_KEY_C_67)) {
					m_gCamera = GCamera();
				}
				break;
			case NW_KEY_E_69:
				pCamera->nRoll += 10.0f;
				break;
			case NW_KEY_Q_81:
				pCamera->nRoll -= 10.0f;
				break;
			}
			break;
		}
	}
	void GCameraLad::OnEvent(WindowEvent& rwEvt, GCamera* pCamera)
	{
		switch (rwEvt.evType) {
		case ET_WINDOW_RESIZE:
			whBounds = V2f{ rwEvt.nX, rwEvt.nY };
			break;
		}
	}
	// --==</--on_event_methods>==--
}