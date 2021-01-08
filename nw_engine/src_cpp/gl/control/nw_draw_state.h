#ifndef NW_DRAW_STATE
#define NW_DRAW_STATE

#include <gl/render/nw_drawable.h>

#undef DrawState

namespace NW
{
	/// DrawState struct
	/// Description:
	/// -- DrawEngine works via this class
	/// -- This is set of data required for organized rendering
	/// -- Here are all relevant setting for every draw call
	class NW_API DrawState
	{
		friend class DrawEngine;
	public:
		char strName[128] = "des_default";
		UInt8 unDrawOrder = 0;
		GCamera* pGCamera = nullptr;
		AFrameBuf* pFrameBuf = nullptr;
		V4f xywhViewport;
		PrimitiveTypes DPrimitive = PT_TRIANGLES;
		HashMap<GMaterial*, DArray<ADrawable*>> m_DData;
	public:
		DrawState() : strName("des_default") { }
		DrawState(const char* sName) : strName("") { strcpy(&strName[0], &sName[0]); }

		// --setters
		inline void Reset() {
			unDrawOrder = 0;
			pGCamera = nullptr;
			pFrameBuf = nullptr;
			xywhViewport = { -1.0f, -1.0f, 1.0f, 1.0f };
			DPrimitive = PT_TRIANGLES;
			for (auto& itDData : m_DData) { itDData.second.clear(); }
		}
		inline void AddDrawable(ADrawable* pDrb) { m_DData[pDrb->pGMtl].push_back(pDrb); }
		// --operators
		inline bool operator>(const DrawState* pDS) { return	pDS->unDrawOrder > unDrawOrder; }
		inline bool operator>=(const DrawState* pDS) { return	pDS->unDrawOrder >= unDrawOrder; }
		inline bool operator==(const DrawState* pDS) { return	pDS->unDrawOrder == unDrawOrder; }
		inline bool operator<=(const DrawState* pDS) { return	pDS->unDrawOrder <= unDrawOrder; }
		inline bool operator<(const DrawState* pDS) { return	pDS->unDrawOrder < unDrawOrder; }
	};
}

#endif	// NW_DRAW_STATE