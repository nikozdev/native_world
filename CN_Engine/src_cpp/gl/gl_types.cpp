#include <cn_pch.hpp>
#include <gl/gl_types.h>
#include <gl/gl_structs.h>

#include <extern/GLAD/glad.h>

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

		void VertexArr::addVBuffer(VertexBuf& vBuffer)
		{
			bind();
			if (!m_isBound)
			{ CN_ERR("The array isn't bound"); return; }
			vBuffer.bind();
			auto attribs = vBuffer.getVertexAttribs();
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
				GL_CALL( glVertexAttribPointer(i, attrib->count, GL_FLOAT, attrib->normalized,
					m_layout.getStride(), (void*)offset) );
				offset += attrib->count * getTypeSz(attrib->type);
			}
			vBuffer.unbind();
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

		// Buffer data for float
		void GLBuffer::setData(float* dataPtr, UInt count,
			GLBuffer::DataLoadTypes loadType)
		{
			m_data.bytes += count * sizeof(float);
			m_data.count += count;
			m_data.loadType = loadType;
			bind();
			if (!m_isBound) { CN_ERR("The buffer isn't bound"); return; }
			GL_CALL(glBufferData(m_data.bufferType, m_data.bytes, dataPtr, m_data.loadType));
			unbind();
		}
		void GLBuffer::updateData(float* dataPtr, UInt count)
		{
			bind();
			if (!m_isBound) { CN_ERR("The buffer isn't bound"); return; }
			if (m_data.loadType != DataLoadTypes::DYNAMIC)
			{
				CN_ERR("We don't have dynamic data. Let's change it.");
				setData(dataPtr, count, DataLoadTypes::DYNAMIC);
				return;
			}
			size_t bytes = count * sizeof(float);
			if (bytes > m_data.bytes) return;
			GL_CALL(glBufferSubData(m_data.bufferType, 0, bytes, dataPtr));
			unbind();
		}
		// Buffer data for UInt
		void GLBuffer::setData(UInt* dataPtr, UInt count,
			GLBuffer::DataLoadTypes loadType)
		{
			m_data.bytes += count * sizeof(UInt);
			m_data.count += count;
			m_data.loadType = loadType;
			bind();
			if (!m_isBound) { CN_ERR("The buffer isn't bound"); return; }
			GL_CALL(glBufferData(m_data.bufferType, m_data.bytes, dataPtr, m_data.loadType));
			unbind();
		}
		void GLBuffer::updateData(UInt* dataPtr, UInt count)
		{
			bind();
			if (!m_isBound) { CN_ERR("The buffer isn't bound"); return; }
			if (m_data.loadType != DataLoadTypes::DYNAMIC)
			{
				CN_ERR("We don't have dynamic data. Let's change it.");
				setData(dataPtr, count, DataLoadTypes::DYNAMIC);
				return;
			}
			size_t bytes = count * sizeof(UInt);
			GL_CALL(glBufferSubData(m_data.bufferType, 0, bytes, dataPtr));
			unbind();
		}
		// Buffer data for UChar
		void GLBuffer::setData(UChar* dataPtr, UInt count,
			GLBuffer::DataLoadTypes loadType)
		{
			m_data.bytes += count * sizeof(float);
			m_data.count += count;
			m_data.loadType = loadType;
			bind();
			if (!m_isBound) { CN_ERR("The buffer isn't bound"); return; }
			GL_CALL(glBufferData(m_data.bufferType, m_data.bytes, dataPtr, m_data.loadType));
			unbind();
		}
		void GLBuffer::updateData(UChar* dataPtr, UInt count)
		{
			bind();
			if (!m_isBound) { CN_ERR("The buffer isn't bound"); return; }
			if (m_data.loadType != DataLoadTypes::DYNAMIC)
			{
				CN_ERR("We don't have dynamic data. Let's change it.");
				setData(dataPtr, count, DataLoadTypes::DYNAMIC);
				return;
			}
			size_t bytes = count * sizeof(UChar);
			GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, bytes, dataPtr));
			unbind();
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
	}
	namespace GL
	{
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
}