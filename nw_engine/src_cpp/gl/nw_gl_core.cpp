#include <nw_pch.hpp>
#include "nw_gl_core.h"

#include <sys/nw_mem_sys.h>

#if (defined NW_GRAPHICS)
// Functions
namespace NW
{
	static inline UInt32 SDType_GetSize(ShaderDataTypes sDataType)
	{
		switch (sDataType)
		{
		case SDT_BOOL: case SDT_INT8: case SDT_UINT8:
			return 1;
		case SDT_INT16: case SDT_UINT16:
			return 2;
		case SDT_INT32: case SDT_UINT32: case SDT_FLOAT32:
			return 4;
		}
		NW_ERR("Inaccessible shader data type");
	}
}
// BufferLayout
namespace NW
{
	void BufferLayout::Update()
	{
		unStride = 0;
		for (auto& elem : BufElems)
		{
			elem.unOffset = unStride;
			unStride += elem.unCount * SDType_GetSize(elem.sdType);
		}
	}
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
#include <glad/glad.h>
// Functions
namespace NW
{
	void OGL_ClearErr()
	{// glGetError gets last message and clears errorLog
		while (glGetError() != GL_NO_ERROR);
	}

	bool OGL_ErrLog(const char* funcName, const char* file, int line)
	{// Get error and match error message for it
		UInt32 errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case NW_INVALID_ENUM:
				error = "INVALID_ENUM";
				break;
			case NW_INVALID_VALUE:
				error = "INVALID_VALUE";
				break;
			case NW_INVALID_OPERATION:
				error = "INVALID_OPERATION";
				break;
			case NW_STACK_OVERFLOW:
				error = "STACK_OVERFLOW";
				break;
			case NW_STACK_UNDERFLOW:
				error = "STACK_UNDERFLOW";
				break;
			case NW_OUT_OF_MEMORY:
				error = "OUT_OF_MEMORY";
				break;
			}
			printf("NW_ERR_LOG::FILE_%s::LINE_%d:\n%s\n",
				funcName, line, error.c_str());
			return false;
		}
		return true;
	}

	Int32 OGL_ErrLog_Shader(ShaderTypes shaderType, UInt32 shaderID)
	{
		if (shaderID == 0) return NW_ERR_UNKNOWN_ID;
		Int32 success = 0;
		Int32 errLogSize = 0;
		String errLog;
		std::string strType = shaderType == NW_SHADER_VERTEX ?
			"SHADER_VERTEX" : shaderType == NW_SHADER_PIXEL ?
			"SHADER_PIXEL" : "SHADER_PROG";
		if (shaderType != ST_SHADER)
		{
			glGetShaderiv(shaderID, NW_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &errLogSize);
				errLog.resize(errLogSize);

				glGetShaderInfoLog(shaderID, errLogSize, NULL, &errLog[0]);
				std::cout << "Failed to compile " << strType << std::endl;
				NW_ERR(errLog);
				return NW_ERR_SHADER_COMPILE;
			}
		}
		else
		{
			glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &errLogSize);
				errLog.resize(errLogSize);

				glGetProgramInfoLog(shaderID, errLogSize, NULL, &errLog[0]);
				std::cout << "Failed to link " << strType << std::endl;
				NW_ERR(&errLog[0]);
				return NW_ERR_SHADER_LINK;
			}
		}
		return NW_OK;
	}
}
#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
#endif	// NW_GRAPHICS

#include <sys/nw_mem_sys.h>
#include <sys/nw_data_sys.h>

