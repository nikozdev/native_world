#include <cn_pch.hpp>
#include <gl/cn_gl_lib.hpp>
#include <extern/GLAD/glad.h>

namespace CN
{
	namespace GL
	{
		size_t getTypeSz(GLenum type)
		{
			switch (type)
			{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
			default: return sizeof(type);
			}
		}
		void GL_clearError()
		{//glGetError gets last message and clears errorLog
			while (glGetError() != GL_NO_ERROR);
		}
		bool GL_log(const char* funcName, const char* file, int line)
		{// Get error and match error message for it
			GLenum errorCode;
			while ((errorCode = glGetError()) != GL_NO_ERROR)
			{
				std::string error;
				switch (errorCode)
				{
				case GL_INVALID_ENUM:
					error = "INVALID_ENUM";
					break;
				case GL_INVALID_VALUE:
					error = "INVALID_VALUE";
					break;
				case GL_INVALID_OPERATION:
					error = "INVALID_OPERATION";
					break;
				case GL_STACK_OVERFLOW:
					error = "STACK_OVERFLOW";
					break;
				case GL_STACK_UNDERFLOW:
					error = "STACK_UNDERFLOW";
					break;
				case GL_OUT_OF_MEMORY:
					error = "OUT_OF_MEMORY";
					break;
				case GL_INVALID_FRAMEBUFFER_OPERATION:
					error = "INVALID_FRAMEBUFFER_OPERATION";
					break;
				}
				printf("GL::LOG::%s::LINE_%d:\n%s\n",
					funcName, line, error.c_str());
				return false;
			}
			return true;
		}

	}
}