#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include <cn_core.hpp>

#include <gl/gl_declarations.hpp>

namespace CN
{
	// Renderer
	namespace GL
	{
		/// Renderer class
		/// --Wraps opengl functions
		/// --Gets data and draws it
		/// --Can draw throught abstraction classes and VBO/VAO/EBO
		class CN_API Renderer
		{
		public: // Methods
			// Constructor&Destructor
			Renderer();
			~Renderer();

			// Core functions
			void clear();
			void draw();

			// Static methods
			static void draw(VertexArr& va, IndexBuf& ib, Shader& shader);
			static void clear(float* colorRGBA);
		private: // OpenGL types
			VertexArr* m_va;
			IndexBuf* m_ib;
			VertexBuf* m_vb;
		private: // Default data
			float m_globalColor[4];
			UInt* m_indices;
			float* m_vertices;
		};
	}
}

#endif