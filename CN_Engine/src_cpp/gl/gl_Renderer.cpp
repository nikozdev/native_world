#include <cn_pch.hpp>

#include <gl/gl_Renderer.h>
#include <gl/gl_Shader.h>
#include <gl/gl_types.h>
#include <gl/gl_structs.h>

#include <cn/cn_LoadMaster.h>

#include <extern/GLAD/glad.h>

namespace CN
{
	// Renderer
	namespace GL
	{
		// Constructors&Destructor
		Renderer::Renderer() :
			m_globalColor{ 0.0f, 0.2f, 0.3f, 1.0f }
		{
			//CN::LoadMaster::get().loadIt_bin("F://dev/CheerNik/CN_Engine/resources/color_data/default_color.bdt",
			//	(void*)m_globalColor, sizeof(m_globalColor));

			m_va = new VertexArr();
			m_vb = new VertexBuf();
			m_ib = new IndexBuf();
			m_shader = new Shader("shaders/core_2d_vs.lua", "shaders/core_2d_fs.lua");
			//CN::LoadMaster::get().loadIt_bin("F://dev/CheerNik/CN_Engine/resources/vertex_data/rect_vd_4_2.bdt", (void*)m_vertices, sizeof(m_vertices));
			//CN::LoadMaster::get().loadIt_bin("F://dev/CheerNik/CN_Engine/resources/vertex_data/rect_ind_4_2.bdt", (void*)m_indices, sizeof(m_vertices));

			m_ib->setData(m_indices, 6, GLBuffer::DYNAMIC);
			m_vb->setData(m_vertices, 8, GLBuffer::DYNAMIC);
			m_vb->addAttrib(2, GL_FLOAT, GL_FALSE);

			m_va->addVBuffer(*m_vb);
			m_va->setIBuffer(*m_ib);

			CN_LOG("Renderer has been created");
		}
		Renderer::~Renderer()
		{
			delete m_va;
			delete m_vb;
			delete m_ib;
			delete m_shader;

			//CN::LoadMaster::get().saveIt_bin("F://dev/CheerNik/CN_Engine/resources/vertex_data/rect_vd_4_2.bdt", (void*)m_vertices, sizeof(m_vertices));
			//CN::LoadMaster::get().saveIt_bin("F://dev/CheerNik/CN_Engine/resources/vertex_data/rect_ind_4_2.bdt", (void*)m_indices, sizeof(m_indices));
			//CN::LoadMaster::get().saveIt_bin("F://dev/CheerNik/CN_Engine/resources/color_data/default_color.bdt", (void*)m_globalColor, sizeof(m_globalColor));

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
			m_shader->use();
			m_va->bind();
			GL_CALL(glDrawElements(GL_TRIANGLES, m_ib->getBufferData()->count, GL_UNSIGNED_INT, nullptr));
			m_va->unbind();
			m_shader->stopUse();
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