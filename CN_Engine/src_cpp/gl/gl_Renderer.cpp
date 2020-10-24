#include <cn_pch.hpp>

#include <gl/gl_Renderer.h>
#include <gl/gl_Shader.h>
#include <gl/gl_types.h>
#include <gl/gl_structs.h>

#include <cn/cn_LoadMaster.h>
#include <extern/GLEW/glew.h>

namespace CN
{
	// Renderer
	namespace GL
	{
		// Constructors&Destructor
		Renderer::Renderer() :
			m_globalColor{ 0.0f, 0.2f, 0.3f, 1.0f }
		{
			m_shader = new Shader("F:\\dev\\CheerNik\\CN_Engine\\shaders\\core_2d_vs.lua",
				"F:\\dev\\CheerNik\\CN_Engine\\shaders\\core_2d_ps.lua");
			m_rect = new Rectangle();
			m_rect->setShaderPtr(*m_shader);
		
			CN_LOG("Renderer has been created");
		}
		Renderer::~Renderer()
		{
			if(m_va)delete m_va;
			if(m_vb)delete m_vb;
			if(m_ib)delete m_ib;
			if(m_shader)delete m_shader;
			if(m_rect)delete m_rect;

			CN_LOG("Renderer has been destroyed");
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
			if (false)
			{
				m_shader->use();
				m_va->bind();
				GL_CALL(glDrawElements(GL_TRIANGLES, m_ib->getBufferData()->count, GL_UNSIGNED_INT, nullptr));
				m_va->unbind();
				m_shader->stopUse();
			}
			else
			{
				m_rect->draw();
			}
		}

		// Static functions
		void Renderer::draw(VertexArr& va, IndexBuf& ib, Shader& shader)
		{
			if (false)
			{
				shader.use();
				va.bind();
				GL_CALL(glDrawElements(GL_TRIANGLES, ib.getBufferData()->count, GL_UNSIGNED_INT, nullptr));
				va.unbind();
				shader.stopUse();
			}
		}
		void Renderer::clear(float* colorRGBA)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(colorRGBA[0], colorRGBA[1], colorRGBA[2], colorRGBA[3]);
		}
	}
}