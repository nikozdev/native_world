#ifndef NW_GCAMERA_H
#define NW_GCAMERA_H

#include <gl/nw_gl_core.h>

#include <lib/utils/math_transform.h>

namespace NW
{
	/// GraphicsCamera struct
	/// Description:
	/// -- Makes all relevant rotation and movement calculations
	/// -- Makes projection and view-lookAt matricies
	/// Interface:
	/// -> Config Frustrum, tarGet, 
	/// -> Set coordinates and rotation
	/// -> Get transform matricies: proj and view
	struct NW_API GCamera
	{
	public:
		enum GCameraModes;
		enum GCameraTypes;
	public:
		union {
			struct {
				float x, y, z;
			} xyzCrd;
			struct {
				float xCrd, yCrd, zCrd;
			};
		};
		union {
			struct {
				float x, y, z;
			}xyzRtn;
			struct {
				float xRtn, yRtn, zRtn;
			};
			struct {
				float nPitch, nYaw, nRoll;
			};
		};

		float nViewField = 45.0f, nViewScale = 1.0f;
		float nAspectRatio = 16.0f / 9.0f;
		float nNearClip = 0.1f, nFarClip = 100.0f;

		V3f dirRight = V3f{ 1.0f, 0.0f, 0.0f },
			dirUp = V3f{ 0.0f, 1.0f, 0.0f },
			dirFront = V3f{ 0.0f, 0.0f, 1.0f },
			dirWorldUp = V3f{ 0.0f, 1.0f, 0.0f };
	public:
		GCamera();
		~GCamera() = default;

		// --getters
		virtual inline const Mat4f& GetViewMatrix();
		virtual inline const Mat4f& GetProjMatrix();
		inline GCameraTypes& GetType() { return m_gcType; }
		inline GCameraModes& GetMode() { return m_gcMode; }
		// --setters
		inline void SetType(GCameraTypes gcType) {
			m_gcType = gcType;
			if (m_gcType == GCT_ORTHO) {
				nNearClip = -100.0f;
				nFarClip = 100.0f;
			}
			else if (m_gcType == GCT_PERSPECT) {
				nNearClip = 0.01f;
				nFarClip = 1000.0f;
			}
		}
		inline void SetMode(GCameraModes gcMode) {
			m_gcMode = gcMode;
			if (m_gcMode == GCM_2D) {
				zCrd = 1.0f;
			} else if (m_gcMode == GCM_2D) {
				zCrd = 1.0f;
			}
		}
	public:
		enum GCameraTypes {
			GCT_ORTHO = 0,
			GCT_PERSPECT = 1
		};
		enum GCameraModes {
			GCM_2D = 2,
			GCM_3D = 3
		};
	private:
		Mat4f m_m4View, m_m4Proj;

		GCameraTypes m_gcType;
		GCameraModes m_gcMode;
	};
}
#endif // NW_GCAMERA_H