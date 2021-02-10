#ifndef GFX_CAMERA_LAD_H
#define GFX_CAMERA_LAD_H

#include <gfx_core.hpp>
#include <gfx/gfx_camera.h>

namespace NW
{
	/// GraphicsCameraLad Singleton class
	/// Interface:
	/// -> Get static instance -> set current camera -> configure if it's required
	/// -> Update every frame with the set camera -> Dispatch events to GfxCameraLad
	class NW_API GfxCameraLad
	{
	public: // Configurable Attributes
		float nRtnSpeed, nMoveSpeed, nZoomSpeed;

		float nMaxYaw = 360.0f;
		float nMaxPitch = 90.0f;
		float nMaxRoll = 0.0f;

		V2f whBounds = V2f{ 0.0f, 0.0f };
	private:
		GfxCameraLad();
		GfxCameraLad(GfxCameraLad& rCpy) = delete;
		GfxCameraLad(const GfxCameraLad& rCpy) = delete;
		void operator=(GfxCameraLad& rCpy) = delete;
		void operator=(const GfxCameraLad& rCpy) = delete;
	public:
		~GfxCameraLad();
		// --getters
		static inline GfxCameraLad& Get() { static GfxCameraLad s_Lad; return s_Lad; }
		inline GfxCamera* GetGfxCamera() { return &m_gCamera; }
		// --setters
		// --core_methods
		void UpdateCamera(GfxCamera& rCamera);
		inline void UpdateCamera() { UpdateCamera(m_gCamera); }
		void OnEvent(MouseEvent& rmEvt, GfxCamera& rCamera);
		void OnEvent(KeyboardEvent& rkEvt, GfxCamera& rCamera);
		void OnEvent(WindowEvent& rwEvt, GfxCamera& rCamera);
		inline void OnEvent(MouseEvent& rmEvt) { OnEvent(rmEvt, m_gCamera); }
		inline void OnEvent(KeyboardEvent& rkEvt) { OnEvent(rkEvt, m_gCamera); }
		inline void OnEvent(WindowEvent& rwEvt) { OnEvent(rwEvt, m_gCamera); }
	private:
		GfxCamera m_gCamera;
	};
}

#endif	// GFX_CAMERA_LAD_H