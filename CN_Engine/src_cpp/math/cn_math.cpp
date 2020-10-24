#include <cn_pch.hpp>
#include <cn_core.hpp>
#include <math/cn_math.h>
#include <cmath>

namespace CN
{
	// Vectors
	namespace MAT
	{
		// Vec2
		void Vec2::normalize()
		{
			float len = length();
			x /= len;
			y /= len;
		}
		float Vec2::length()
		{
			return sqrt(x * x + y * y);
		}
		float Vec2::angleDeg(Vec2& vec)
		{
			return 0.0f;
		}
		float Vec2::angleRad(Vec2& vec)
		{
			return 0.0f;
		}
		float Vec2::operator[](unsigned int index)
		{
			switch (index)
			{
			case 0: return this->x;
			case 1: return this->y;
			default:
				CN_ASSERT(false, "We can not index the Vec2 with this number.");
				return 0.0f;
			}
		}

		// Vec3
		void Vec3::normalize()
		{
			float len = length();
			x /= len;
			y /= len;
			z /= len;
		}
		float Vec3::length()
		{
			return sqrt(x * x + y * y + z * z);
		}
		float Vec3::angleDeg(Vec3& vec)
		{
			return 0.0f;
		}
		float Vec3::angleRad(Vec3& vec)
		{
			return 0.0f;
		}
		float Vec3::operator[](unsigned int index)
		{
			switch (index)
			{
			case 0: return this->x;
			case 1: return this->y;
			case 2: return this->z;
			default:
				CN_ASSERT(false, "We can not index the Vec3 with this number.");
				return 0.0f;
			}
		}

		// Vec4
		void Vec4::normalize()
		{
			float len = length();
			x /= len;
			y /= len;
			z /= len;
		}

		float Vec4::length()
		{
			return sqrt(x * x + y * y + z * z + w * w);
		}

		float Vec4::angleDeg(Vec3& vec)
		{
			return 0.0f;
		}

		float Vec4::angleRad(Vec3& vec)
		{
			return 0.0f;
		}
		float Vec4::operator[](unsigned int index)
		{
			switch (index)
			{
			case 0: return this->x;
			case 1: return this->y;
			case 2: return this->z;
			case 3: return this->w;
			default:
				CN_ASSERT(false, "We can not index the Vec4 with this number.");
				return 0.0f;
			}
		}
	}
	// Math functions
	namespace MAT
	{
		// Absolute number
		void abs(int& num)
		{
			if (num < 0) num = -num;
		}
		void abs(float& num)
		{
			if (num < 0.0) num = -num;
		}
		void abs(double& num)
		{
			if (num < 0.0) num = -num;
		}
		int abs(int num)
		{
			if (num < 0) num = -num;
			return num;
		}
		float abs(float num)
		{
			if (num < 0.0) num = -num;
			return num;
		}
		double abs(double num)
		{
			if (num < 0.0) num = -num;
			return num;
		}

		// Approximate equality
		bool approxEqual(int num1, int num2, int accuracy)
		{
			if (abs(num1 - num2) < accuracy)
				return true;
			return false;
		}
		bool approxEqual(float num1, float num2, float accuracy)
		{
			if (abs(num1 - num2) < accuracy)
				return true;
			return false;
		}
		bool approxEqual(double num1, double num2, double accuracy)
		{
			if (abs(num1 - num2) < accuracy)
				return true;
			return false;
		}

		// Clamp function
		void setInterval(int& num, int min, int max)
		{
			if (num < min) num = min;
			else if (num > max) num = max;
		}
		void setInterval(float& num, float min, float max)
		{
			if (num < min) num = min;
			else if (num > max) num = max;
		}
		void setInterval(double& num, double min, double max)
		{
			if (num < min) num = min;
			else if (num > max) num = max;
		}

		// Degrees-radians transition
		float getRadian(float degrees)
		{
			return degrees * (float)DEG_RAD;
		}
		double getRadian(double degrees)
		{
			return degrees * DEG_RAD;
		}
		float getDegree(float radians)
		{
			return (float)RAD_DEG * radians;
		}
		double getDegree(double radians)
		{
			return RAD_DEG * radians;
		}

		// Percent of an interval
		float getLinearInter(float min, float max, float percent)
		{
			return (min + ((max - min) * percent));
		}

		// Spline points
		float getBezierSpline(float time, float Pt0, float Pt1, float Pt2)
		{
			setInterval(time, 0.0f, 1.0f);
			return ((1.0f - time)* (1.0f - time) * Pt0) +
				(2.0f * time * (1.0f - time) * Pt1) +
				(time * time * Pt2);
		}

		// Cartesian_Isometric convertion
		Vec2 cart_iso(float cartX, float cartY)
		{
			Vec2 isoCoord;
			isoCoord.x = cartX - cartY;
			isoCoord.y = (cartX + cartY) / 2;
			return isoCoord;
		}
		Vec2 iso_cart(float isoX, float isoY)
		{
			Vec2 cartCoord;
			cartCoord.x = (2 * isoY - isoX) / 2;
			cartCoord.y = (2 * isoY + isoX) / 2;
			return cartCoord;
		}

		// Power function
		void pow(int& num, UInt power)
		{
			int iPower = static_cast<int>(power);

			if (iPower > 0)
			{
				for (int i = 0; i < iPower - 1; i++)
					num = num * num;
			}
			else if (iPower == 0) num = 1;
		}
		int pow(int num, UInt power)
		{
			float result = 1.0f;
			int iPower = static_cast<int>(power);

			if (iPower > 0)
			{
				for (int i = 0; i < iPower - 1; i++)
					num = num * num;
			}
			else if (num == 0) result = 0.0f;

			return (int)result;
		}
		float pow(float num, double power)
		{
			float result = 1.0f;
			int iPower = static_cast<int>(power);

			if (iPower > 0)
			{
				for (int i = 0; i < iPower; i++)
					result = result * num;
			}
			else if (iPower != 0)
			{// Negative power
				if (num != 0)
					for (int i = 0; i < -iPower; i++)
						result = result / num;
				else
					CN_ASSERT(num != 0, "We can not set 0 in any power");
			}
			return result;
		}
		double pow(double num, double power)
		{
			double result = 1.0;
			int iPower = static_cast<int>(power);

			if (iPower > 0)
			{
				for (int i = 0; i < iPower; i++)
					result = result * num;
			}
			else if (iPower != 0)
			{// Negative power
				if (num != 0)
					for (int i = 0; i < -iPower; i++)
						result = result / num;
				else
					CN_ASSERT(num != 0, "We can not set 0 in any power");
			}
			return result;
		}

		// Get changed vector
		Vec2 normalVec(Vec2& vec)
		{
			float len = vec.length();
			return Vec2(vec.x / len, vec.y / len);
		}
		Vec3 normalVec(Vec3& vec)
		{
			float len = vec.length();
			return Vec3(vec.x / len, vec.y / len, vec.z / len);
		}
	}
}