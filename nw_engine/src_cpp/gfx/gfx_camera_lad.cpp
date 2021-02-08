#include <gfx_pch.hpp>
#include "gfx_camera_lad.h"

namespace NW
{
	GCameraLad::GCameraLad() :
		nZoomSpeed(1000.0f),
		nMoveSpeed(2.0f),
		nRtnSpeed(200.0f),
		whBounds{ 800.0f, 600.0f } { }
	GCameraLad::~GCameraLad() = default;

	// --==<core_methods>==--
	void GCameraLad::UpdateCamera(GCamera& rCamera)
	{
		rCamera.nAspectRatio = whBounds.x / whBounds.y;
		if (!(IOSys::GetMouseIMode() & IM_CURSOR_DISABLED)) { return; }
		float MoveSpeed = this->nMoveSpeed * TimeSys::GetDeltaS();
		if (rCamera.GetMode() == GCM_2D)
		{
			if (IOSys::GetKeyHeld(KC_W))
				rCamera.yCrd += MoveSpeed;
			if (IOSys::GetKeyHeld(KC_S))
				rCamera.yCrd -= MoveSpeed;
			if (IOSys::GetKeyHeld(KC_D))
				rCamera.xCrd += MoveSpeed;
			if (IOSys::GetKeyHeld(KC_A))
				rCamera.xCrd -= MoveSpeed;
		}
		else if (rCamera.GetMode() == GCM_3D) {
			MoveSpeed = -MoveSpeed;
			if (IOSys::GetKeyHeld(KC_W)) {	// Move Forward
				rCamera.xCrd += rCamera.dirFront.x * MoveSpeed;
				//rCamera.yCrd += rCamera.dirFront.y * MoveSpeed;
				rCamera.zCrd += rCamera.dirFront.z * MoveSpeed;
			} if (IOSys::GetKeyHeld(KC_S)) {	// Move Back
				rCamera.xCrd -= rCamera.dirFront.x * MoveSpeed;
				//rCamera.yCrd -= rCamera.dirFront.y * MoveSpeed;
				rCamera.zCrd -= rCamera.dirFront.z * MoveSpeed;
			} if (IOSys::GetKeyHeld(KC_D)) {	// Move Right
				rCamera.xCrd += rCamera.dirRight.x * MoveSpeed;
				rCamera.yCrd += rCamera.dirRight.y * MoveSpeed;
				rCamera.zCrd += rCamera.dirRight.z * MoveSpeed;
			} if (IOSys::GetKeyHeld(KC_A)) {	// Move Left
				rCamera.xCrd -= rCamera.dirRight.x * MoveSpeed;
				rCamera.yCrd -= rCamera.dirRight.y * MoveSpeed;
				rCamera.zCrd -= rCamera.dirRight.z * MoveSpeed;
			} if (IOSys::GetKeyHeld(KC_SPACE)) {		// Move Up
				rCamera.yCrd -= MoveSpeed;
			} if (IOSys::GetKeyHeld(KC_LSHIFT)) {	// Move Down
				rCamera.yCrd += MoveSpeed;
			}
		}
	}
	// --==</core_methods>==--

	// --==<--on_event_methods>==--
	void GCameraLad::OnEvent(MouseEvent& rmEvt, GCamera& rCamera)
	{
		if (!(IOSys::GetMouseIMode() & IM_CURSOR_DISABLED)) { return; }
		switch (rmEvt.evType) {
		case ET_MOUSE_MOVE:
			if (rCamera.GetMode() == GCM_2D) {
				if (IOSys::GetMouseHeld(MSB_RIGHT)) {
					rCamera.xyzCrd.x += -IOSys::GetMouseMoveDeltaX() * TimeSys::GetDeltaS() * nMoveSpeed;
					rCamera.xyzCrd.y += IOSys::GetMouseMoveDeltaY() * TimeSys::GetDeltaS() * nMoveSpeed;
				}
				float nRoll_deg = rCamera.nRoll + IOSys::GetMouseMoveDeltaX() * nRtnSpeed * TimeSys::GetDeltaS();
				if (nRoll_deg < -nMaxRoll) { rCamera.nRoll = nMaxRoll; }
				else if (nRoll_deg > nMaxRoll) { rCamera.nRoll = -nMaxRoll; }
				else { rCamera.nRoll = nRoll_deg; }
			}
			else if (rCamera.GetMode() == GCM_3D) {
				float nYaw_deg = rCamera.nYaw - IOSys::GetMouseMoveDeltaX() * nRtnSpeed * TimeSys::GetDeltaS();
				float nPitch_deg = rCamera.nPitch - IOSys::GetMouseMoveDeltaY() * nRtnSpeed * TimeSys::GetDeltaS();

				if (nYaw_deg < -nMaxYaw) { rCamera.nYaw = nMaxYaw; }
				else if (nYaw_deg > nMaxYaw) { rCamera.nYaw = -nMaxYaw; }
				else { rCamera.nYaw = nYaw_deg; }

				if (nPitch_deg > nMaxPitch) { rCamera.nPitch = nMaxPitch; }
				else if (nPitch_deg < -nMaxPitch) { rCamera.nPitch = -nMaxPitch; }
				else { rCamera.nPitch = nPitch_deg; }
			}
			break;
		case ET_MOUSE_SCROLL:
			if (!(IOSys::GetMouseIMode() & IM_CURSOR_DISABLED)) return;
			float nZoom = -rmEvt.nY * nZoomSpeed * TimeSys::GetDeltaS();
			if (rCamera.GetType() == GCT_ORTHO) {
				float nScale = rCamera.nViewScale + nZoom * rCamera.nViewScale / 40.0f + 0.01f;
				if (nScale > 0.0f) rCamera.nViewScale = nScale;
			}
			else if (rCamera.GetType() == GCT_PERSPECT) {
				float nViewField = rCamera.nViewField + nZoom;
				if (nViewField >= 0.01f && nViewField <= 180.0f) rCamera.nViewField = nViewField;
			}
			break;
		}
	}
	void GCameraLad::OnEvent(KeyboardEvent& rkEvt, GCamera& rCamera)
	{
		if (!(IOSys::GetMouseIMode() & IM_CURSOR_DISABLED)) { return; }
		if (IOSys::GetKeyHeld(KC_LCTRL)) {
			if (IOSys::GetKeyHeld(KC_C)) {
				if (IOSys::GetKeyHeld(KC_K0)) { rCamera = GCamera(); }
				else if (IOSys::GetKeyHeld(KC_K2)) { rCamera.SetMode(GCM_2D); rCamera.SetType(GCT_ORTHO); }
				else if (IOSys::GetKeyHeld(KC_K3)) { rCamera.SetMode(GCM_3D); rCamera.SetType(GCT_PERSPECT); }
			}
		}
		switch (rkEvt.evType) {
		case ET_KEY_RELEASE:
			switch (rkEvt.unKeyCode) {
			case KC_K0:
				break;
			default: break;
			}
			break;
		}
	}
	void GCameraLad::OnEvent(WindowEvent& rwEvt, GCamera& rCamera)
	{
		switch (rwEvt.evType) {
		case ET_WINDOW_RESIZE:
			whBounds = V2f{ rwEvt.nX, rwEvt.nY };
			break;
		}
	}
	// --==</--on_event_methods>==--
}