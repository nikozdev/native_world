#ifndef GFX_CAMERA_LAD_H
#define GFX_CAMERA_LAD_H

#include <gfx_core.hpp>
#include <gfx_camera.h>

namespace NW
{
	/// GraphicsCameraLad Singleton class
	/// Interface:
	/// -> Get static instance -> set current camera -> configure if it's required
	/// -> Update every frame with the set camera -> Dispatch events to GCameraLad
	class GFX_API GCameraLad
	{
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
		static inline GCameraLad& Get() { static GCameraLad s_Lad; return s_Lad; }
		inline GCamera* GetGCamera() { return &m_gCamera; }
		// --setters
		// --core_methods
		void UpdateCamera(GCamera& rCamera);
		inline void UpdateCamera() { UpdateCamera(m_gCamera); }
		void OnEvent(MouseEvent& rmEvt, GCamera& rCamera);
		void OnEvent(KeyboardEvent& rkEvt, GCamera& rCamera);
		void OnEvent(WindowEvent& rwEvt, GCamera& rCamera);
		inline void OnEvent(MouseEvent& rmEvt) { OnEvent(rmEvt, m_gCamera); }
		inline void OnEvent(KeyboardEvent& rkEvt) { OnEvent(rkEvt, m_gCamera); }
		inline void OnEvent(WindowEvent& rwEvt) { OnEvent(rwEvt, m_gCamera); }
	private:
		GCamera m_gCamera;
	};
}

#endif	// GFX_CAMERA_LAD_H