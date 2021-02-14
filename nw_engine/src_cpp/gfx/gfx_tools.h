#ifndef NW_GFX_TOOLS_H
#define NW_GFX_TOOLS_H

#include <gfx_core.hpp>

#if (defined NW_GAPI)
namespace NW
{
	inline Size SdTypeGetSize(ShaderDataTypes sDataType, UInt32 unCount = 1) {
		Size szData = 0;
		switch (sDataType) {
		case SDT_BOOL:	case SDT_INT8:		case SDT_UINT8:		szData = 1;	break;
		case SDT_INT16:	case SDT_UINT16:	case SDT_SAMPLER:	szData = 4;	break;
		case SDT_INT32:	case SDT_UINT32:	case SDT_FLOAT32:	szData = 4;	break;
		case SDT_FLOAT64:										szData = 8;	break;
		default:	NWL_ERR("Invalid shader data type");		szData = 0;	break;
		}
		return szData * unCount;
	}
	inline Size SdTypeGetAllignedSize(ShaderDataTypes sDataType, UInt32 unCount = 1) {
		Size szAll = 0;
		switch (sDataType) {
		case SDT_BOOL:	case SDT_INT8:	case SDT_UINT8:			szAll = 4;	break;
		case SDT_INT16:	case SDT_UINT16:						szAll = 4;	break;
		case SDT_INT32:	case SDT_UINT32:	case SDT_SAMPLER:	szAll = 4;	break;
		case SDT_FLOAT32:										szAll = 4;	break;
		case SDT_FLOAT64:										szAll = 8;	break;
		default:	NWL_ERR("Invalid shader data type");		szAll = 0;	break;
		}
		return szAll * ((unCount + (szAll - 1)) & ~(szAll - 1));
	}
	inline const char* SdTypeGetStr(ShaderDataTypes sdType) {
		return sdType == SDT_BOOL ? "boolean" :
			sdType == SDT_INT8 ? "byte" : sdType == SDT_UINT8 ? "unsigned byte" :
			sdType == SDT_INT16 ? "short" : sdType == SDT_UINT16 ? "unsigned short" :
			sdType == SDT_INT32 ? "integer" : sdType == SDT_UINT32 ? "unsigned integer" :
			sdType == SDT_FLOAT32 ? "float" : sdType == SDT_FLOAT64 ? "double" :
			"unknown";
	}
}
namespace NW
{
	struct NW_API GfxConfig {
		struct {
			struct {
				DrawModes dMode = DM_FILL;
				FacePlanes facePlane = FACE_DEFAULT;
			} PolyMode;
			Float32 nLineWidth = 0.5f;
			Float32 nPixelSize = 0.5f;
			UInt32 unSwapInterval = 1u;
		} General;
		struct {
			Bit bEnable = false;
			BlendConfigs FactorSrc = BC_SRC_ALPHA;
			BlendConfigs FactorDest = BC_ONE_MINUS_SRC_ALPHA;
		} Blending;
		struct {
			Bit bEnable = false;
			CullFaceConfigs CullFactor = CFC_DEFAULT;
		} Culling;
		struct {
			Bit bEnable = false;
			DepthConfigs Func = DTC_DEFAULT;
		} DepthTest;
		struct {
			Bit bEnable = false;
			StencilConfigs Func = STC_DEFAULT;
			UInt8 nBitMask = 0x0;
			UInt32 nRefValue = 0x0;
		} StencilTest;
	public:
		inline void Reset() {
			General.PolyMode.dMode = DM_FILL;
			General.PolyMode.facePlane = FACE_DEFAULT;

			General.nLineWidth = 0.5f;
			General.nPixelSize = 0.5f;

			Blending.bEnable = false;
			Blending.FactorSrc = BC_SRC_ALPHA;
			Blending.FactorDest = BC_ONE_MINUS_SRC_ALPHA;

			DepthTest.bEnable = false;
			DepthTest.Func = DTC_GREATER;
		}
	};
	/// GraphicsApiInfo struct
	struct NW_API GfxInfo
	{
	public:
		Char strRenderer[256], strVersion[256], strVendor[256], strShdLang[256];
		Int32 nMaxVertexAttribs = 0;
		Int32 nActiveTextureId = 0;
		Int32 nMaxTextures = 0;
	public:
		inline OutStream& operator<<(OutStream& rStream);
	};
	inline OutStream& GfxInfo::operator<<(OutStream& rStream) {
		return rStream <<
			"--==<graphics_info>==--" << std::endl <<
			"graphics context: " << &strVersion[0] << std::endl <<
			"renderer: " << &strRenderer[0] << std::endl <<
			"version: " << &strVersion[0] << std::endl <<
			"vendor: " << &strVendor[0] << std::endl <<
			"shading language: " << &strShdLang[0] << std::endl <<
			"--==</graphics_info>==--" << std::endl;
	}
	inline OutStream& operator<<(OutStream& rStream, GfxInfo& rgInfo) { return rgInfo.operator<<(rStream); }
	/// DrawTools struct
	/// Description:
	/// -- Check and change renderer attributes for more or less performance/resource usage
	struct NW_API DrawTools
	{
	public:
		// --vertex_data
		UByte* pVtxData = nullptr;
		UByte* pVtxIter = nullptr;
		Size szVtxData = 0;
		// --index_data
		UInt32* pIdxIter = nullptr;
		UInt32* pIdxData = nullptr;
		Size szIdxData = 0;
		// --shader_data
		UByte* pShdData = nullptr;
		UByte* pShdIter = nullptr;
		Size szShdData = 0;
		// --texture_data
		Texture* pTextures[1];
		UInt8 unTexCount = 0;
		// --objects
		Shader* pShader = nullptr;
		GfxMaterial* pgMtl = nullptr;
		VertexBuf* pVtxBuf = nullptr;
		IndexBuf* pIdxBuf = nullptr;
		ShaderBuf* pShdBuf = nullptr;
	public:
		inline void ResetData() {
			pVtxIter = &pVtxData[0];
			szVtxData = 0;
			pIdxIter = &pIdxData[0];
			szIdxData = 0;
			pShdIter = &pShdData[0];
			szShdData = 0;
			for (UInt8 txi = 0; txi < unTexCount; txi++) { pTextures[txi] = nullptr; }
			unTexCount = 0;
		}
	};
}
namespace NW
{
	/// BufferElement structure
	/// --Contains all relevant data for shader usage of vertex buffer data
	struct NW_API BufferElement
	{
	public:
		Char strName[128];
		ShaderDataTypes sdType;
		UInt32 unCount;
		Bit bNormalized;
		UInt32 unOffset;
	public:
		BufferElement() : strName(""), sdType(ShaderDataTypes::SDT_DEFAULT), unCount(0), bNormalized(false), unOffset(0) { }
		BufferElement(const char* sName, ShaderDataTypes dataType, UInt32 Count, bool Normalized) :
			strName(""), sdType(dataType), unCount(Count),
			bNormalized(Normalized), unOffset(0) {
			strcpy(strName, sName);
		}
	};
	struct NW_API ShaderBlock
	{
	public:
		Char strName[128];
		UInt8 unBindPoint;
		DArray<BufferElement> BufElems;
		Size szOffset;
		Size szAll;
	public:
		ShaderBlock() : strName(""), unBindPoint(0), szOffset(0), szAll(0) {};
		ShaderBlock(const char* sName, UInt8 BindPoint) :
			strName(""),
			unBindPoint(BindPoint), szAll(0), szOffset(0) {
			strcpy(strName, sName);
		}
	};
	/// VertexBufLayout class
	class NW_API VertexBufLayout
	{
	public:
		VertexBufLayout() : m_unStride(1) { }
		VertexBufLayout(const DArray<BufferElement>& rBufElems) :
			m_unStride(1) {
			SetElements(rBufElems);
		}

