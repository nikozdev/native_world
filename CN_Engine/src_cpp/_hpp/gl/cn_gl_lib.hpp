#ifndef CN_GL_LIB_H
#define CN_GL_LIB_H

#include <cn_core.hpp>
#include <cn_pch.hpp>
#include <gl/gl_declarations.hpp>

namespace CN
{
	namespace GL
	{
		/// Return size in bytes of given GLenum type
		size_t getTypeSz(GLenum type);
		
		// Debug
		/// Clear GL error buffer
		void GL_clearError();
		/// Return suitable error message accordingly to glGetError()
		bool GL_log(const char* funcName,
			const char* file, int line);
	}
}

#endif