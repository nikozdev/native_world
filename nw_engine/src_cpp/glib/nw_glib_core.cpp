#include <nw_pch.hpp>
#include "nw_glib_core.h"

#include <sys/nw_mem_sys.h>

#pragma warning (disable: 4312)

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
		return NW_OK;
	}
}
#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
#endif	// NW_GRAPHICS