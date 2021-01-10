#ifndef NW_ADRAWABLE_H
#define NW_ADRAWABLE_H

#include <glib/render/nw_texture.h>
#include <glib/nw_glib_core.h>

#include <glib_decl.hpp>

#include <lib/utils/math_matrix.h>

namespace NW
{
	/// Abstract Drawable struct
	/// Description:
	/// -- GraphEngine needs objects to draw. That objects has to be a bunch of data
	/// -- We need to save that "drawable" objects with their vertex data.
	/// -- Also, we need to derrive a structes from it. They should be as simple with their data as it's possible
	/// -- This struct contatins all the vertex and index data for drawing
	/// + Reference to material and other drawing stuff
	/// Interface:
	/// -> Create an instance -> Set the attributes -> Give it to the GraphEngine
	struct NW_API ADrawable
	{
	public:
		Mat4f m4Transform = Mat4f(1.0f);
		GMaterial* pGMtl = nullptr;
		UInt8 unDrawOrder = 0;
	public:
		virtual ~ADrawable() = default;

		// --getters
		virtual inline const void* GetVData() const = 0;
		virtual inline UInt32 GetVDataCount() const = 0;
		virtual inline Size GetVDataSize() const = 0;
		virtual inline UInt32 GetVertexSize() const = 0;
		
		virtual inline const UInt32* GetIData() const = 0;
		virtual inline Size GetIDataSize() = 0;
		virtual inline UInt32 GetIDataCount() const = 0;

		// --core_methods
		virtual void UpdateVData() = 0;
		virtual void UpdateIData() = 0;
		// --operators
		inline bool operator>(const ADrawable* pDrb) { return pDrb->unDrawOrder > unDrawOrder; }
		inline bool operator>=(const ADrawable* pDrb) { return pDrb->unDrawOrder >= unDrawOrder; }
		inline bool operator==(const ADrawable* pDrb) { return pDrb->unDrawOrder == unDrawOrder; }
		inline bool operator<=(const ADrawable* pDrb) { return pDrb->unDrawOrder <= unDrawOrder; }
		inline bool operator<(const ADrawable* pDrb) { return pDrb->unDrawOrder < unDrawOrder; }
	};
}

namespace NW
{
	// --==</Drawables>==--
	struct NW_API Particle {
		V3f xyzCrd;
		V3f xyzScale;
		V3f xyzRtn;
		V4f rgbaClr;
	};
	/// Particles struct
	/// Descprition:
	/// -- Consist of points and for beautiful rendering can be configured
	/// -- Renderer needs to set up points size, points shape (via geometry shader)
	/// -- The main thing is to make "DrawCall" with "Points" primitive
	struct NW_API Particles : public ADrawable
	{
	public:
		UInt32 unPartCount;
		float unPartScale;
		PrimitiveTypes DrawPrimitive;
	public:
		Particles();

		// --getters
		virtual inline const void* GetVData() const override { return &vtxData[0]; }
		virtual inline UInt32 GetVDataCount() const override { return vtxData.size(); };
		virtual inline Size GetVDataSize() const override { return GetVDataCount() * GetVertexSize(); }
		virtual inline UInt32 GetVertexSize() const override { return sizeof(VertexBatch1d); }
		
		virtual inline const UInt32* GetIData() const override { return &idxData[0]; }
		virtual inline UInt32 GetIDataCount() const override { return idxData.size(); }
		virtual inline Size GetIDataSize() override { return GetIDataCount() * sizeof(UInt32); }

		// --setters

		// --core_methods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
	private:
		DArray<VertexBatch1d> vtxData;
		DArray<UInt32> idxData;
	};
	/// PolyLine struct
	struct NW_API PolyLine : public ADrawable
	{
	public:
		DArray<V3f> vtxCrds;
	private:
		DArray<VertexBatch1d> vtxData;
		DArray<UInt32> idxData;
	public:
		PolyLine();
		
		// --getters
		virtual inline const void* GetVData() const override { return &vtxData[0]; }
		virtual inline UInt32 GetVDataCount() const override { return vtxData.size(); };
		virtual inline Size GetVDataSize() const override { return GetVDataCount() * GetVertexSize(); }
		virtual inline UInt32 GetVertexSize() const override { return sizeof(VertexBatch1d); }
		
		virtual inline const UInt32* GetIData() const override { return &idxData[0]; }
		virtual inline UInt32 GetIDataCount() const override { return idxData.size(); }
		virtual inline Size GetIDataSize() override { return GetIDataCount() * sizeof(UInt32); }

		// --core_methods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
	};
	/// Polygon struct
	struct NW_API Polygons : public ADrawable
	{
	public:
		V3f xyzCrd;
		V3f xyzScale;
		V3f xyzRtn;
		V4f rgbaClr;
		DArray<V3f> vtxCrds;
	public:
		Polygons();

		// --getters
		virtual inline const void* GetVData() const override { return &vtxData[0]; }
		virtual inline UInt32 GetVDataCount() const override { return vtxData.size(); };
		virtual inline Size GetVDataSize() const override { return GetVDataCount() * GetVertexSize(); }
		virtual inline UInt32 GetVertexSize() const override { return sizeof(VertexBatch1d); }
		
