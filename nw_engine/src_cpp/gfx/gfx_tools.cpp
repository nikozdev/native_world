#include <nw_pch.hpp>
#include "gfx/gfx_tools.h"

#pragma warning (disable: 4312)

#if (defined NW_GAPI)
#if (NW_GAPI & NW_GAPI_OGL)
#include <gfx/gfx_loader.h>
// Functions
namespace NW
{
	// glGetError gets last message and clears errorLog
	void OglClearErr() { while (glGetError() != GL_NO_ERROR); }

	bool OglErrLog(const char* funcName, const char* file, int line)
	{// Get error and match error message for it
		UInt32 errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			String strErr;
			switch (errorCode) {
			case NW_INVALID_ENUM:
				strErr = "INVALID_ENUM";
				break;
			case NW_INVALID_VALUE:
				strErr = "INVALID_VALUE";
				break;
			case NW_INVALID_OPERATION:
				strErr = "INVALID_OPERATION";
				break;
			case NW_STACK_OVERFLOW:
				strErr = "STACK_OVERFLOW";
				break;
			case NW_STACK_UNDERFLOW:
				strErr = "STACK_UNDERFLOW";
				break;
			case NW_OUT_OF_MEMORY:
				strErr = "OUT_OF_MEMORY";
				break;
			}
			printf("GFX_ERR_LOG::FILE_%s::LINE_%d:\n%s\n", funcName, line, &strErr[0]);
			return false;
		}
		return true;
	}

	Int32 OglErrLogShader(ShaderTypes shaderType, UInt32 shaderID)
	{
		if (shaderID == 0) return NWL_ERR_UNKNOWN_ID;
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
				NWL_ERR(errLog);
				return NWL_ERR_SHADER_COMPILE;
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
				NWL_ERR(&errLog[0]);
				return NWL_ERR_SHADER_LINK;
			}
		}
		return NW_OK;
	}
}
#endif // NW_GAPI

#if (NW_GAPI & NW_GAPI_DX)

#endif

#endif	// NW_GAPI