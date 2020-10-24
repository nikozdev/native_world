#ifndef CN_MATH_H
#define CN_MATH_H

namespace CN
{
	// Constants
	namespace MAT
	{
		// Core constant values
		/// The length of the bow in 180 degrees
		constexpr double PI = 3.14159265;
		/// Multiply degrees by this to get ragians (this is tha angle of length 1)
		constexpr double DEG_RAD = 180 / PI;
		/// Multiply radian by this to get degrees (this is the length of angle 1 degree)
		constexpr double RAD_DEG = PI / 180;
	}
	// Structs
	namespace MAT
	{
		/// Mathematical vector interface
		template <int dimentions>
		struct MathVec
		{
			MathVec() {};
			virtual ~MathVec() {};
		};
		/// CheerNik 2-float-vector implementation
		/// --Can be used for:
		/// -->Point coordinates on cartesian system or iso projection
		/// -->Maintains standard operators
		struct Vec2
		{
			union
			{
				float x;
				float r;
			};
			union
			{
				float y;
				float g;
			};

			Vec2(float X, float Y) :
				x(X), y(Y) {}
			Vec2(float num = 0.0f) :
				x(num), y(num) {}
			Vec2(const Vec2& rVec) : x(rVec.x), y(rVec.y) {}

			// Normalization
			void normalize();
			float length();
			float angleDeg(Vec2& vec);
			float angleRad(Vec2& vec);

			// Scalar operations
			void operator+ (float& scalar)
			{
				this->x += scalar;
				this->y += scalar;
			}
			void operator- (float& scalar)
			{
				this->x -= scalar;
				this->y -= scalar;
			}
			void operator* (float& scalar)
			{
				this->x *= scalar;
				this->y *= scalar;
			}
			void operator/ (float& scalar)
			{
				this->x /= scalar;
				this->y /= scalar;
			}

			// Vector2 operations
			void operator+ (Vec2& rVec)
			{
				this->x += rVec.x;
				this->y += rVec.y;
			}
			void operator- (Vec2& rVec)
			{
				this->x -= rVec.x;
				this->y -= rVec.y;
			}
			void operator* (Vec2& rVec)
			{
				this->x *= rVec.x;
				this->y *= rVec.y;
			}
			void operator/ (Vec2& rVec)
			{
				this->x /= rVec.x;
				this->y /= rVec.y;
			}

			// Indexing
			float operator[] (unsigned int index);
		};
		/// CheerNik 3-float-vector implementation
		/// --Can be used for:
		/// -->Point coordinates in 3D space
		/// -->Color RGB/HSL representation
		/// --Maintains standard operators (+- */) for other CN::vectors and scalars
		struct Vec3
		{
			union
			{
				float x;
				float r;
			};
			union
			{
				float y;
				float g;
			};
			union
			{
				float z;
				float b;
			};

			Vec3(float X, float Y, float Z) :
				x(X), y(Y), z(Z) {}
			Vec3(float num = 0.0f) :
				x(num), y(num), z(num) {}
			Vec3(const Vec3& rVec) : x(rVec.x), y(rVec.y), z(rVec.z) {}

			// Normalization
			void normalize();
			float length();
			float angleDeg(Vec3& vec);
			float angleRad(Vec3& vec);

			// Scalar operations
			void operator+ (float& scalar)
			{
				this->x += scalar;
				this->y += scalar;
			}
			void operator- (float& scalar)
			{
				this->x -= scalar;
				this->y -= scalar;
			}
			void operator* (float& scalar)
			{
				this->x *= scalar;
				this->y *= scalar;
			}
			void operator/ (float& scalar)
			{
				this->x /= scalar;
				this->y /= scalar;
			}

			// Vector2 operations
			void operator+ (Vec2& rVec)
			{
				this->x += rVec.x;
				this->y += rVec.y;
			}
			void operator- (Vec2& rVec)
			{
				this->x -= rVec.x;
				this->y -= rVec.y;
			}
			void operator* (Vec2& rVec)
			{
				this->x *= rVec.x;
				this->y *= rVec.y;
			}
			void operator/ (Vec2& rVec)
			{
				this->x /= rVec.x;
				this->y /= rVec.y;
			}

			// Vector3 operations
			void operator+ (Vec3& rVec)
			{
				this->x += rVec.x;
				this->y += rVec.y;
				this->z += rVec.z;
			}
			void operator- (Vec3& rVec)
			{
				this->x -= rVec.x;
				this->y -= rVec.y;
				this->z -= rVec.z;
			}
			void operator* (Vec3& rVec)
			{
				this->x *= rVec.x;
				this->y *= rVec.y;
				this->z *= rVec.z;
			}
			void operator/ (Vec3& rVec)
			{
				this->x /= rVec.x;
				this->y /= rVec.y;
				this->z /= rVec.z;
			}

			// Indexing
			float operator[] (unsigned int index);
		};
		/// CheerNik 4x4-float-matrix implementation
		/// --Can be used for:
		/// -->Transformation matricies
		/// ||==ProjectionMatrix
		/// ||==ViewMatrix
		/// ||==ModelMatrix
		struct Vec4
		{
			union
			{
				float x;
				float r;
			};
			union
			{
				float y;
				float g;
			};
			union
			{
				float z;
				float b;
			};
			union
			{
				float z;
				float b;
			};
			union
			{
				float w;
				float a;
			};

