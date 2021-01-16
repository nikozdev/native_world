#ifndef GLIB_CAMERA_LAD_H
#define GLIB_CAMERA_LAD_H

#include <glib/vision/glib_camera.h>

#include <nwlib/glib_singleton.h>

#include <glib_decl.hpp>

namespace GLIB
{
	/// GraphicsCameraLad Singleton class
	/// Interface:
	/// -> Get static instance -> set current camera -> configure if it's required
	/// -> Update every frame with the set camera -> Dispatch events to GCameraLad
	class GLIB_API GCameraLad : public ASingleton<GCameraLad>
	{
		friend class ASingleton<GCameraLad>;
	public: // Configurable Attributes
		float nRtnSpeed, nMoveSpeed, nZoomSpeed;

		float nMaxYaw = 360.0f;
		float nMaxPitch = 90.0f;
		float nMaxRoll = 0.0f;

		V2f whBounds = V2f{ 0.0f, 0.0f };
	private:
		GCameraLad();
		GCameraLad(GCameraLad& rCpy) = delete;
		GCameraLad(const GCameraLad& rCpy) = delete;
		void operator=(GCameraLad& rCpy) = delete;
		void operator=(const GCameraLad& rCpy) = delete;
	public:
		~GCameraLad();

		// --getters
		inline GCamera* GetGCamera() { return &m_GCamera; }
		// --setters

		// --core_methods
		inline void UpdateCamera() { UpdateCamera(&m_GCamera); }
		void UpdateCamera(GCamera* pGCamera);

		// --on_event_methods
		void OnEvent(MouseEvent& rmEvt, GCamera* pGCamera);
		void OnEvent(KeyboardEvent& rkEvt, GCamera* pGCamera);
		void OnEvent(WindowEvent& rwEvt, GCamera* pGCamera);
		inline void OnEvent(MouseEvent& rmEvt) { OnEvent(rmEvt, &m_GCamera); }
		inline void OnEvent(KeyboardEvent& rkEvt) { OnEvent(rkEvt, &m_GCamera); }
		inline void OnEvent(WindowEvent& rwEvt) { OnEvent(rwEvt, &m_GCamera); }
	private:// Implementation Attributes
		GCamera m_GCamera;
	};
}

#endif	// GLIB_CAMERA_LAD_H