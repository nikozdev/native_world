#ifndef NW_ADRAWABLE_H
#define NW_ADRAWABLE_H

#include <glib/nw_texture.h>
#include <glib/nw_gtools.h>

#include <nw_glib_decl.hpp>

namespace NW
{
	/// Abstract Drawable class
	/// Description:
	/// -- GEngine needs objects to draw. That objects has to be a bunch of data
	/// -- We need to save that "drawable" objects with their vertex data.
	/// -- Also, we need to derrive a classes from it. They should be as simple with their data as it's possible
	/// -- This class contatins all the vertex and index data for drawing
	/// + Reference to material and other drawing stuff
	/// Interface:
	/// -> Create an instance -> Set the attributes -> Give it to the GEngine
	class NW_API ADrawable
	{
	public:
		Mat4f m4Transform = Mat4f(1.0f);
		GMaterial* pGMtl = nullptr;
	public:
		virtual ~ADrawable() = default;

		// --getters
		// --core_methods
		virtual void OnDraw() = 0;
	};
}

namespace NW
{
	// --==</Drawables>==--
	class NW_API Particle {
		V3f xyzCrd;
		V3f xyzScale;
		V3f xyzRtn;
		V4f rgbaClr;
	};
	/// Particles class
	/// Descprition:
	/// -- Consist of points and for beautiful rendering can be configured
	/// -- Renderer needs to set up points size, points shape (via geometry shader)
	/// -- The main thing is to make "DrawCall" with "Points" primitive
	class NW_API Particles : public ADrawable
	{
	public:
		UInt32 unPartCount;
		float unPartScale;
		GPrimitiveTypes DrawPrimitive;
	public:
		Particles();

		// --getters
		// --setters

		// --core_methods
		virtual void OnDraw() override;
	private:
		DArray<VertexBatch1d> vtxData;
		DArray<UInt32> idxData;
	};
	/// PolyLine class
	class NW_API PolyLine : public ADrawable
	{
	public:
		DArray<V3f> vtxCrds;
	private:
		DArray<VertexBatch1d> vtxData;
		DArray<UInt32> idxData;
	public:
		PolyLine();
		
		// --getters

		// --core_methods
		virtual void OnDraw() override;
	};
	/// Polygon class
	class NW_API Polygons : public ADrawable
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
		// --core_methods
		virtual void OnDraw() override;
	private:
		DArray<VertexBatch3d> vtxData;
		DArray<UInt32> idxData;
	};
	// --==</Drawables>==--
}
namespace NW
{
	// --==<Shapes2d>==--
	/// Triangle class
	class NW_API Triangle : public ADrawable
	{
	public:
		V2f vtxCrds[3];
	public:
		Triangle(const V2f& xyV0 = { -1.0f, -1.0f }, const V2f& xyV1 = { -0.5f, 1.0f }, const V2f& xyV2 = { 1.0f, -1.0f });
		
		// --getters
		// --core_methods
		virtual void OnDraw() override;
	private:
		VertexBatch3d vtxData[3];
		UInt32 idxData[3];
	};
	/// Rectangle class
	class NW_API Rectangle : public ADrawable
	{
	public:
		V2f whSize;
		V2f xyPivot;
		SubTexture2d SubTex;
	public:
		Rectangle(const V2f& whSize = V2f{ 1.0f, 1.0f });
		
		// --getters
		// --core_methods
		virtual void OnDraw() override;
	private:
		VertexBatch3d vtxData[4];
		UInt32 idxData[6];
		V2f vtxCrds[4];
	};
	class NW_API TileSprite
	{
		V2i xyIdx = { 0, 0 };
		SubTexture2d SubTex;
	};
	/// TileMapSprite class
	/// --It is assumed to be used with the special geometry shader
	/// --Here is data of triangles with calculated texture coordinates for tiles
	/// --Every tile has 3 vertices: left-bottom, left-top and right-top
	/// --Geometry shader draws quads via strip triangles
	class NW_API TileMapSprite : public ADrawable
	{
	public:
		V2f whTileSize;
		V4f xywhTilePadding;
		ATexture2d* pTileMap;
		DArray<TileSprite> Tiles;
	public:
		TileMapSprite();

		// --getters
		// --core_methods
		virtual void OnDraw() override;
		void UpdateTileData();
	private:
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
	/// Mesh3d class : public AShape3d
	/// Description:
	/// -- Mesh has default set of data: vertex, normal, and texture coords
	/// -- In compaison with 2d shapes can't be drawn in batch rendering
	/// -- Handles own Vertex and Index buffers
	/// -- For rendering - give it to Renderer3d in method "DrawObject()"
	class NW_API Mesh3d : public ADrawable
	{
	public:
		Mesh3d();
		~Mesh3d();

		// --getters
		// --setters
		void SetData(VertexShape3d* pVtxData, UInt32 unVtxCount, UInt32* punIndData, UInt32 unIndCount);

		// --core_methods
		virtual void OnDraw() override;
	private:
		DArray<VertexShape3d> vtxData;
		DArray<UInt32> idxData;
	};
	// --==</Shapes3d>==--
}

#endif // NW_ADRAWABLE_H