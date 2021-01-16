#ifndef NW_MATH_VECTOR_H
#define NW_MATH_VECTOR_H

#if (NW_MATH & NW_MATH_NATIVE)
	#include <nw_core.hpp>
	#include <nw_pch.hpp>
#elif (NW_MATH & NW_MATH_GLM)
	#if defined NW_BUILD_DLL || NW_BUILD_LIB
		#include <glm/vec2.hpp>
		#include <glm/vec3.hpp>
		#include <glm/vec4.hpp>
	#elif defined NW_BUILD_EXE
		#include <glm/vec2.hpp>
		#include <glm/vec3.hpp>
		#include <glm/vec4.hpp>
	#endif // NW_BUILD
#endif // NW_MATH

namespace NW
{
#if (NW_MATH & NW_MATH_NATIVE)
	/// CheerNik 2-vector implementation
	/// --Can be used for:
	/// -->Point coordinates on cartesian system or iso projection
	/// -->Maintains standard operators
	template <typename VT>
	struct NW_API Vec2
	{
		union
		{
			VT x;
			VT r;
		};
		union
		{
			VT y;
			VT g;
		};

		Vec2(VT X, VT Y) :
			x(X), y(Y) {}
		Vec2(VT num = 0.0f) :
			x(num), y(num) {}
		Vec2(const Vec2<VT>& rVec) : x(rVec.x), y(rVec.y) {}

		// Normalization
		void normalize()
		{
			VT len = length();
			this->x /= len;
			this->y /= len;
		}
		VT length()
		{
			return sqrt(x * x + y * y + z * z);
		}
		VT dot(const Vec2& rVec) { return rVec.length() * length() * apxCosDeg(angleDeg(rVec)); }
		VT angleDeg(const Vec2<VT>& rVec)
		{
			return VT();
		}
		VT angleRad(const Vec2<VT>& rVec)
		{
			return VT();
		}

		// Scalar operations
		Vec2 operator+ (const VT& scalar) { return Vec2(this->x + scalar; this->y + scalar); }
		Vec2 operator- (const VT& scalar) { return Vec2(this->x - scalar; this->y - scalar); }
		Vec2 operator* (const VT& scalar) { return Vec2(this->x * scalar; this->y * scalar); }
		Vec2 operator/ (const VT& scalar) { return Vec2(this->x / scalar; this->y / scalar); }
		Vec2& operator+= (const VT& scalar) { this->x += scalar; this->y += scalar; return *this }
		Vec2& operator-= (const VT& scalar) { this->x -= scalar; this->y -= scalar; return *this; }
		Vec2& operator*= (const VT& scalar) { this->x *= scalar; this->y *= scalar; return *this; }
		Vec2& operator/= (const VT& scalar) { this->x /= scalar; this->y /= scalar; return *this; }

		// Vector2 operations
		Vec2 operator+ (const Vec2& rVec) { return Vec2(this->x + rVec.x; this->y + rVec.y); }
		Vec2 operator- (const Vec2& rVec) { return Vec2(this->x - rVec.x; this->y - rVec.y); }
		Vec2 operator* (const Vec2& rVec) { return Vec2(this->x * rVec.x; this->y * rVec.y); }
		Vec2 operator/ (const Vec2& rVec) { return Vec2(this->x / rVec.x; this->y / rVec.y); }
		Vec2& operator+= (const Vec2& rVec) { this->x += rVec.x; this->y += rVec.y; return *this }
		Vec2& operator-= (const Vec2& rVec) { this->x -= rVec.x; this->y -= rVec.y; return *this; }
		Vec2& operator*= (const Vec2& rVec) { this->x *= rVec.x; this->y *= rVec.y; return *this; }
		Vec2& operator/= (const Vec2& rVec) { this->x /= rVec.x; this->y /= rVec.y; return *this; }
		// Unary operators
		Vec2<VT> operator+() const { return { +x, +y }; }
		Vec2<VT> operator-() const { return { -x, -y }; }
		// Indexing
		VT& operator[] (int index)
		{
			switch (index)
			{
			case 0:
				return this->x;
			case 1:
				return this->y;
			default:
				NW_ERR("Inaccessible index value")
					return this->x;
			}
		}
		VT& operator[] (UInt index)
		{
			switch (index)
			{
			case 0:
				return this->x;
			case 1:
				return this->y;
			default:
				NW_ERR("Inaccessible index value")
					return this->x;
			}
		}
		VT& operator[] (USInt index)
		{
			switch (index)
			{
			case 0:
				return this->x;
			case 1:
				return this->y;
			default:
				NW_ERR("Inaccessible index value")
					return this->x;
			}
		}

