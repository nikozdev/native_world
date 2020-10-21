#ifndef GL_TYPES_H
#define GL_TYPES_H

#include <cn_core.hpp>
#include <cn_pch.hpp>
#include <gl/gl_declarations.hpp>

namespace CN
{
	namespace GL
	{
		/// VertexBuffer class
		/// --Wrapping for VertexArrayObject of OpenGL
		class CN_API VertexArr
		{
		public:
			VertexArr();
			~VertexArr();

			void addBuffer(GLBuffer& bufferObj);

			void bind();
			void unbind();
		private:
			bool m_isBound;
			bool m_hasIndex;
			GLuint m_id;

			std::vector<VertexBuf*> m_VBuffPtrs;
			IndexBuf* m_IBufferPtr;
			BufferLayout m_layout;
		};
		/// GLBuffer interface for OpenGL buffer objects
		/// --Has id and generates buffer by this ID in constructor
		/// --Has data type(static/dynamic), counter of data elements and general data size
		/// --Wraps binding functions of OpenGL
		class CN_API GLBuffer
		{
		public:
			enum class DataDrawTypes {
				STATIC = GL_STATIC_DRAW,
				DYNAMIC = GL_DYNAMIC_DRAW,
				STREAM = GL_STREAM_DRAW
			};
		public:
			GLBuffer();
			virtual ~GLBuffer();

			/// Load data of some type for bufferObject by DataDrawType
			virtual void setData(void* dataPtr, UInt count, GLenum type,
				DataDrawTypes dataType) = 0;
			/// Call glBufferSubData() for buffer object.
			/// --Works only with DYNAMIC data drawing type
			/// --On the 21.10.2020 can't load data with offsets
			virtual void updateData(void* dataPtr, UInt count) = 0;

			virtual void bind() = 0;
			virtual void unbind() = 0;

			inline virtual GLuint getCount() { return m_count; }
			inline virtual size_t getSize() { return m_bytes; }
			inline virtual GLenum getDataType() { return m_dataType; }
			inline virtual DataDrawTypes getDrawType() { return m_drawType; }
			/// Return size in bytes of given GLenum type
			static size_t getTypeSz(GLenum type)
			{
				switch (type)
				{
				case GL_FLOAT: return 4;
				case GL_UNSIGNED_INT: return 4;
				case GL_UNSIGNED_BYTE: return 1;
				default: return sizeof(type);
				}
			}
		protected:
			GLuint m_id;
			bool m_isBound;
			
			GLuint m_count;
			size_t m_bytes;
			DataDrawTypes m_drawType;
			GLenum m_dataType;
		};
		/// VertexBuffer class <- GLBuffer
		/// --Wrapping for VertexBufferObject of OpenGL
		/// --Can be loaded dynamically
		class CN_API VertexBuf : public GLBuffer
		{
		public:
			VertexBuf();
			~VertexBuf();

			void setData(void* dataPtr, UInt count, GLenum type,
				GLBuffer::DataDrawTypes dataType) override;
			void updateData(void* dataPtr, UInt count) override;

			virtual void bind() override;
			virtual void unbind() override;

			std::vector<VertexAttrib>* getVertexAttribs() { return &m_VAttribs; }
		private:
			std::vector<VertexAttrib> m_VAttribs;
		};
		/// IndexBuffer class <- GLBuffer
		/// --Wrapping for ElementVertexObject of OpenGL
		class CN_API IndexBuf : public GLBuffer
		{
		public:
			IndexBuf();
			~IndexBuf();

			void setData(void* dataPtr, UInt count, GLenum type,
				GLBuffer::DataDrawTypes dataType) override;
			
			void updateData(void* dataPtr, UInt count) override;

			virtual void bind() override;
			virtual void unbind() override;
		private:
		};
		/// BufferLayout class
		/// --Layout size and stride tracker for GL buffers
		class CN_API BufferLayout
		{
		public:
			BufferLayout(): m_stride(0) {}
			inline void addAttrib(BufferData& bufferData, UInt count) { m_elements.push_back(bufferData); }
			inline std::vector<BufferData>* getElems() { return &m_elements; }
			inline size_t getStride() { return m_stride; }
		private:
			size_t m_stride;
			std::vector<BufferData> m_elements;
		};
	}
}

#endif