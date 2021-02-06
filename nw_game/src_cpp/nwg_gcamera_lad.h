#ifndef NWG_GCAMERA_LAD_H
#define NWG_GCAMERA_LAD_H

#include <nwg_pch.hpp>

namespace NWG
{
	/// GraphicsCameraLad Singleton class
	/// Interface:
	/// -> Get static instance -> set current camera -> configure if it's required
	/// -> Update every frame with the set camera -> Dispatch events to GCameraLad
	class GCameraLad
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
		inline void UpdateCamera() { UpdateCamera(&m_gCamera); }
		void UpdateCamera(GCamera* pCamera);
		void OnEvent(MouseEvent& rmEvt, GCamera* pCamera);
		void OnEvent(KeyboardEvent& rkEvt, GCamera* pCamera);
		void OnEvent(WindowEvent& rwEvt, GCamera* pCamera);
		inline void OnEvent(MouseEvent& rmEvt) { OnEvent(rmEvt, &m_gCamera); }
		inline void OnEvent(KeyboardEvent& rkEvt) { OnEvent(rkEvt, &m_gCamera); }
		inline void OnEvent(WindowEvent& rwEvt) { OnEvent(rwEvt, &m_gCamera); }
	private:
		GCamera m_gCamera;
	};
}

#endif	// NWG_GCAMERA_LAD_H