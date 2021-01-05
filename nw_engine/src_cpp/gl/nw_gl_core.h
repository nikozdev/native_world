#ifndef GL_LIB_STRUCTS_H
#define GL_LIB_STRUCTS_H
#include <gl_decl.hpp>

#include <lib/utils/math_vector.h>
#include <lib/utils/math_matrix.h>

#if (defined NW_GRAPHICS)

// Functions
namespace NW
{
	/// 
	inline UInt32 SDType_GetSize(ShaderDataTypes sDataType);
	/// Translate UInt id to the
	template <typename Type>
	inline ShaderDataTypes TypeToSDType()
	{
		switch (typeid(typename))
		{
			case typeid(bool) :
				return NW_BOOL;
				case typeid(int) :
					return NW_INT;
					case typeid(UInt) :
						return NW_UINT;
						case typeid(float) :
							return NW_FLOAT;
		}
	}
}

// BufferLayout
namespace NW
{
	/// BufferElement structure
	/// --Contains all relevant data for shader usage of vertex buffer data
	struct NW_API BufferElement
	{
		ShaderDataTypes sdType;
		UInt32 unCount;
		bool bNormalized;
		UInt32 unOffset;

		BufferElement() = default;
		BufferElement(ShaderDataTypes shaderDataType, UInt32 unCount, bool normalized) :
			sdType(shaderDataType), unCount(unCount),
			bNormalized(normalized), unOffset(0) {}
	};
	/// BufferLayout structure
	/// --Contains buffer elements for a particular vertexBuffer
	struct NW_API BufferLayout
	{
		/// The size of entire attribute
		UInt32 unStride;
		/// Data for separate attributes
		DArray<BufferElement> BufElems;

		BufferLayout() :
			unStride(0) { }
		BufferLayout(const DArray<BufferElement>& bufferElements) :
			unStride(0) {
			SetElements(bufferElements);
		}

		// -- Setters
		inline void SetElements(const DArray<BufferElement>& bufferElements) { BufElems = bufferElements; Update(); }
		inline void AddElement(const BufferElement& bufferElement) { BufElems.push_back(bufferElement); Update(); }

		/// Set offsets of the elements and recalculate the stride
		void Update();
	};
}
// Vertex
namespace NW
{
	/// Vertex for 1D objects
	/// -- XYZ coordinates for vertices
	/// -- RGBA color for vertices
	/// -- X coordinate for texture sampling
	/// -- Index for the texture slot
	struct NW_API VertexBatch1d
	{
		V3f vtxCrd;
		V4f vtxClr;
		float texCrd;
		float nTexSlot;
	};
	/// Vertex for 2D objects
	/// -- XY coordinates for vertices
	/// -- RGBA color for vertices
	/// -- UV coordinates for texture coordinates
	/// -- Index for the texture slot
	struct NW_API VertexBatch2d
	{
		V2f vtxCrd;
		V4f vtxClr;
		V2f texCrd;
		float nTexSlot;
	};
	/// Vertex for 3D objects
	/// -- XYZ coordinates for vertices
	/// -- RGBA color for vertices
	/// -- XY coordinates for texture sampling
	/// -- Index for the texture slot
	struct NW_API VertexBatch3d
	{
		V3f vtxCrd;
		V4f vtxClr;
		V2f texCrd;
		float nTexSlot;
		Mat4f m4Transform;
	};
	/// Vertex for 3D world
	/// -- XYZ coordinates for vertex coordinates
	/// -- UV coordinates for texture coordinates
	/// -- XYZ coordinates for normals
	struct NW_API VertexShape3d
	{
		V3f vtxCrd;
		V2f texCrd;
		V3f normCrd;

