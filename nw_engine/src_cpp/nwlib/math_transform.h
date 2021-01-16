#ifndef MATH_TRANSFORM_H
#define MATH_TRANSFORM_H

#if (NW_MATH & NW_MATH_NATIVE)
	#include <nw_core.hpp>
	#include <nw_pch.hpp>
#elif (NW_MATH & NW_MATH_GLM)
	#if defined NW_BUILD_DLL|| NW_BUILD_LIB
		#include <glm/gtc/matrix_transform.hpp>
	#elif defined NW_BUILD_EXE
		#include <glm/gtc/matrix_transform.hpp>
	#endif // NW_BUILD
#endif // NW_MATH
#include <nwlib/math_vector.h>
#include <nwlib/math_matrix.h>

namespace NW
{
#if (NW_MATH & NW_MATH_NATIVE)
	template <typename TT>
	struct NW_API Transform
	{
		V3f m_xyzCoord_l;
		Vec3<TT> m_xyzRotation_l;
		Vec3<TT> m_xyzScale_l;
		Vec3<TT> coord_g;
		Vec3<TT> rotat_g;
		Vec3<TT> scale_g;
		Mat4x4<TT> m_Mat4fModel;
		Mat4x4<TT> projMat;
		Mat4x4<TT> viewMat;

		Transform(const Vec3<TT> localCoord, const Vec3<TT> localRotation, const Vec3<TT> localScale,
			const Vec3<TT> globalCoord, const Vec3<TT> globalRotation, const Vec3<TT> globalScale) :
			m_xyzCoord_l(localCoord), m_xyzRotation_l(localRotation), m_xyzScale_l(localScale),
			coord_g(globalCoord), rotat_g(globalRotation), scale_g(globalScale) {}
	};
	// --==<matrices>==--
	template <typename MT>
	void MoveMtx(Mat4x4<MT>& inMtx, Vec3<MT> moveTo)
	{
		minMtx[0][3] += moveTo.x;
		minMtx[0][3] += moveTo.y;
		minMtx[0][3] += moveTo.z;
	}
	template <typename MT>
	Mat4x4<MT> GetMoveMtx(Mat4x4<MT>& inMtx, Vec3<MT> moveTo)
	{
		Mat4x4<MT> movedMtx = inMtx;
		movedMtx[0][3] += moveTo.x;
		movedMtx[1][3] += moveTo.y;
		movedMtx[2][3] += moveTo.z;
		return movedMtx;
	}
	template <typename MT>
	void RotateMtx(Mat4x4<MT>& mtx, Vec3<MT> rotateAround, MT radians)
	{
		return mtx;
	}
	template <typename MT>
	Mat4x4<MT>& GetRotateMtx(Mat4x4<MT> mtx, Vec3<MT> rotateAround, MT radians)
	{
		Mat4x4<MT> matRot;

		return matRot;
	}
	template <typename MT>
	Mat4x4<MT>& GetRotateMtx(Mat4x4<MT> mtx, Vec3<MT> rotations)
	{
		// Z
		Mat4x4<MT> matRot;
		matRot.m[0][0] = cosf(fTheta);
		matRot.m[0][1] = sinf(fTheta);
		matRot.m[1][0] = -sinf(fTheta);
		matRot.m[1][1] = cosf(fTheta);
		matRot.m[2][2] = 1;
		matRot.m[3][3] = 1;
		// X
		matRot.m[0][0] = 1;
		matRot.m[1][1] = cosf(fTheta * 0.5f);
		matRot.m[1][2] = sinf(fTheta * 0.5f);
		matRot.m[2][1] = -sinf(fTheta * 0.5f);
		matRot.m[2][2] = cosf(fTheta * 0.5f);
		matRot.m[3][3] = 1;
		return matRot;
	}
	template <typename MT>
	void ScaleMtx(Mat4x4<MT>& mtx, Vec3<MT> scale)
	{
		mtx[0][0] *= scale[0];
		mtx[1][1] *= scale[1];
		mtx[2][2] *= scale[2];
	}
	template <typename MT>
	Mat4x4<MT> GetScaleMtx(Mat4x4<MT>& mtx, Vec3<MT> scale)
	{
		Mat4x4<MT> scaleMtx;
		mtx[0][0] *= scale[0];
		mtx[1][1] *= scale[1];
		mtx[2][2] *= scale[2];
		return scaleMtx;
	}
	// Projection
	template <typename MT>
	Mat4x4<MT> PerspectMtx(MT viewField,
		MT aspectWH = 800.0f / 600.0f,
		MT nearF = 0.1f, MT farF = 100.0f)
	{
		MT fFovRad = 1.0f / tanf(viewField * 0.5f / 180.0f * 3.14159f);
		Mat4x4<MT> projMat;
		projMat[0][0] = aspectWH * fFovRad;
		projMat[1][1] = fFovRad;
		projMat[2][2] = farF / (farF - nearF);
		projMat[3][2] = (-farF * nearF) / (farF - nearF);
		projMat[2][3] = 1.0f;
		projMat[3][3] = 0.0f;
		return projMat;
	}
	template <typename MT>
	Mat4x4<MT> OrthoMtx(MT left, MT right, MT bottom, MT up, MT nearF, MT farF)
	{
		Mat4x4<MT> projMat;
		return projMat;
	}
	// --==</Matrices\>==--
#elif (NW_MATH & NW_MATH_GLM)
	/// Transform class
	/// Interface:
	/// -> Create -> Set coordinates, scale and rotation -> Setup -> get model matrix
	class NW_API Transform
	{
	public:
		float nScale;
		V3f xyzCrd;
		V3f xyzRtn;
	public:
		Transform(const V3f& xyzCoord = V3f(0.0f), const V3f& V3fRotation = V3f(0.0f), float nScale = 1.0f) :
			xyzCrd(xyzCoord), xyzRtn(V3fRotation), nScale(nScale),
			m_m4Model(Mat4f(1.0f)) {}

		// --getters
		inline const Mat4f& GetModelMatrix() { UpdateData(); return m_m4Model; }
		// --setters
		// -- Core Functions
		void UpdateData()
		{
			m_m4Model = Mat4f(1.0f);
			m_m4Model = glm::translate(m_m4Model, xyzCrd);
			m_m4Model = glm::rotate(m_m4Model, glm::radians(xyzRtn.x), V3f(0.0f, 1.0f, 0.0f));
			m_m4Model = glm::rotate(m_m4Model, glm::radians(xyzRtn.y), V3f(1.0f, 0.0f, 0.0f));
			m_m4Model = glm::rotate(m_m4Model, glm::radians(xyzRtn.z), V3f(0.0f, 0.0f, 1.0f));
			m_m4Model = glm::scale(m_m4Model, V3f{ nScale });
		}
		void Reset()
		{
			xyzCrd = V3f(0.0f);
			xyzRtn = V3f(0.0f);
			nScale = 1.0f;

			m_m4Model = Mat4f(1.0f);
		}
	private:
		Mat4f m_m4Model;
	};
#endif // NW_MATH
}

#endif // MATH_TRANSFORM_H