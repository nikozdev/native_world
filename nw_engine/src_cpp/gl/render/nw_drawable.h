#ifndef NW_ADRAWABLE_H
#define NW_ADRAWABLE_H

#include <gl/nw_gl_core.h>
#include <gl/vision/nw_gmaterial.h>

#include <gl_decl.hpp>

namespace NW
{
	/// Abstract Drawable class
	/// Description:
	/// -- GraphEngine needs objects to draw. That objects has to be a bunch of data
	/// -- We need to save that "drawable" objects with their vertex data.
	/// -- Also, we need to derrive a classes from it. They should be as simple with their data as it's possible
	/// -- This class contatins all the vertex and index data for drawing
	/// + Reference to material and other drawing stuff
	/// Interface:
	/// -> Create an instance -> Set the attributes -> Give it to the GraphEngine
	class NW_API ADrawable
	{
	public:
		Mat4f m4Transform = Mat4f(1.0f);
		GMaterial* pGMtl = nullptr;
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
	class NW_API Particles : public ADrawable
	{
	public:
		UInt32 unPartCount;
		float unPartScale;
		PrimitiveTypes DrawPrimitive;
	public:
		Particles();

		// --getters
		virtual inline const void* GetVData() const override { return &vtxVData[0]; }
		virtual inline UInt32 GetVDataCount() const override { return vtxVData.size(); };
		virtual inline Size GetVDataSize() const override { return GetVDataCount() * GetVertexSize(); }
		virtual inline UInt32 GetVertexSize() const override { return sizeof(VertexBatch1d); }
		
		virtual inline const UInt32* GetIData() const override { return &indIData[0]; }
		virtual inline UInt32 GetIDataCount() const override { return indIData.size(); }
		virtual inline Size GetIDataSize() override { return GetIDataCount() * sizeof(UInt32); }

		// --setters

		// --core_methods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
	private:
		DArray<VertexBatch1d> vtxVData;
		DArray<UInt32> indIData;
	};
	/// PolyLine class
	class NW_API PolyLine : public ADrawable
	{
	public:
		DArray<V3f> vtxCrds;
	private:
		DArray<VertexBatch1d> vtxVData;
		DArray<UInt32> indIData;
	public:
		PolyLine();
		
		// --getters
		virtual inline const void* GetVData() const override { return &vtxVData[0]; }
		virtual inline UInt32 GetVDataCount() const override { return vtxVData.size(); };
		virtual inline Size GetVDataSize() const override { return GetVDataCount() * GetVertexSize(); }
		virtual inline UInt32 GetVertexSize() const override { return sizeof(VertexBatch1d); }
		
		virtual inline const UInt32* GetIData() const override { return &indIData[0]; }
		virtual inline UInt32 GetIDataCount() const override { return indIData.size(); }
		virtual inline Size GetIDataSize() override { return GetIDataCount() * sizeof(UInt32); }

		// --core_methods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
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
		virtual inline const void* GetVData() const override { return &vtxVData[0]; }
		virtual inline UInt32 GetVDataCount() const override { return vtxVData.size(); };
		virtual inline Size GetVDataSize() const override { return GetVDataCount() * GetVertexSize(); }
		virtual inline UInt32 GetVertexSize() const override { return sizeof(VertexBatch1d); }
		
		virtual inline const UInt32* GetIData() const override { return &indIData[0]; }
		virtual inline UInt32 GetIDataCount() const override { return indIData.size(); }
		virtual inline Size GetIDataSize() override { return GetIDataCount() * sizeof(UInt32); }

		// --core_methods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
	private:
		DArray<VertexBatch3d> vtxVData;
		DArray<UInt32> indIData;
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
		Triangle();
		Triangle(const V2f& xyV0, const V2f& xyV1, const V2f& xyV2);
		// --getters
		virtual inline const void* GetVData() const override { return &vtxVData[0]; }
		virtual inline UInt32 GetVDataCount() const override { return 3; };
		virtual inline Size GetVDataSize() const override { return GetVDataCount() * GetVertexSize(); }
		virtual inline UInt32 GetVertexSize() const override { return sizeof(VertexBatch3d); }
		
		virtual inline const UInt32* GetIData() const override { return &indIData[0]; }
		virtual inline UInt32 GetIDataCount() const override { return 3; }
		virtual inline Size GetIDataSize() override { return GetIDataCount() * sizeof(UInt32); }

		// --core_methods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
	private:
		VertexBatch3d vtxVData[3];
		UInt32 indIData[3];
	};
	/// Rectangle class
	class NW_API Rectangle : public ADrawable
	{
	public:
		V2f whSize;
		V2f xyPivot;
	public:
		Rectangle(const V2f& whSize = V2f{ 1.0f, 1.0f });
		
		// --getters
		virtual inline const void* GetVData() const override { return &vtxVData[0]; }
		virtual inline UInt32 GetVDataCount() const override { return 4; };
		virtual inline Size GetVDataSize() const override { return GetVDataCount() * GetVertexSize(); }
		virtual inline UInt32 GetVertexSize() const override { return sizeof(VertexBatch3d); }
		
		virtual inline const UInt32* GetIData() const override { return &indIData[0]; }
		virtual inline UInt32 GetIDataCount() const override { return 6; }
		virtual inline Size GetIDataSize() override { return GetIDataCount() * sizeof(UInt32); }

		// -- InterfaceMethods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
	protected:
		VertexBatch3d vtxVData[4];
		UInt32 indIData[6];
		V2f vtxCrds[4];
	};
	/// Sprite struct
	struct NW_API Sprite : public Rectangle
	{
	public:
		SubTexture2d SubTex;
	public:
		Sprite();
		
		// -- InterfaceMethods
		virtual void UpdateVData() override;
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
		DArray<VertexShape3d> vtxVData;
		DArray<UInt32> indIData;
	};
	// --==</Shapes3d>==--
}

#endif // NW_ADRAWABLE_H