		// --getters
		inline const BufferElement& GetElem(UInt8 unIdx) const { return m_BufElems.at(unIdx); }
		inline const DArray<BufferElement>& GetElems() const { return m_BufElems; }
		inline const UInt32 GetStride() const { return m_unStride; }
		// --setters
		inline void SetElements(const DArray<BufferElement>& rBufElems) { m_BufElems = rBufElems; Update(); }
		inline void AddElement(const BufferElement& rBufElem, Int8 nElems = 1) { while (nElems-- > 0) { m_BufElems.push_back(rBufElem); } Update(); }
		inline void AddElement(const char* strName, ShaderDataTypes sdType, UInt8 unCount, Int8 nElems = 1) {
			AddElement(BufferElement(strName, sdType, unCount, false), nElems);
		}
		inline void Reset() { m_unStride = 0; m_BufElems.clear(); }
	private:
		UInt32 m_unStride;
		DArray<BufferElement> m_BufElems;
	private:
		inline void Update() {
			m_unStride = 0;
			for (auto& rBufElem : m_BufElems) {
				rBufElem.unOffset = m_unStride;
				m_unStride += SdTypeGetSize(rBufElem.sdType, rBufElem.unCount);
			}
		}
	};
	/// ShaderBufLayout class
	class NW_API ShaderBufLayout
	{
	public:
		ShaderBufLayout() : m_szData(0) { }
		ShaderBufLayout(const DArray<ShaderBlock>& rBlocks) :
			m_szData(0) {
			SetBlocks(rBlocks);
		}
		// --getters
		inline Size GetSize() const { return m_szData; }
		inline const DArray<BufferElement>& GetGlobals() const { return m_Globals; }
		inline const ShaderBlock& GetBlock(UInt8 unIdx) const { return m_Blocks.at(unIdx); }
		inline const DArray<ShaderBlock>& GetBlocks() const { return m_Blocks; }
		// --setters
		inline void AddGlobalElem(const BufferElement& rBufElem) { m_Globals.push_back(rBufElem); }
		inline void SetBlocks(const DArray<ShaderBlock>& rBlocks) { m_Blocks = rBlocks; Update(); }
		inline void AddBlock(const ShaderBlock& rBlock, Int8 nElems = 1) { while (nElems-- > 0) { m_Blocks.push_back(rBlock); } Update(); }
		inline void Reset() { m_szData = 0; m_Blocks.clear(); m_Globals.clear(); }
	private:
		DArray<BufferElement> m_Globals;
		DArray<ShaderBlock> m_Blocks;
		Size m_szData;
	private:
		inline void Update() {
			m_szData = 0;
			for (UInt8 bi = 0; bi < m_Blocks.size(); bi++) {
				auto& rBlock = m_Blocks[bi];
				rBlock.unBindPoint = bi;
				rBlock.szOffset = m_szData;
				for (auto& rElem : rBlock.BufElems) {
					rElem.unOffset += rBlock.szAll;
					rBlock.szAll += SdTypeGetAllignedSize(rElem.sdType, rElem.unCount);
				}
				m_szData += rBlock.szAll;
			}
		}
	};
}
#if (NW_GAPI & NW_GAPI_OGL)
// --functions
namespace NW
{
	/// Clear GL error buffer
	void OglClearErr();
	/// Return suitable error message accordingly to glGetError()
	bool OglErrLog(const char* strInfo, const char* strFile, int nLine);
	/// Get compile and linking status return true if there are errors
	int OglErrLogShader(ShaderTypes ShaderType, UInt32 unShaderId);
}
	#if (defined NW_DEBUG)
			#define OGL_CALL(call) OglClearErr();\
				call;\
				NWL_ASSERT(OglErrLog(#call, NWL_FNAME_APATH((std::string)__FILE__), __LINE__), "GL_ERROR: ")
			#define NWL_OGL_SHADER_ERR_LOG(errType, objectID) (OglErrLogShader(errType, objectID))
			#define NWL_OGL_SHADER_ERR_LOG(errType, objectID) (OglErrLogShader(errType, objectID))
	#else
		#define OGL_CALL(function);
	#endif
#endif
#if (NW_GAPI & NW_GAPI_DX)
// --functions
namespace NW
{
	bool DxErrLog(const char* strInfo, const char* strFile, int nLine);
}
#endif
#endif	// NW_GAPI
#endif	// NW_GFX_TOOLS_H