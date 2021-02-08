#include <gfx_pch.hpp>
#include "gfx_tools.h"

#pragma warning (disable: 4312)

#if (GFX_GAPI & GFX_GAPI_OGL)
#include <gfx_loader.h>
// Functions
namespace NW
{
	void OglClearErr()
	{// glGetError gets last message and clears errorLog
		while (glGetError() != GL_NO_ERROR);
	}

	bool OglErrLog(const char* funcName, const char* file, int line)
	{// Get error and match error message for it
		UInt32 errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case GFX_INVALID_ENUM:
				error = "INVALID_ENUM";
				break;
			case GFX_INVALID_VALUE:
				error = "INVALID_VALUE";
				break;
			case GFX_INVALID_OPERATION:
				error = "INVALID_OPERATION";
				break;
			case GFX_STACK_OVERFLOW:
				error = "STACK_OVERFLOW";
				break;
			case GFX_STACK_UNDERFLOW:
				error = "STACK_UNDERFLOW";
				break;
			case GFX_OUT_OF_MEMORY:
				error = "OUT_OF_MEMORY";
				break;
			}
			printf("GFX_ERR_LOG::FILE_%s::LINE_%d:\n%s\n",
				funcName, line, error.c_str());
			return false;
		}
		return true;
	}

	Int32 OglErrLogShader(ShaderTypes shaderType, UInt32 shaderID)
	{
		if (shaderID == 0) return NW_ERR_UNKNOWN_ID;
		Int32 success = 0;
		Int32 errLogSize = 0;
		String errLog;
		std::string strType = shaderType == GFX_SHADER_VERTEX ?
			"SHADER_VERTEX" : shaderType == GFX_SHADER_PIXEL ?
			"SHADER_PIXEL" : "SHADER_PROG";
		if (shaderType != ST_SHADER)
		{
			glGetShaderiv(shaderID, GFX_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &errLogSize);
				errLog.resize(errLogSize);

				glGetShaderInfoLog(shaderID, errLogSize, NULL, &errLog[0]);
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
				NW_ERR(&errLog[0]);
				return NW_ERR_SHADER_LINK;
			}
		}
		return GFX_OK;
	}
}
#endif // GFX_GAPI

#if (GFX_GAPI & GFX_GAPI_DX)
#endif // GFX_GAPI