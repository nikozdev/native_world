#ifndef NW_ADRAWABLE_H
#define NW_ADRAWABLE_H

#include <gl/nw_gl_core.h>
#include <gl/vision/nw_gmaterial.h>

#include <gl_decl.hpp>

namespace NW
{
	/// Abstract Drawable class
	/// Description:
	/// -- Drawer needs objects to draw. That objects has to be a bunch of data
	/// -- We need to save that "drawable" objects with their vertex data.
	/// -- Also, we need to derrive a classes from it. They should be as simple with their data as it's possible
	/// -- This class contatins all the vertex and index data for drawing
	/// + Reference to material and other drawing stuff
	/// Interface:
	/// -> Create an instance -> Set the attributes -> Give it to the Drawer
	class NW_API ADrawable
	{
	public:
		Mat4f m4Transform = Mat4f(1.0f);
	public:
		virtual ~ADrawable() = default;

		// -- Getters
		virtual inline void* GetVData() = 0;
		virtual inline UInt32 GetVDataCount() const = 0;
		virtual inline Size GetVDataSize() const = 0;
		virtual inline UInt32 GetVertexSize() const = 0;
		virtual inline UInt32* GetIData() = 0;
		virtual inline UInt32 GetIDataCount() const = 0;
		virtual inline AGMaterial* GetGMaterial() = 0;

		// -- Setters
		virtual void SetGMaterial(AGMaterial* pGMtl) = 0;

		// -- Core Methods
		virtual void UpdateVData() = 0;
		virtual void UpdateIData() = 0;
	};
}

namespace NW
{
	// ========</Drawables>========
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

		// -- Getters
		virtual inline void* GetVData() override { UpdateVData(); return &vtxVData[0]; }
		virtual inline UInt32 GetVDataCount() const override { return vtxVData.size(); };
		virtual inline Size GetVDataSize() const override { return GetVDataCount() * GetVertexSize(); }
		virtual inline UInt32 GetVertexSize() const override { return sizeof(VertexBatch1d); }
		virtual inline UInt32* GetIData() override { UpdateIData(); return &indIData[0]; }
		virtual inline UInt32 GetIDataCount() const override { return indIData.size(); }
		virtual inline AGMaterial* GetGMaterial() override { return m_pGMtl; }

		// -- Setters
		virtual void SetGMaterial(AGMaterial* pGMtl) override { m_pGMtl = pGMtl; }

		// -- Core Methods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
	private:
		DArray<VertexBatch1d> vtxVData;
		DArray<UInt32> indIData;
		AGMaterial* m_pGMtl;
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
		
		// -- Getters
		virtual inline void* GetVData() override { UpdateVData(); return &vtxVData[0]; }
		virtual inline UInt32 GetVDataCount() const override { return vtxVData.size(); };
		virtual inline Size GetVDataSize() const override { return GetVDataCount() * GetVertexSize(); }
		virtual inline UInt32 GetVertexSize() const override { return sizeof(VertexBatch1d); }
		virtual inline UInt32* GetIData() override { UpdateIData(); return &indIData[0]; }
		virtual inline UInt32 GetIDataCount() const override { return indIData.size(); }
		virtual inline AGMaterial* GetGMaterial() override { return m_pGMtl; }

		// -- Setters
		virtual void SetGMaterial(AGMaterial* pGMtl) override { m_pGMtl = pGMtl; }

		// -- Core Methods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
	private:
		AGMaterial* m_pGMtl;
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

		// -- Getters
		virtual inline void* GetVData() override { UpdateVData(); return &vtxVData[0]; }
		virtual inline UInt32 GetVDataCount() const override { return vtxVData.size(); };
		virtual inline Size GetVDataSize() const override { return GetVDataCount() * GetVertexSize(); }
		virtual inline UInt32 GetVertexSize() const override { return sizeof(VertexBatch1d); }
		virtual inline UInt32* GetIData() override { UpdateIData(); return &indIData[0]; }
		virtual inline UInt32 GetIDataCount() const override { return indIData.size(); }
		virtual inline AGMaterial* GetGMaterial() override { return m_pGMtl; }

		// -- Setters
		virtual void SetGMaterial(AGMaterial* pGMtl) override { m_pGMtl = pGMtl; }

		// -- Core Methods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
	private:
		DArray<VertexBatch3d> vtxVData;
		DArray<UInt32> indIData;
		AGMaterial* m_pGMtl;
	};
	// ========</Drawables>========
}
namespace NW
{
	// ========<Shapes2d>========
	/// Triangle class
	class NW_API Triangle : public ADrawable
	{
	public:
		V2f vtxCrds[3];
	public:
		Triangle();
		Triangle(const V2f& xyV0, const V2f& xyV1, const V2f& xyV2);
		// -- Getters
		virtual inline void* GetVData() override { UpdateVData(); return &vtxVData[0]; }
		virtual inline UInt32 GetVDataCount() const override { return 3; };
		virtual inline Size GetVDataSize() const override { return GetVDataCount() * GetVertexSize(); }
		virtual inline UInt32 GetVertexSize() const override { return sizeof(VertexBatch3d); }
		virtual inline UInt32* GetIData() override { UpdateIData(); return &indIData[0]; }
		virtual inline UInt32 GetIDataCount() const override { return 3; }
		virtual inline AGMaterial* GetGMaterial() override { return m_pGMtl; }

		// -- Setters
		virtual void SetGMaterial(AGMaterial* pGMtl) override { m_pGMtl = pGMtl; }

		// -- Core Methods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
	private:
		VertexBatch3d vtxVData[3];
		UInt32 indIData[3];
		AGMaterial* m_pGMtl;
	};
	/// Rectangle class
	class NW_API Rectangle : public ADrawable
	{
	public:
		V2f whSize;
		V2f xyPivot;
	public:
		Rectangle(const V2f& whSize = V2f{ 1.0f, 1.0f });
		
		// -- Getters
		virtual inline void* GetVData() override { UpdateVData(); return &vtxVData[0]; }
		virtual inline UInt32 GetVDataCount() const override { return 4; };
		virtual inline Size GetVDataSize() const override { return GetVDataCount() * GetVertexSize(); }
		virtual inline UInt32 GetVertexSize() const override { return sizeof(VertexBatch3d); }
		virtual inline UInt32* GetIData() override { UpdateIData(); return &indIData[0]; }
		virtual inline UInt32 GetIDataCount() const override { return 6; }
		virtual inline AGMaterial* GetGMaterial() override { return m_pGMtl; }

		// -- Setters
		virtual void SetGMaterial(AGMaterial* pGMtl) override { m_pGMtl = pGMtl; }

		// -- InterfaceMethods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
	protected:
		VertexBatch3d vtxVData[4];
		UInt32 indIData[6];
		V2f vtxCrds[4];
		AGMaterial* m_pGMtl;
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
	// ========</Shapes2d>========
}
namespace NW
{
	// ========<Shapes3d>========
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

		// -- Getters
		// -- Setters
		void SetData(VertexShape3d* pVtxData, UInt32 unVtxCount, UInt32* punIndData, UInt32 unIndCount);
		virtual void SetGMaterial(AGMaterial* pGMtl) override { m_pGMtl = pGMtl; }

		// -- Interface Methods
		virtual void UpdateVData() override;
		virtual void UpdateIData() override;
	private:
		DArray<VertexShape3d> vtxVData;
		DArray<UInt32> indIData;
		AGMaterial *m_pGMtl = nullptr;
	};
	// ========</Shapes3d>========
}

#endif // NW_ADRAWABLE_H