#if (defined NW_GRAPHICS)
#include <gl/control/nw_drawer.h>
// Buffers
namespace NW
{
	AVertexBuf* AVertexBuf::Create(Size szAlloc, void* pVtxData)
	{
		AVertexBuf* pVB = nullptr;
		switch (Drawer::GetGApi()->GetType()) {
		#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
		case GApiTypes::GAPI_COUT:
			if (pVtxData == nullptr) pVB = MemSys::NewT<VertexBufCout>;
			else pVB = MemSys::NewT<VertexBufCout>(szAlloc, pVtxData);
			break;
		#endif	// NW_GRAPHICS
		#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL:
			if (pVtxData == nullptr) pVB = MemSys::NewT<VertexBufOgl>(szAlloc);
			else pVB = MemSys::NewT<VertexBufOgl>(szAlloc, pVtxData);
			break;
		#endif	// NW_GRAPHICS
		default:
			NW_ERR("There is no accessible API");
			break;
		}
		return pVB;
	}
	AIndexBuf* AIndexBuf::Create(UInt32 unCount, UInt32* pIndData)
	{
		AIndexBuf* pIB = nullptr;
		switch (Drawer::GetGApi()->GetType()) {
		#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
		case GApiTypes::GAPI_COUT:
			if (iDataPtr == nullptr) pIB = MemSys::NewT<IndexBufCout>(unCount);
			else pIB = MemSys::NewT<IndexBufCout>(unCount, iDataPtr);
			break;
		#endif // NW_GRAPHICS
		#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL:
			if (pIndData == nullptr) pIB = MemSys::NewT<IndexBufOgl>(unCount);
			else pIB = MemSys::NewT<IndexBufOgl>(unCount, pIndData);
			break;
		#endif // NW_GRAPHICS
		default:
			NW_ERR("Graphics API is not defined");
			break;
		}
		return pIB;
	}
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
#include <glad/glad.h>
namespace NW
{
	// VertexBufOglfer
	VertexBufOgl::VertexBufOgl(Size szAlloc) :
		m_unRIdVB(0), m_szData(szAlloc)
	{
		glGenBuffers(1, &m_unRIdVB);
		glGenVertexArrays(1, &m_unRIdVA);
		glBindBuffer(GL_ARRAY_BUFFER, m_unRIdVB);
		glBufferData(GL_ARRAY_BUFFER, szAlloc, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	VertexBufOgl::VertexBufOgl(Size szAlloc, void* pVtxData) :
		m_unRIdVB(0), m_szData(szAlloc)
	{
		glGenBuffers(1, &m_unRIdVB);
		glGenVertexArrays(1, &m_unRIdVA);
		glBindBuffer(GL_ARRAY_BUFFER, m_unRIdVB);
		glBufferData(GL_ARRAY_BUFFER, szAlloc, pVtxData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	VertexBufOgl::~VertexBufOgl()
	{
		glDeleteBuffers(1, &m_unRIdVB);
		glDeleteVertexArrays(1, &m_unRIdVA);
	}

	// Interface Methods
	void VertexBufOgl::SetData(Size szAlloc, void* pVtxData, Size szOffset)
	{
		m_szData += szAlloc;
		glBindBuffer(GL_ARRAY_BUFFER, m_unRIdVB);
		glBufferSubData(GL_ARRAY_BUFFER, szOffset, szAlloc, pVtxData);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBufOgl::Bind() const
	{
		glBindVertexArray(m_unRIdVA);
		glBindBuffer(GL_ARRAY_BUFFER, m_unRIdVB);
		for (UInt32 ati = 0; ati < m_bufLayout.BufElems.size(); ati++)
		{
			const BufferElement& rBufElem = m_bufLayout.BufElems[ati];
			glVertexAttribPointer(ati, rBufElem.unCount,
				rBufElem.sdType, rBufElem.bNormalized ? GL_TRUE : GL_FALSE,
				m_bufLayout.unStride, reinterpret_cast<void*>(rBufElem.unOffset));
			glEnableVertexAttribArray(ati);
		}
	}
	void VertexBufOgl::Unbind() const
	{
		for (UInt32 ati = 0; ati < m_bufLayout.BufElems.size(); ati++)
		{
			glDisableVertexAttribArray(ati);
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
// IndexBufferOgl
namespace NW
{
	IndexBufOgl::IndexBufOgl(UInt32 unIndCount) :
		m_unRId(0), m_unIndCount(unIndCount)
	{
		glGenBuffers(1, &m_unRId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unRId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, unIndCount * sizeof(UInt32), nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	IndexBufOgl::IndexBufOgl(UInt32 unIndCount, UInt32* pIndData) :
		m_unRId(0), m_unIndCount(unIndCount)
	{
		glGenBuffers(1, &m_unRId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unRId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, unIndCount * sizeof(UInt32), pIndData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	IndexBufOgl::~IndexBufOgl()
	{
		glDeleteBuffers(1, &m_unRId);
	}

	// Interface Methods
	void IndexBufOgl::SetData(UInt32 unIndCount, UInt32* pIndData, Size szOffset)
	{
		m_unIndCount += unIndCount;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unRId);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, szOffset, unIndCount * sizeof(UInt32), pIndData);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void IndexBufOgl::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unRId);
	}
	void IndexBufOgl::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
// VertexBufCout
namespace NW
{
	VertexBufCout::VertexBufCout(UInt32 unSizeInBytes)
	{
		m_daVData.reserve(unSizeInBytes);
	}
	VertexBufCout::VertexBufCout(UInt32 unSizeInBytes, void* pVData)
	{
		m_daVData.reserve(unSizeInBytes);
		memcpy(&m_daVData[0], pVData, unSizeInBytes * sizeof(char));
	}
	VertexBufCout::~VertexBufCout()
	{
	}

	// Interface Methods
	void VertexBufCout::SetData(UInt unSizeInBytes, void* pVData, UInt unOffsetSize)
	{
		if (unSizeInBytes > GetSize())
		{
			NW_ERR("Too much bytes!");
			return;
		}
		for (UInt32 i = unOffsetSize; i < unSizeInBytes; i++)
			m_daVData.push_back(((char*)(pVData))[i]);
	}

	void VertexBufCout::Bind() const
	{
	}
	void VertexBufCout::Unbind() const
	{
	}
}
// IndexBufferCout
namespace NW
{
	IndexBufCout::IndexBufCout(UInt32 unCount)
	{
		m_unIData.reserve(unCount);
	}
	IndexBufCout::IndexBufCout(UInt32 unCount, UInt32* pIndices)
	{
		m_unIData.reserve(unCount);
		for (UInt32 i = 0; i < unCount; i++)
			m_unIData.push_back(pIndices[i]);
	}
	IndexBufCout::~IndexBufCout()
	{
	}

	// Interface Methods
	void IndexBufCout::SetData(UInt32 unCount, UInt32* pIndices, UInt32 unOffsetSize)
	{
		if (unCount > GetCount())
		{
			NW_ERR("Too much indices!");
			return;
		}
		for (UInt32 i = unOffsetSize; i < unCount; i++)
			m_unIData.push_back(pIndices[i]);
	}
	void IndexBufCout::Bind() const
	{
	}
	void IndexBufCout::Unbind() const
	{
	}
}
#endif // NW_GRAPHICS