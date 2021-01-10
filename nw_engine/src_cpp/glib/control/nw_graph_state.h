#ifndef NW_DRAW_LAYER_H
#define NW_DRAW_LAYER_H

#include <glib/render/nw_drawable.h>

namespace NW
{
	/// Abstract GraphState class
	/// Description:
	/// -- GraphEngine works via this class
	/// -- This is set of data required for organized rendering
	/// -- Here are all relevant setting for every draw call
	class NW_API GraphState
	{
	public:
		UInt8 unDrawOrder = 0;
		GCamera* pGCamera = nullptr;
		AFrameBuf* pFrameBuf = nullptr;
		V4f xywhViewport;
		HashMap<GMaterial*, DArray<ADrawable*>> m_DData;

		struct {
			PrimitiveTypes DPrimitive = PT_TRIANGLES;
			DrawModes DMode = DM_FILL;
			Float32 nLineWidth = 0.5f;
			Float32 nPixelSize = 0.5f;
		} Config;
		struct {
			bool bEnable = false;
			BlendConfigs FactorSrc = BC_SRC_ALPHA;
			BlendConfigs FactorDest = BC_ONE_MINUS_SRC_ALPHA;
		} Blending;
		struct {
			bool bEnable = false;
			DepthConfigs Func = DC_;
		} DepthTest;
	public:
		GraphState() : m_strName("des_default") { }
		GraphState(const char* sName) : m_strName("") { strcpy(m_strName, sName); }

		// --getters
		inline const char* GetName() { return m_strName; }
		// --setters
		inline void Reset() {
			for (auto& itDData : m_DData) { itDData.second.clear(); }
			
			Config.DPrimitive = PT_TRIANGLES;
			Config.DMode = DM_FILL;
			
			DepthTest.bEnable = false;
			DepthTest.Func = DC_;
			
			Blending.bEnable = false;
			Blending.FactorSrc = BC_SRC_ALPHA;
			Blending.FactorDest = BC_ONE_MINUS_SRC_ALPHA;
		}
		inline void AddDrawable(ADrawable* pDrb) { m_DData[pDrb->pGMtl].push_back(pDrb); }
		// --operators
		inline bool operator>(const GraphState* pDS) { return	pDS->unDrawOrder > unDrawOrder; }
		inline bool operator>=(const GraphState* pDS) { return	pDS->unDrawOrder >= unDrawOrder; }
		inline bool operator==(const GraphState* pDS) { return	pDS->unDrawOrder == unDrawOrder; }
		inline bool operator<=(const GraphState* pDS) { return	pDS->unDrawOrder <= unDrawOrder; }
		inline bool operator<(const GraphState* pDS) { return	pDS->unDrawOrder < unDrawOrder; }
	private:
		Char m_strName[256] = "des_default";
	};
}

#endif	// NW_DRAW_LAYER_H