		VertexShape3d() :
			vtxCrd{}, texCrd{}, normCrd{} {}
		VertexShape3d(const V3f& vCoord, const V2f& vUV, const V3f& vNorm) :
			vtxCrd(vCoord), texCrd(vUV), normCrd(vNorm) {}
	};
}
// Drawing data
namespace NW
{
	/// DrawObjectData struct
	struct NW_API DrawObjectData
	{
	public:
		ADrawable* pDrawable = nullptr;
		AShader* pShader = nullptr;
		UInt8 unDrawOrder = 0;
		PrimitiveTypes DrawPrimitive = PT_TRIANGLES;
		inline bool operator>(const DrawObjectData& rDOD) { return rDOD.unDrawOrder > unDrawOrder; }
		inline bool operator>=(const DrawObjectData& rDOD) { return rDOD.unDrawOrder >= unDrawOrder; }
		inline bool operator==(const DrawObjectData& rDOD) { return rDOD.unDrawOrder == unDrawOrder; }
		inline bool operator<=(const DrawObjectData& rDOD) { return rDOD.unDrawOrder <= unDrawOrder; }
		inline bool operator<(const DrawObjectData& rDOD) { return rDOD.unDrawOrder < unDrawOrder; }
	};
	/// DrawSceneData struct
	struct NW_API DrawSceneData
	{
	public:
		GCamera* pGCamera = nullptr;
	};
	/// RenderAttributes structure
	/// Description:
	/// -- Check and change renderer attributes for more or less performance/resource usage
	struct NW_API DrawerInfo
	{
	public:	// Interface Attributes
		// Configurations
		Size szVertex = 0;
		UInt32 unMaxVtx = 0;
		Size szMaxVtx = 0;
		UInt32 unMaxInd = 0;
		Size szMaxInd = 0;
		UInt32 unMaxTex = 0;
		// Counters
		UInt32 unVtx = 0;
		Size szVtx = 0;
		UInt32 unInd = 0;
		Size szInd = 0;
		UInt32 unTex = 0;
		// Drawing
		UInt16 unDrawCalls = 0;
	public:	// Interface Methods
		// -- Setters
		inline void SetVertexSize(UInt32 szVertex) {
			this->szVertex = szVertex;
			this->szMaxVtx = this->unMaxVtx * szVertex;
		}
		inline void SetMaxVCount(UInt32 unMaxVtx) {
			this->unMaxVtx = unMaxVtx;
			this->szMaxVtx = this->unMaxVtx * szVertex;
		};
		inline void SetVCount(UInt32 unVtx) {
			this->unVtx = unVtx;
			this->szVtx = this->unVtx * szVertex;
		};
		inline void SetICount(UInt32 unInd) {
			this->unInd = unInd;
			this->szInd = this->unInd * sizeof(UInt32);
		};
		inline void SetMaxICount(UInt32 unMaxInd) {
			this->unMaxInd = unMaxInd;
			this->szMaxInd = this->unMaxInd * sizeof(UInt32);
		};

		inline void Reset() {
			SetVertexSize(sizeof(VertexBatch2d));
			SetVCount(0);
			SetICount(0);
			unTex = 0;
			unDrawCalls = 0;
		}
	};
}
// Pixel
namespace NW
{
	/// Pixel structure
	/// Description:
	/// -- Represents color in rgba for float and unsigned integer formats
	struct NW_API Pixel
	{
	public:
		explicit Pixel(float red = 0, float green = 0, float blue = 0, float alpha = 0) :
			ur(static_cast<UInt8>(red * 255.0f)),
			ug(static_cast<UInt8>(green * 255.0f)),
			ub(static_cast<UInt8>(blue * 255.0f)),
			ua(static_cast<UInt8>(alpha * 255.0f)) {}
		explicit Pixel(Int32 red, Int32 green, Int32 blue, Int32 alpha = 0) :
			ur(static_cast<UInt8>(red)),
			ug(static_cast<UInt8>(green)),
			ub(static_cast<UInt8>(blue)),
			ua(static_cast<UInt8>(alpha)) {}
		explicit Pixel(Int8 rgbaColor) :
			ur(static_cast<UInt8>(rgbaColor)),
			ug(static_cast<UInt8>(rgbaColor)),
			ub(static_cast<UInt8>(rgbaColor)),
			ua(static_cast<UInt8>(rgbaColor)) {};
		explicit Pixel(V4f rgbaColor)
		{
			ur = static_cast<UInt8>(rgbaColor.r * 255.0f);
			ug = static_cast<UInt8>(rgbaColor.g * 255.0f);
			ub = static_cast<UInt8>(rgbaColor.b * 255.0f);
			ua = static_cast<UInt8>(rgbaColor.a * 255.0f);
		}
		Pixel(UInt32 unRgba) { this->unRgba = unRgba; }

