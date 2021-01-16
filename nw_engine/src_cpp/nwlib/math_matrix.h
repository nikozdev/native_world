#ifndef NW_MATH_MATRIX_H
#define NW_MATH_MATRIX_H

#if (NW_MATH & NW_MATH_NATIVE)
	#include <math/math_vec.h>
#elif (NW_MATH & NW_MATH_GLM)
	#if defined NW_BUILD_DLL || NW_BUILD_LIB
		#include <glm/mat3x3.hpp>
		#include <glm/mat4x4.hpp>
	#elif defined NW_BUILD_EXE
		#include <glm/mat3x3.hpp>
		#include <glm/mat4x4.hpp>
	#endif  // NW_BUILD
#endif // NW_MATH

namespace NW
{
#if (NW_MATH & NW_MATH_NATIVE)
	/// CheerNik 4x4-Matrix_generic implementation
	/// --Can be used for:
	/// -->Transformation Matricies
	/// ||==Projection matrix
	/// ||==View matrix
	/// ||==Model matrix
	template <typename MT>
	struct NW_API Mat4x4
	{
		MT data[4][4];

		Mat4x4(MT num = 1.0f) :
			data{ 0 }
		{
			data[0][0] = num;
			data[1][1] = num;
			data[2][2] = num;
			data[3][3] = num;
		}
		Mat4x4(const Mat4x4& rMat) :
			data{ 0 }
		{
			for (USInt x = 0; x < 4; x++)
				for (USInt y = 0; y < 4; y++)
				{
					//data[x][y] = rMat[x][y];
				}
		}

		// Operators
		Mat4x4<MT>& operator=(Mat4x4<MT>& rMat)
		{
			for (USInt x = 0; x < 4; x++)
				for (USInt y = 0; y < 4; y++)
					data[x][y] = rMat[x][y];
			return *this;
		}
		Vec4<MT>& operator*(Vec4<MT>& rVec4)
		{
			Vec4<MT> vec4(0);
			for (int x = 0; x < 4; x++)
				for (int y = 0; y < 4; y++)
					vec4[x] += rVec4[x] * data[x][y];
			return vec4;
		}
		Vec3<MT>& operator*(Vec3<MT>& rVec3)
		{
			Vec3<MT> vec3(0);
			for (int x = 0; x < 3; x++)
				for (int y = 0; y < 3; y++)
					vec3[x] += rVec3[x] * data[x][y];
			MT w = rVec3.x * data[0][3] + data[1][3] + data[2][3] + data[3][3];
			if (w != 0)
				vec3 /= w;
			return vec3;
		}
		Mat4x4<MT>& operator*(Mat4x4<MT>& rMatrix)
		{
			return *this;
		}

		MT* operator[](int index)
		{
			return this->data[index];
		}
		MT* operator[](UInt index)
		{
			return this->data[index];
		}
		MT* operator[](USInt index)
		{
			return this->data[index];
		}
	};

	// Typedefs
	typedef Mat4x4<float> Mat4f;
	typedef Mat4x4<double> mat4d;
#elif (NW_MATH & NW_MATH_GLM)
	typedef glm::mat2 Mat2f;
	typedef glm::mat3 Mat3f;
	typedef glm::mat4 Mat4f;
	typedef glm::dmat2x2 mat2d;
	typedef glm::dmat3x3 mat3d;
	typedef glm::dmat4x4 mat4d;
#endif // NW_MATH
}

#endif // NW_MATH_MATRIX_H