		//Predicates
		/*bool operator==(const Vec2<VT>& rVec)
		{
			return false;
		}
		bool operator<(const Vec2<VT>& rVec)
		{
			return false;
		}
		bool operator>(const Vec2<VT>& rVec)
		{
			return false;
		}
		bool operator<=(const Vec2<VT>& rVec)
		{
			return false;
		}
		bool operator>=(const Vec2<VT>& rVec)
		{
			return false;
		}*/
		bool operator == (const Vec2& rhs) const { return (this->x == rhs.x && this->y == rhs.y); }
		bool operator != (const Vec2& rhs) const { return (this->x != rhs.x || this->y != rhs.y); }
		const std::string str() const { return std::string("(") + std::to_string(this->x) + "," + std::to_string(this->y) + ")"; }
		friend std::ostream& operator << (std::ostream& os, const Vec2& rhs) { os << rhs.str(); return os; }
		operator Vec2<int32_t>() const { return { static_cast<int32_t>(this->x), static_cast<int32_t>(this->y) }; }
		operator Vec2<float>() const { return { static_cast<float>(this->x), static_cast<float>(this->y) }; }
		operator Vec2<double>() const { return { static_cast<double>(this->x), static_cast<double>(this->y) }; }

		// ===Casting Methods===
		// ===/Casting Methods===
	};
	/// CheerNik 3-vector implementation
	/// --Can be used for:
	/// -->RGB color representation
	/// -->Three dementional positions
	template <typename VT>
	struct NW_API Vec3 : public Vec2<VT>
	{
		/*union
		{
			VT x;
			VT r;
		};
		union
		{
			VT y;
			VT g;
		};*/
		union
		{
			VT z;
			VT b;
		};

		Vec3(VT X, VT Y, VT Z) :
			Vec2<VT>(X, Y), z(Z) {}
		Vec3(VT num = 0.0f) :
			Vec2<VT>(num), z(num) {}
		Vec3(const Vec3<VT>& rVec) :
			Vec2<VT>(rVec.x, rVec.y),
			z(rVec.z) {}

		// Normalization
		void normalize()
		{
			//
		}
		VT length()
		{
			return VT();
		}
		VT angleDeg(const Vec3<VT>& rVec)
		{
			return VT();
		}
		VT angleRad(const Vec3<VT>& rVec)
		{
			return VT();
		}
		Vec3<VT> cross(const Vec3<VT>& rVec)
		{
			return{
				y * rVec.z - rVec.y * z,
				z * rVec.x - rVec.z * x,
				x * rVec.y - rVec.y * x };
		}

		// Scalar operations
		void operator+ (VT& scalar)
		{
			this->x += scalar;
			this->y += scalar;
			this->z += scalar;
		}
		void operator- (VT& scalar)
		{
			this->x -= scalar;
			this->y -= scalar;
			this->z -= scalar;
		}
		void operator* (VT& scalar)
		{
			this->x *= scalar;
			this->y *= scalar;
			this->z *= scalar;
		}
		void operator/ (VT& scalar)
		{
			this->x /= scalar;
			this->y /= scalar;
			this->z /= scalar;
		}

		// Vector3 operations
		void operator+ (Vec3<VT>& rVec)
		{
			this->x += rVec.x;
			this->y += rVec.y;
			this->z += rVec.z;
		}
		void operator- (Vec3<VT>& rVec)
		{
			this->x -= rVec.x;
			this->y -= rVec.y;
			this->z -= rVec.z;
		}
		void operator* (Vec3<VT>& rVec)
		{
			this->x *= rVec.x;
			this->y *= rVec.y;
			this->z *= rVec.z;
		}
		void operator/ (Vec3<VT>& rVec)
		{
			this->x /= rVec.x;
			this->y /= rVec.y;
			this->z /= rVec.z;
		}

		// Indexing
		VT& operator[] (int index)
		{
			switch (index)
			{
			case 0:
				return this->x;
			case 1:
				return this->y;
			case 2:
				return this->z;
			default:
				NW_ERR("Inaccessible Index");
				return this->x;
			}
		}

		//Predicates
		bool operator==(const Vec3<VT>& rVec)
		{
			return false;
		}
		bool operator<(const Vec3<VT>& rVec)
		{
			return false;
		}
		bool operator>(const Vec3<VT>& rVec)
		{
			return false;
		}
		bool operator<=(const Vec3<VT>& rVec)
		{
			return false;
		}
		bool operator>=(const Vec3<VT>& rVec)
		{
			return false;
		}
	};
	/// CheerNik 4-vector implementation
	/// --Can be used for:
	/// -->RGBA color representation
	/// -->Four dementional positions (or 3D with homogenous axis)
	template <typename VT>
	struct NW_API Vec4 : public Vec3<VT>
	{
		/*union
		{
			VT x;
			VT r;
		};
		union
		{
			VT y;
			VT g;
		};
		union
		{
			VT z;
			VT b;
		};*/
		union
		{
			VT w;
			VT a;
		};