			Vec4(float X, float Y, float Z, float W) :
				x(X), y(Y), z(Z), w(W) {}
			Vec4(float num = 0.0f) :
				x(num), y(num), z(num), w(num) {}
			Vec4(const Vec4& rVec) : x(rVec.x), y(rVec.y), z(rVec.z), w(rVec.w) {}

			// Normalization
			void normalize();
			float length();
			float angleDeg(Vec3& vec);
			float angleRad(Vec3& vec);

			// Scalar operations
			void operator+ (float& scalar)
			{
				this->x += scalar;
				this->y += scalar;
			}
			void operator- (float& scalar)
			{
				this->x -= scalar;
				this->y -= scalar;
			}
			void operator* (float& scalar)
			{
				this->x *= scalar;
				this->y *= scalar;
			}
			void operator/ (float& scalar)
			{
				this->x /= scalar;
				this->y /= scalar;
			}

			// Vector2 operations
			void operator+ (Vec2& rVec)
			{
				this->x += rVec.x;
				this->y += rVec.y;
			}
			void operator- (Vec2& rVec)
			{
				this->x -= rVec.x;
				this->y -= rVec.y;
			}
			void operator* (Vec2& rVec)
			{
				this->x *= rVec.x;
				this->y *= rVec.y;
			}
			void operator/ (Vec2& rVec)
			{
				this->x /= rVec.x;
				this->y /= rVec.y;
			}

			// Vector3 operations
			void operator+ (Vec3& rVec)
			{
				this->x += rVec.x;
				this->y += rVec.y;
				this->z += rVec.z;
			}
			void operator- (Vec3& rVec)
			{
				this->x -= rVec.x;
				this->y -= rVec.y;
				this->z -= rVec.z;
			}
			void operator* (Vec3& rVec)
			{
				this->x *= rVec.x;
				this->y *= rVec.y;
				this->z *= rVec.z;
			}
			void operator/ (Vec3& rVec)
			{
				this->x /= rVec.x;
				this->y /= rVec.y;
				this->z /= rVec.z;
			}

			// Vector4 operations
			void operator+ (Vec4& rVec)
			{
				this->x += rVec.x;
				this->y += rVec.y;
				this->z += rVec.z;
			}
			void operator- (Vec4& rVec)
			{
				this->x -= rVec.x;
				this->y -= rVec.y;
				this->z -= rVec.z;
			}
			void operator* (Vec4& rVec)
			{
				this->x *= rVec.x;
				this->y *= rVec.y;
				this->z *= rVec.z;
			}
			void operator/ (Vec4& rVec)
			{
				this->x /= rVec.x;
				this->y /= rVec.y;
				this->z /= rVec.z;
			}

			// Indexing
			float operator[] (unsigned int index);
		};
		/// CheerNik 4x4-float-matrix implementation
		/// --Can be used for:
		/// -->Transformation matricies
		/// ||==ProjectionMatrix
		/// ||==ViewMatrix
		/// ||==ModelMatrix

		struct Mat4x4
		{
			Mat4x4& operator*(Vec4 vec4);
		};
	}
	// Functions
	namespace MAT
	{
		/// Absolute integer
		void abs(int& num);
		/// Absolute float
		void abs(float& num);
		/// Absolute double
		void abs(double& num);
		/// Return absolute integer
		int abs(int num);
		/// Return absolute float
		float abs(float num);
		/// Return absolute double
		double abs(double num);

		/// Return true if the difference between given integers is lesser than accuracy
		bool approxEqual(int num1, int num2, int accuracy);
		/// Return true if the difference between given floats is lesser than accuracy
		bool approxEqual(float num1, float num2, float accuracy);
		/// Return true if the difference between given doubles is lesser than accuracy
		bool approxEqual(double num1, double num2, double accuracy);

		/// Set given integer between min and max
		void setInterval(int& num, int min, int max);
		/// Set given float between min and max
		void setInterval(float& num, float min, float max);
		/// Set given double between min and max
		void setInterval(double& num, double min, double max);

		/// Translate degrees to radians (float)
		float getRadian(float degrees);
		/// Translate degrees to radians (double)
		double getRadian(double degrees);
		/// Translate radians to degrees (float)
		float getDegree(float radians);
		/// Translate radians to degrees(double)
		double getDegree(double radians);

		/// Get the float equaled the percent of the given interval
		float getLinearInter(float min, float max, float percent);

		/// 
		float getBezierSpline(float time, float start, float destination, float average);

		/// Convert cartesian coordinates into isometric
		/// (rotate 45 and squash in Y)
		Vec2 cart_iso(float cartX, float cartY);
		/// Convert isometric coordinates into cartesian
		Vec2 iso_cart(float isoX, float isoY);

		/// Just power function for integer: (multiply by "number") "power times"
		void pow(int& num, UInt power);
		/// Just power function for float: (multiply by "number") "power times"
		void pow(float& num, float power);
		/// Just power function for double: (multiply by "number") "power times"
		void pow(double& num, double power);
		/// Just power function for integer: (multiply by "number") "power times"
		int pow(int num, UInt power);
		/// Just power function for float: (multiply by "number") "power times"
		float pow(float num, float power);
		/// Just power function for double: (multiply by "number") "power times"
		double pow(double num, double power);

		/// Normalize vector 2 - set vector's x and y between 0.0 and 1.0
		Vec2 normalVec(Vec2& vec);
		/// Normalize vector 3 - set vector's x, y and z between 0.0 and 1.0
		Vec3 normalVec(Vec3& vec);

		// Matricies
		Mat4x4& translateMat();
	}
}

#endif