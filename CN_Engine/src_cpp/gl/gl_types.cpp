#include <cn_pch.hpp>
#include <gl/gl_types.h>
#include <gl/gl_structs.h>
#include <gl/gl_Shader.h>

#include <extern/glm/gtc/matrix_transform.hpp>
#include <extern/GLEW/glew.h>

namespace CN
{
	// VertexArray
	namespace GL
	{
		VertexArr::VertexArr() :
			m_isBound(false), m_IBufferPtr(nullptr)
		{
			glGenVertexArrays(1, &m_id);
		}
		VertexArr::~VertexArr()
		{
			glDeleteVertexArrays(1, &m_id);
		}

		void VertexArr::setVBuffer(VertexBuf& vBuffer)
		{
			bind();
			if (!m_isBound)
			{ CN_ERR("The array isn't bound"); return; }
			if (!pointsVBuffer(&vBuffer))
			{
				m_VBuffPtrs.push_back(&vBuffer);
			}
			for (auto& buffPtr : m_VBuffPtrs)
			{
				buffPtr->bind();
				auto attribs = buffPtr->getVertexAttribs();
				// Every next attribute will be pointed with offset
				size_t offset = 0;
				for (int i = 0; i < attribs->size(); i++)
				{// Add all attributes of VBO to VAO layout
					m_layout.addAttrib(attribs->at(i));
				}
				for (int i = 0; i < attribs->size(); i++)
				{// Config every attribute due to layout's attributes
					VertexAttrib* attrib = m_layout.getAttrib(i);
					GL_CALL(glEnableVertexAttribArray(i));
					GL_CALL(glVertexAttribPointer(i, attrib->count, attrib->type, attrib->normalized,
						m_layout.getStride(), (void*)offset));
					offset += attrib->count * getTypeSz(attrib->type);
				}
				buffPtr->unbind();
			}
			unbind();
		}

		void VertexArr::setIBuffer(IndexBuf& iBuffer)
		{
			bind();
			iBuffer.bind();
			unbind();
			iBuffer.unbind();
			m_IBufferPtr = &iBuffer;
		}

		void VertexArr::bind()
		{
			if (m_isBound) return;
			glBindVertexArray(m_id);
			m_isBound = true;
		}
		void VertexArr::unbind()
		{
			if (!m_isBound) return;
			glBindVertexArray(0);
			m_isBound = false;
		}
	}
	// GLBuffers
	namespace GL
	{
		// GLBuffer interface
		GLBuffer::GLBuffer(BufferTypes bufferType) :
			m_isBound(false), m_data(0, 0, NO_LTYPE, bufferType)
		{
			GL_CALL(glGenBuffers(1, &m_id));
		}
		GLBuffer::~GLBuffer()
		{
			glDeleteBuffers(1, &m_id);
		}

		// Binding/Unbinding
		void GLBuffer::bind()
		{
			if (m_isBound) return;
			GL_CALL(glBindBuffer(m_data.bufferType, m_id));
			m_isBound = true;
		}

		void GLBuffer::unbind()
		{
			if (!m_isBound) return;
			GL_CALL(glBindBuffer(m_data.bufferType, 0));
			m_isBound = false;
		}

		// VertexBuffer
		VertexBuf::VertexBuf() : GLBuffer(BufferTypes::VERTEX)
		{
		}
		VertexBuf::~VertexBuf()
		{
		}