		union { UInt32 unRgba; struct { UInt8 ur, ug, ub, ua; }; struct { Int8 r, g, b, a; }; };
	public:
		// -- Convertion to float
		inline const float fR() const { return static_cast<float>(ur) / 255.0f; }
		inline const float fG() const { return static_cast<float>(ug) / 255.0f; }
		inline const float fB() const { return static_cast<float>(ub) / 255.0f; }
		inline const float fA() const { return static_cast<float>(ua) / 255.0f; }
		inline operator V4f() { return V4f{ fR(), fG(), fB(), fA() }; }
		// -- Operations with other pixels
		inline Pixel operator+(const Pixel& rPixel) {
			return Pixel(static_cast<Int8>(r + rPixel.r), static_cast<Int8>(g + rPixel.g),
				static_cast<Int8>(b + rPixel.b), static_cast<Int8>(a + rPixel.a));
		}
		inline Pixel operator-(const Pixel& rPixel) {
			return Pixel(static_cast<Int8>(r - rPixel.r), static_cast<Int8>(g - rPixel.g),
				static_cast<Int8>(b - rPixel.b), static_cast<Int8>(a - rPixel.a));
		}
		inline Pixel operator*(const Pixel& rPixel) {
			return Pixel(static_cast<Int8>(r * rPixel.r), static_cast<Int8>(g * rPixel.g),
				static_cast<Int8>(b * rPixel.b), static_cast<Int8>(a * rPixel.a));
		}
		inline Pixel operator/(const Pixel& rPixel) {
			return Pixel(static_cast<Int8>(r / rPixel.r), static_cast<Int8>(g / rPixel.g),
				static_cast<Int8>(b / rPixel.b), static_cast<Int8>(a / rPixel.a));
		}
		inline Pixel operator+=(const Pixel& rPixel) { r + rPixel.r; g + rPixel.g; b + rPixel.b; a + rPixel.a; return *this; }
		inline Pixel operator-=(const Pixel& rPixel) { r - rPixel.r; g - rPixel.g; b - rPixel.b; a - rPixel.a; return *this; }
		inline Pixel operator*=(const Pixel& rPixel) { r* rPixel.r; g* rPixel.g; b* rPixel.b; a* rPixel.a; return *this; }
		inline Pixel operator/=(const Pixel& rPixel) { r / rPixel.r; g / rPixel.g; b / rPixel.b; a / rPixel.a; return *this; }
		// -- Operations with float vector
		inline Pixel operator+(const V4f& rColor) {
			Pixel rPixel(rColor);
			return Pixel(r + rPixel.r, g + rPixel.g, b + rPixel.b, a + rPixel.a);
		}
		inline Pixel operator-(const V4f& rColor)
		{
			Pixel rPixel(rColor);
			return Pixel(r - rPixel.r, g - rPixel.g, b - rPixel.b, a - rPixel.a);
		}
		inline Pixel operator*(const V4f& rColor)
		{
			Pixel rPixel(rColor);
			return Pixel(r * rPixel.r, g * rPixel.g, b * rPixel.b, a * rPixel.a);
		}
		inline Pixel operator/(const V4f& rColor)
		{
			Pixel rPixel(rColor);
			return Pixel(r / rPixel.r, g / rPixel.g, b / rPixel.b, a / rPixel.a);
		}
		inline Pixel& operator+=(const V4f& rColor)
		{
			r += static_cast<Int8>(rColor.r) * 255.0f;
			g += static_cast<Int8>(rColor.g) * 255.0f;
			b += static_cast<Int8>(rColor.b) * 255.0f;
			a += static_cast<Int8>(rColor.a) * 255.0f;
			return *this;
		}
		inline Pixel& operator-=(const V4f& rColor)
		{
			r -= static_cast<Int8>(rColor.r) * 255.0f;
			g -= static_cast<Int8>(rColor.g) * 255.0f;
			b -= static_cast<Int8>(rColor.b) * 255.0f;
			a -= static_cast<Int8>(rColor.a) * 255.0f;
			return *this;
		}
		inline Pixel& operator*=(const V4f& rColor)
		{
			r *= static_cast<Int8>(rColor.r) * 255.0f;
			g *= static_cast<Int8>(rColor.g) * 255.0f;
			b *= static_cast<Int8>(rColor.b) * 255.0f;
			a *= static_cast<Int8>(rColor.a) * 255.0f;
			return *this;
		}
		inline Pixel& operator/=(const V4f& rColor)
		{
			r /= static_cast<Int8>(rColor.r) * 255.0f;
			g /= static_cast<Int8>(rColor.g) * 255.0f;
			b /= static_cast<Int8>(rColor.b) * 255.0f;
			a /= static_cast<Int8>(rColor.a) * 255.0f;
			return *this;
		}
	};
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
#define GL_DEBUG_ERR_LOG(errType, objectID) (OGL_ErrLog_Shader(errType, objectID))
// Functions
namespace NW
{
	// Debug
	/// Clear GL error buffer
	extern void OGL_ClearErr();
	/// Return suitable error message accordingly to glGetError()
	extern bool OGL_ErrLog(const char* funcName, const char* file, int line);
	/// Get compile and linking status return true if there are errors
	extern int OGL_ErrLog_Shader(ShaderTypes ShaderType, UInt32 unShaderId);
}

#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
namespace NW
{
	/// PixelInteger struct
	/// --Contains 32 bits of memory and 4 partf of it as uChars
	struct NW_API PxInt
	{
		/// Color of the pixel
		union
		{
			UInt32 unColor = 0xFFFFFFFF;
			struct
			{
				unsigned char r, g, b, a;
			};
		};
		PxInt() : unColor(0) {};
		PxInt(UInt32 unRgbaBytes) : unColor(unRgbaBytes) {};
	};
	/// PixelShort struct
	/// --Contains 16 bits of memory and 2 parts of it as uChars
	struct NW_API PxShort
	{
		/// Color of the pixel
		union
		{
			uint16_t color;
			struct
			{
				unsigned char uiBack, uiFace;
			};
		};
		PxShort() : color(0b0000'0000) {};
	};

