#include <cn_pch.hpp>
#include <gl/gl_Renderer.h>
#include <gl/gl_types.h>
#include <gl/gl_structs.h>

#include <cn/cn_LoadMaster.h>

#include <extern/GLFW/glfw3.h>

namespace CN
{
	// Renderer
	namespace GL
	{
		// Constructors&Destructor
		Renderer::Renderer() :
			m_globalColor{ 0.0f, 0.0f, 0.0f, 1.0f }
		{
			m_va = new VertexArr();
			m_vb = new VertexBuf();
			m_ib = new IndexBuf();

			m_vertices = new float[] {
				-0.5f, -0.5f, // Left-bottom
				0.5f, -0.5f, // Right-Bottom
				0.5f, 0.5f, // Right-upper
				-0.5f, 0.5f // Left-upper
			};
			m_indices = new UInt[] {
				0, 1, 2,
				2, 3, 0
			};

			CN_LOG("GL::RENDERER::DESTRUCTOR", "Renderer has been created");
		}
		Renderer::~Renderer()
		{
			delete m_va;
			delete m_vb;
			delete m_ib;

			if (m_indices) delete m_indices;
			if (m_vertices) delete m_vertices;

			CN_LOG("GL::RENDERER::DESTRUCTOR", "Renderer has been destroyed");
		}

		// Core functions
		void Renderer::clear()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(m_globalColor[0], m_globalColor[1],
				m_globalColor[2], m_globalColor[3]);
		}
		void Renderer::draw()
		{
			m_va->bind();
			m_ib->bind();
			m_
		}

		// Static functions
		void Renderer::draw(VertexArr& va, IndexBuf& ib, Shader& shader)
		{
		}
		void Renderer::clear(float* colorRGBA)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(colorRGBA[0], colorRGBA[1], colorRGBA[2], colorRGBA[3]);
		}
	}
}