		// IndexBuffer
		IndexBuf::IndexBuf() : GLBuffer(BufferTypes::INDEX)
		{
		}
		IndexBuf::~IndexBuf()
		{
		}
	}
	// GraphicsObject
	namespace GL
	{
		GraphicsObj::GraphicsObj(DimTypes dType) :
			b_dimention(dType),
			b_modelMat(glm::mat4(1.0f)),
			b_scale_l(MAT::Vec3(1.0f)),
			b_rotat_l(MAT::Vec3(0.0f)),
			b_coord_l(MAT::Vec3(0.0f))
		{
			switch (dType)
			{
			case DimTypes::_1D:
				break;
			case DimTypes::_2D:
				break;
			case DimTypes::_3D:
				break;
			default:
				//b_vData = nullptr;
				//b_indices = nullptr;
				break;
			}
			glGenTextures(1, &b_texID);
		}
		GraphicsObj::~GraphicsObj()
		{
			if (b_vData) delete[] b_vData;
			if (b_indices) delete[] b_indices;
			glDeleteTextures(1, &b_texID);
		}

		void GraphicsObj::draw()
		{
			b_shaderPtr->use();
			updateModelMat();
			updateUniforms();

			b_va.bind();
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, b_texID);
				GL_CALL(glDrawElements(GL_TRIANGLES, b_ib.getBufferData()->count, GL_UNSIGNED_INT, nullptr));
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			b_va.unbind();
			
			b_shaderPtr->stopUse();
		}

		void GraphicsObj::updateModelMat()
		{
			b_modelMat = glm::translate(b_modelMat, { b_coord_l.x, b_coord_l.y, b_coord_l.z });
			b_modelMat = glm::rotate(b_modelMat, MAT::getRadian(b_rotat_l.x), { b_rotat_l.x, 0.0f, 0.0f });
			b_modelMat = glm::rotate(b_modelMat, MAT::getRadian(b_rotat_l.y), { 0.0f, b_rotat_l.y, 0.0f });
			b_modelMat = glm::rotate(b_modelMat, MAT::getRadian(b_rotat_l.z), { 0.0f, 0.0f, b_rotat_l.z });
			b_modelMat = glm::scale(b_modelMat, { b_scale_l.x, b_scale_l.y, b_scale_l.z });
		}
		void GraphicsObj::updateUniforms()
		{
			b_shaderPtr->setInt("material.texture_diffuse", 0);
		}
	}
	// Primitives
	namespace GL
	{
		Primitive::Primitive(PrimTypes primType) :
			GraphicsObj(DimTypes::DEFAULT),
			b_primType(primType)
		{
			switch (b_primType)
			{
			case PrimTypes::LINE:
				b_dimention = DimTypes::_1D;
				break;
			case PrimTypes::TRIANGLE: case PrimTypes::RECTANGLE:
				b_dimention = DimTypes::_2D;
				b_dimention = DimTypes::_2D;
				break;
			case PrimTypes::POLY:
				b_dimention = DimTypes::_3D;
				break;
			}
		}
		Primitive::~Primitive()
		{
		}

		// Rectangle
		Rectangle::Rectangle() :
			Primitive(PrimTypes::RECTANGLE)
		{
			if (true)
			{
				b_vData = new float[] {
					-0.5f,	-0.5f,		0.0f, 0.0f,
					0.5f,	-0.5f,		1.0f, 0.0f,
					0.5f,	0.5f,		1.0f, 1.0f
					-0.5f,	0.5f,		0.0f, 1.0f
				};
				b_indices = new UInt[]{
					0, 1, 2,
					2, 3, 0
				};
				setIndexData(b_indices, 6, GLBuffer::DataLoadTypes::DYNAMIC);
				setDrawData<float>(&b_vData[0], 16, 2, GLBuffer::DataLoadTypes::DYNAMIC);
				//setDrawData<float>(&b_vData[7], 8, 2, GLBuffer::DataLoadTypes::DYNAMIC);
				UChar test_texture[]{
						32, 128, 64, 255,
						32, 64, 128, 128,
						32, 128, 64, 255,
						32, 64, 128, 128
				};
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, b_texID);
					GL_CALL(glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
					GL_CALL(glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
					GL_CALL(glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
					GL_CALL(glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
					GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, test_texture));
					glGenerateMipmap(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
		}
		Rectangle::~Rectangle()
		{
		}
	}
}