	/// 0xFF'FF'FF'FF -> 32 bits
	/// --4 bits for Red, 4 bits for Blue and 4 bits for Green
	/// --Additional 4 bits for alplha channel
	enum PxCharTypes : wchar_t
	{
		PX_DITHER = 0x2489,
		PX_1_4 = 0x2591,
		PX_2_4 = 0x2592,
		PX_3_4 = 0x2593,
		PX_SOLID = 0x2588,
	};
	/// 0b0000'0000 -> 8 bits
	/// --Right 4 bits - background -> 16 variations;
	/// --Left 4 bits - foreground -> 16 variations;
	enum USIntColors
	{
		FG_BLACK = 0x0000,
		FG_DARK_BLUE = 0x0001,
		FG_DARK_GREEN = 0x0002,
		FG_DARK_CYAN = 0x0003,
		FG_DARK_RED = 0x0004,
		FG_DARK_MAGENTA = 0x0005,
		FG_DARK_YELLOW = 0x0006,
		FG_GREY = 0x0007,
		FG_DARK_GREY = 0x0008,
		FG_BLUE = 0x0009,
		FG_GREEN = 0x000A,
		FG_CYAN = 0x000B,
		FG_RED = 0x000C,
		FG_MAGENTA = 0x000D,
		FG_YELLOW = 0x000E,
		FG_WHITE = 0x000F,
		BG_BLACK = 0x0000,
		BG_DARK_BLUE = 0x0010,
		BG_DARK_GREEN = 0x0020,
		BG_DARK_CYAN = 0x0030,
		BG_DARK_RED = 0x0040,
		BG_DARK_MAGENTA = 0x0050,
		BG_DARK_YELLOW = 0x0060,
		BG_GREY = 0x0070,
		BG_DARK_GREY = 0x0080,
		BG_BLUE = 0x0090,
		BG_GREEN = 0x00A0,
		BG_CYAN = 0x00B0,
		BG_RED = 0x00C0,
		BG_MAGENTA = 0x00D0,
		BG_YELLOW = 0x00E0,
		BG_WHITE = 0x00F0,
	};
}
#endif // NW_GRAPHICS

#endif // GL_LIB_STRUCTS_H

#ifndef GL_LIB_TYPES_H
#define GL_LIB_TYPES_H

#include <core/nw_data_res.h>

#if (defined NW_GRAPHICS)
// Graphics Objetcs
namespace NW
{
	/// Abstract VertexBuffer Class
	/// --Interface:
	/// -> Create -> If created with data in OpenGL, load dynamically,
	/// else - load statically and don't change the buffer size;
	/// -> Load Data (if it isn't)
	/// -> Give it's pointer to VertexArray EITHER
	/// Bind -> Draw -> Unbind
	class NW_API AVertexBuf
	{
	public:
		virtual ~AVertexBuf() {}

		// -- Getters
		virtual inline Size GetDataSize() const = 0;
		virtual inline BufferLayout& GetLayout() = 0;
		// -- Setters
		virtual inline void SetLayout(const BufferLayout& bLayout) = 0;
		virtual void SetData(Size szAlloc, void* pVtxData, Size szOffset = 0) = 0;
		
		// Interface methods
		virtual inline void Bind() const = 0;
		virtual inline void Unbind() const = 0;

		static AVertexBuf* Create(Size szAlloc, void* pVtxData = nullptr);
	};
	/// Abstract IndexBuffer Class
	/// --Interface:
	/// -> Create -> If created with data in OpenGL, load dynamically,
	/// else - load statically and don't change the buffer size;
	/// -> Load Data (if it isn't)
	/// -> Give it's pointer to VertexArray EITHER
	/// Bind -> Bind VertexBuffer -> Draw -> Unbind both Buffers
	class NW_API AIndexBuf
	{
	public:
		virtual ~AIndexBuf() {};