		virtual inline const UInt32* GetIData() const override { return &idxData[0]; }
		virtual inline UInt32 GetIDataCount() const override { return idxData.size(); }
		virtual inline Size GetIDataSize() override { return GetIDataCount() * sizeof(UInt32); }

		// --core_methods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
	private:
		DArray<VertexBatch3d> vtxData;
		DArray<UInt32> idxData;
	};
	// --==</Drawables>==--
}
namespace NW
{
	// --==<Shapes2d>==--
	/// Triangle struct
	struct NW_API Triangle : public ADrawable
	{
	public:
		V2f vtxCrds[3];
	public:
		Triangle();
		Triangle(const V2f& xyV0, const V2f& xyV1, const V2f& xyV2);
		// --getters
		virtual inline const void* GetVData() const override { return &vtxData[0]; }
		virtual inline UInt32 GetVDataCount() const override { return 3; };
		virtual inline Size GetVDataSize() const override { return GetVDataCount() * GetVertexSize(); }
		virtual inline UInt32 GetVertexSize() const override { return sizeof(VertexBatch3d); }
		
		virtual inline const UInt32* GetIData() const override { return &idxData[0]; }
		virtual inline UInt32 GetIDataCount() const override { return 3; }
		virtual inline Size GetIDataSize() override { return GetIDataCount() * sizeof(UInt32); }

		// --core_methods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
	private:
		VertexBatch3d vtxData[3];
		UInt32 idxData[3];
	};
	/// Rectangle struct
	struct NW_API Rectangle : public ADrawable
	{
	public:
		V2f whSize;
		V2f xyPivot;
		SubTexture2d SubTex;
	public:
		Rectangle(const V2f& whSize = V2f{ 1.0f, 1.0f });
		
		// --getters
		virtual inline const void* GetVData() const override { return &vtxData[0]; }
		virtual inline UInt32 GetVDataCount() const override { return 4; };
		virtual inline Size GetVDataSize() const override { return GetVDataCount() * GetVertexSize(); }
		virtual inline UInt32 GetVertexSize() const override { return sizeof(VertexBatch3d); }
		
		virtual inline const UInt32* GetIData() const override { return &idxData[0]; }
		virtual inline UInt32 GetIDataCount() const override { return 6; }
		virtual inline Size GetIDataSize() override { return GetIDataCount() * sizeof(UInt32); }

		// --core_methods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
	private:
		VertexBatch3d vtxData[4];
		UInt32 idxData[6];
		V2f vtxCrds[4];
	};
	/// TileMapSprite struct
	/// --It is assumed to be used with the special geometry shader
	/// --Here is data of triangles with calculated texture coordinates for tiles
	/// --Every tile has 3 vertices: left-bottom, left-top and right-top
	/// --Geometry shader draws quads via strip triangles
	struct NW_API TileMapSprite : public ADrawable
	{
	public:
		TileMapSprite();

		// --getters
		virtual inline const void* GetVData() const override { return &vtxData[0]; }
		virtual inline UInt32 GetVDataCount() const override { return vtxData.size(); };
		virtual inline Size GetVDataSize() const override { return GetVDataCount() * GetVertexSize(); }
		virtual inline UInt32 GetVertexSize() const override { return sizeof(VertexBatch3d); }

		virtual inline const UInt32* GetIData() const override { return &idxData[0]; }
		virtual inline UInt32 GetIDataCount() const override { return idxData.size(); }
		virtual inline Size GetIDataSize() override { return GetIDataCount() * sizeof(UInt32); }

		inline ATexture2d* GetTileMap() { return pTileMap; }
		inline const V2i& GetTileSize() const { return whTileSize; }
		inline const V4i& GetTilePadding() const { return xywhTilePadding; }
		// --setters
		void SetTilemap(ATexture2d* pTex);
		void SetTileSize(const V2i& whSize);
		void SetTilePadding(const V4i& xywhPadding);

		// --core_methods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
		void UpdateTileData();
	private:
		ATexture2d* pTileMap;
		V2i whTileSize;
		V4i xywhTilePadding;
		V2i xyTileCount;
		V2i whWholeTileSize;
		DArray<VertexBatch3d> vtxData;
		DArray<UInt32> idxData;
	};
	// --==</Shapes2d>==--
}
namespace NW
{
	// --==<Shapes3d>==--
	/// Mesh3d struct : public AShape3d
	/// Description:
	/// -- Mesh has default set of data: vertex, normal, and texture coords
	/// -- In compaison with 2d shapes can't be drawn in batch rendering
	/// -- Handles own Vertex and Index buffers
	/// -- For rendering - give it to Renderer3d in method "DrawObject()"
	struct NW_API Mesh3d : public ADrawable
	{
	public:
		Mesh3d();
		~Mesh3d();

		// --getters
		// --setters
		void SetData(VertexShape3d* pVtxData, UInt32 unVtxCount, UInt32* punIndData, UInt32 unIndCount);

		// --core_methods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
	private:
		DArray<VertexShape3d> vtxData;
		DArray<UInt32> idxData;
	};
	// --==</Shapes3d>==--
}

#endif // NW_ADRAWABLE_H