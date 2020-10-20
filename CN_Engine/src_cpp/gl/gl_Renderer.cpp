#include <cn_pch.hpp>
#include <gl/gl_Renderer.h>

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
			CN::LoadMaster::get().loadIt_bin("resources/.ddt", m_indices, sizeof(UInt) * 5);

			CN_LOG("GL::RENDERER::DESTRUCTOR", "Renderer has been created");
		}
		Renderer::~Renderer()
		{

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