		// -- Getters
		virtual inline UInt32 GetIndCount() const = 0;
		// -- Setters
		virtual void SetData(UInt32 unCount, UInt32* pIndData, Size szOffset = 0) = 0;

		/// -- Interface Methods
		virtual void Bind() const = 0;
		virtual inline void Unbind() const = 0;
		
		static AIndexBuf* Create(UInt32 unCount, UInt32* pIndData = nullptr);
	};
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
namespace NW
{
	/// VertexBufOgl
	/// --Wrapping for VertexBufOglferObject of OpenGL
	class NW_API VertexBufOgl : public AVertexBuf
	{
	public:
		/// Without the given data it will be loaded dynamically
		VertexBufOgl(Size szAlloc);
		/// With the given data it will be loaded statically
		VertexBufOgl(Size szAlloc, void* pVtxData);
		~VertexBufOgl();

		// -- Getters
		virtual inline Size GetDataSize() const override { return m_szData; }
		virtual inline BufferLayout& GetLayout() override { return m_bufLayout; }
		// -- Setters
		virtual void SetData(Size szAlloc, void* pVtxData, Size szOffset = 0) override;
		virtual inline void SetLayout(const BufferLayout& rBufLayout) override { m_bufLayout = rBufLayout; }
		
		// -- Interface methods
		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		UInt32 m_unRIdVB;
		UInt32 m_unRIdVA;
		Size m_szData;

		BufferLayout m_bufLayout;
	};
	/// IndexBufOgl class
	/// --Wrapping for ElementVertexObject of OpenGL
	class NW_API IndexBufOgl : public AIndexBuf
	{
	public:
		/// Without the given data it will be loaded dynamically
		IndexBufOgl(UInt32 unCount);
		/// With the given data it will be loaded statically
		IndexBufOgl(UInt32 unCount, UInt32* pIndData);
		~IndexBufOgl();

		// Getters
		virtual inline UInt32 GetIndCount() const override { return m_unIndCount; }
		// Setters

		// Interface methods
		virtual void SetData(UInt32 unIndCount, UInt32* pIndData, Size szOffset = 0) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		UInt32 m_unRId;
		UInt32 m_unIndCount;
	};
}
#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
namespace NW
{
	/// VertexBufCout
	/// Description:
	/// -- Stores all the vertex data we need for drawing
	/// -- Takes raw pointer and size of the data and loads it into own binary "storage".
	/// -- This can be used by GraphicsApiCout or can be pointed by CoutVertexArray
	/// -- CoutVertexArray uses buffer layout to define what the buffer data actually is
	class NW_API VertexBufCout : public AVertexBuf
	{
	public:
		/// Without the given data it will be loaded dynamically
		VertexBufCout(UInt32 uiSizeInBytes);
		/// With the given data it will be loaded statically
		VertexBufCout(UInt32 uiSizeInBytes, void* vDataPtr);
		~VertexBufCout();

		// Getters
		virtual inline UInt32 GetSize() const override
		{
			return m_daVData.capacity() * sizeof(char);
		}
		virtual inline BufferLayout& GetLayout() override
		{
			return m_bufLayout;
		}
		inline const DArray<char>& GetVData() const
		{
			return m_daVData;
		}
		// Setters
		virtual inline void SetLayout(const BufferLayout& bLayout) override
		{
			m_bufLayout = bLayout;
		}
		// Interface methods
		virtual void SetData(UInt32 unSizeInBytes, void* pVData, UInt32 unOffsetSize = 0) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		BufferLayout m_bufLayout;
		DArray<char> m_daVData;
	};
	/// IndexBufCout class
	class NW_API IndexBufCout : public AIndexBuf
	{
	public:
		/// Without the given data it will be loaded dynamically
		IndexBufCout(UInt32 unCount);
		/// With the given data it will be loaded statically
		IndexBufCout(UInt32 unCount, UInt32* pIndices);
		~IndexBufCout();

		// Getters
		virtual inline UInt32 GetCount() const override { return m_unIData.capacity(); }
		inline const DArray<UInt32>& GetIndices() const { return m_unIData; }
		// Setters

		// Interface methods
		virtual void SetData(UInt32 unCount, UInt32* punIndices, UInt32 unOffsetSize = 0) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		DArray<UInt32> m_unIData;
	};
}
#endif // NW_GRAPHICS

#endif // GL_LIB_TYPES_H