		Vec4(VT X, VT Y, VT Z, VT W) :
			Vec3<VT>(X, Y, Z), w(W) {}
		Vec4(float num = 0.0f) :
			Vec3<VT>(num), w(num) {}
		Vec4(const Vec4<VT>& rVec) :
			Vec3<VT>(rVec.x, rVec.y, rVec.z),
			w(rVec.w) {}

		// Normalization
		void normalize()
		{
			//
		}
		VT length()
		{
			return VT();
		}
		VT angleDeg(Vec4<VT>& rVec)
		{
			return VT();
		}
		VT angleRad(Vec4<VT>& rVec)
		{
			return VT();
		}

		// Scalar operations
		void operator+ (VT& scalar)
		{
			this->x += scalar;
			this->y += scalar;
			this->z += scalar;
			this->w += scalar;
		}
		void operator- (VT& scalar)
		{
			this->x -= scalar;
			this->y -= scalar;
			this->z -= scalar;
			this->w -= scalar;
		}
		void operator* (VT& scalar)
		{
			this->x *= scalar;
			this->y *= scalar;
			this->z *= scalar;
			this->w *= scalar;
		}
		void operator/ (VT& scalar)
		{
			this->x /= scalar;
			this->y /= scalar;
			this->z /= scalar;
			this->w /= scalar;
		}

		// Vector4 operations
		void operator+ (Vec4<VT>& rVec)
		{
			this->x += rVec.x;
			this->y += rVec.y;
			this->z += rVec.z;
		}
		void operator- (Vec4<VT>& rVec)
		{
			this->x -= rVec.x;
			this->y -= rVec.y;
			this->z -= rVec.z;
			this->w -= rVec.w;
		}
		void operator* (Vec4<VT>& rVec)
		{
			this->x *= rVec.x;
			this->y *= rVec.y;
			this->z *= rVec.z;
			this->w *= rVec.w;
		}
		void operator/ (Vec4<VT>& rVec)
		{
			this->x /= rVec.x;
			this->y /= rVec.y;
			this->z /= rVec.z;
			this->w /= rVec.w;
		}

		// Indexing
		VT& operator[] (int index)
		{
			switch (index)
			{
			case 0:
				return this->x;
			case 1:
				return this->y;
			case 2:
				return this->z;
			case 3:
				return this->w;
			default:
				NW_ERR("Inaccessible index");
				return this->x;
			}
		}

		//Predicates
		bool operator==(const Vec4<VT>& rVec)
		{
			return false;
		}
		bool operator<(const Vec4<VT>& rVec)
		{
			return false;
		}
		bool operator>(const Vec4<VT>& rVec)
		{
			return false;
		}
		bool operator<=(const Vec4<VT>& rVec)
		{
			return false;
		}
		bool operator>=(const Vec4<VT>& rVec)
		{
			return false;
		}
	};
	// Typedefs
	typedef Vec2<int> v2i;
	typedef Vec3<int> v3i;
	typedef Vec4<int> v4i;

	typedef Vec2<unsigned int> v2ui;
	typedef Vec3<unsigned int> v3ui;
	typedef Vec4<unsigned int> v4ui;

	typedef Vec2<short> v2si;
	typedef Vec3<short> v3si;
	typedef Vec4<short> v4si;

	typedef Vec2<float> V2f;
	typedef Vec3<float> V3f;
	typedef Vec4<float> V4f;

	typedef Vec2<double> v2d;
	typedef Vec3<double> v3d;
	typedef Vec4<double> v4d;
#elif (NW_MATH & NW_MATH_GLM)
	typedef glm::bvec2 V2b;
	typedef glm::bvec3 V3b;
	typedef glm::bvec4 V4b;
	typedef glm::i16vec2 V2si;
	typedef glm::i16vec3 V3si;
	typedef glm::i16vec4 V4si;
	typedef glm::i32vec2 V2i;
	typedef glm::i32vec3 V3i;
	typedef glm::i32vec4 V4i;
	typedef glm::i64vec2 V2li;
	typedef glm::i64vec3 V3li;
	typedef glm::i64vec4 V4li;
	typedef glm::u16vec2 V2usi;
	typedef glm::u16vec3 V3usi;
	typedef glm::u16vec4 V4usi;
	typedef glm::u32vec2 V2ui;
	typedef glm::u32vec3 V3ui;
	typedef glm::u32vec4 V4ui;
	typedef glm::u64vec2 V2uli;
	typedef glm::u64vec3 V3uli;
	typedef glm::u64vec4 V4uli;
	typedef glm::vec2 V2f;
	typedef glm::vec3 V3f;
	typedef glm::vec4 V4f;
	typedef glm::dvec2 V2d;
	typedef glm::dvec3 V3d;
	typedef glm::dvec4 V4d;
#endif
}

#endif // NW_MATH_VECTOR_H