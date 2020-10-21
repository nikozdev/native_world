#include <cn_pch.hpp>
#include <gl/gl_types.h>

#include <extern/GLEW/glew.h>

namespace CN
{
	namespace GL
	{
		VertexArr::VertexArr() :
			m_isBound(false), m_hasIndex(false)
		{
			glGenVertexArrays(1, &m_id);
		}
		VertexArr::~VertexArr()
		{
			glDeleteVertexArrays(1, &m_id);
		}

		void VertexArr::addBuffer(GLBuffer& bufferObj)
		{
			if (auto vBufPtr = dynamic_cast<VertexBuf*>(&bufferObj))
			{
				m_layout->addBuffer();
				bind();
				glEnableVertexArrayAttrib(m_id, 0);
				glVertexAttribPointer();
				unbind();
			}
			else if (auto iBufPtr = dynamic_cast<IndexBuf*>(&bufferObj))
			{
				if (m_hasIndex)
				{
					CN_ASSERT(!m_hasIndex, "IndexBuffer is already bound to this vertexArray");
					return;
				}
				;
				bind();
				vBufPtr->bind();
				unbind();
				iBufPtr->unbind();
				m_hasIndex = true;
			}
		}

		void VertexArr::bind()
		{
			if (!m_isBound) return;
			if (m_hasIndex) 
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
		GLBuffer::GLBuffer() :
			m_isBound(false), m_bytes(0), m_count(0),
			m_dataType(0), m_drawType(DataDrawTypes::STATIC)
		{
			glGenBuffers(1, &m_id);
		}
		GLBuffer::~GLBuffer()
		{
			glDeleteBuffers(1, &m_id);
		}

		// VertexBuffer
		VertexBuf::VertexBuf() : GLBuffer()
		{
		}
		VertexBuf::~VertexBuf()
		{
		}
		void VertexBuf::setData(void* dataPtr, UInt count, GLenum type,
			GLBuffer::DataDrawTypes dataType)
		{
			if (m_dataType != 0)
			{
				CN_ASSERT((m_dataType == 0), "This buffer is set already");
			}
			m_dataType = static_cast<GLenum>(dataType);
			bind();
			if (!m_isBound) return;
			m_bytes = getTypeSz(type) * count;
			glBufferData(GL_ARRAY_BUFFER, m_bytes, dataPtr, (GLenum)dataType);
			unbind();
		}

		void VertexBuf::updateData(void* dataPtr, UInt count)
		{
			bind();
			if (!m_isBound) return;
			size_t bytes = GLBuffer::getTypeSz(m_dataType) * count;
			if (bytes > m_bytes) return;
			glBufferSubData(GL_ARRAY_BUFFER, 0, bytes, dataPtr);
			unbind();
		}

		void VertexBuf::bind()
		{
			if (!m_isBound) return;
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
			m_isBound = true;
		}

		void VertexBuf::unbind()
		{
			if (!m_isBound) return;
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			m_isBound = false;
		}

		// IndexBuffer
		IndexBuf::IndexBuf() : GLBuffer()
		{
		}
		IndexBuf::~IndexBuf()
		{
		}

		void IndexBuf::setData(void* dataPtr, UInt count, GLenum type,
			GLBuffer::DataDrawTypes dataType)
		{
			if (m_dataType != 0)
			{
				CN_ASSERT((m_dataType == 0), "This buffer is already set");
			}
			m_dataType = static_cast<GLenum>(dataType);
			m_count = count;
			bind();
			if (!m_isBound) return;
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_bytes, dataPtr, (GLenum)dataType);
			unbind();
		}

		void IndexBuf::updateData(void* dataPtr, UInt count)
		{
			bind();
			if (!m_isBound) return;
			if (m_drawType != DataDrawTypes::DYNAMIC)
			{
				CN_ASSERT((m_drawType == DataDrawTypes::DYNAMIC),
					"We don't have dynamic data. Let's change it.");
				setData(dataPtr, count, m_dataType, DataDrawTypes::DYNAMIC);
			}
			size_t bytes = getTypeSz(m_dataType) * count;
			if (bytes > m_bytes) return;
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_bytes, dataPtr);
			unbind();
		}

		void IndexBuf::bind()
		{
			if (m_isBound) return;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
			m_isBound = true;
		}
		void IndexBuf::unbind()
		{
			if (!m_isBound) return;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			m_isBound = false;
		}
	}
}