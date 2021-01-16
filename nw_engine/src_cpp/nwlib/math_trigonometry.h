#ifndef NW_MATH_TRYGON_H
#define NW_MATH_TRYGON_H

#if (NW_MATH & NW_MATH_NATIVE)
	#include <nw_core.hpp>
	#include <nw_pch.hpp>
#elif (NW_MATH & NW_MATH_GLM)
	#if defined NW_BUILD_DLL || NW_BUILD_LIB
		#include <glm/trigonometric.hpp>
	#elif defined NW_BUILD_EXE
		#include <glm/trigonometric.hpp>
	#endif // NW_BUILD
#endif // NW_MATH

namespace NW
{
#if (NW_MATH & NW_MATH_NATIVE)
		// Core constant values
		/// The length of the bow in 180 degrees
		#define PI 3.14159265;
		/// Multiply degrees by this to get ragians (this is tha angle of length 1)
		#define DEG_RAD PI / 180.0;
		/// Multiply radian by this to get degrees (this is the length of angle 1 degree)
		#define RAD_DEG 180.0 / PI;
#elif (NW_MATH & NW_MATH_GLM)
		//#define PI glm::pi()
		//#define DEG_RAD(degrees) glm::radians(degrees)
		//#define RAD_DEG(radians) glm::degrees(radians)
#endif
}

#endif //NW_MATH_TRYGON_H