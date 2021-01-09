#ifndef NW_GCAMERA_LAD_H
#define NW_GCAMERA_LAD_H

#include <glib/vision/nw_gcamera.h>

#include <lib/nw_singleton.h>

#include <nw_decl.hpp>

namespace NW
{
	/// GraphicsCameraLad Singleton class
	/// Interface:
	/// -> Get static instance -> set current camera -> configure if it's required
	/// -> Update every frame with the set camera -> Dispatch events to GCameraLad
	class NW_API GCameraLad : public ASingleton<GCameraLad>
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
		void OnEvent(MouseEvent& rmEvt);
		void OnEvent(KeyboardEvent& rkEvt);
		void OnEvent(WindowEvent& rwEvt);
	private:// Implementation Attributes
		GCamera m_GCamera;
	};
}

#endif	// NW_GCAMERA_LAD_H