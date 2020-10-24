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
			/// Draw all the data due to
			/// VertexArray and IndexBuffer that is loaded in VetexBuffer object
			void draw();
			void clear();

			// Static methods
			static void draw(VertexArr& va, IndexBuf& ib, Shader& shader);
			static void draw(GraphicsObj& object);
			static void clear(float* colorRGBA);
		private: // OpenGL types
			VertexArr* m_va;
			IndexBuf* m_ib;
			VertexBuf* m_vb;

			Shader* m_shader;
			Rectangle* m_rect;
		private: // Default data
			float m_globalColor[4];
		};
	}
}

#endif