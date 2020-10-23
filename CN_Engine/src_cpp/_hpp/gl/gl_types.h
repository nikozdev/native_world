#ifndef GL_TYPES_H
#define GL_TYPES_H

#include <cn_core.hpp>
#include <cn_pch.hpp>
#include <gl/cn_gl_lib.hpp>
#include <gl/gl_structs.h>

namespace CN
{
	namespace GL
	{
		/// GLBuffer interface for OpenGL buffer objects
		/// --Has id and generates buffer by this ID in constructor
		/// --Has data type(static/dynamic), counter of data elements and general data size
		/// --Wraps binding functions of OpenGL
		class CN_API GLBuffer
		{
		public:
			/// This enumeration confines GLenumeration to get only draw types
			enum DataLoadTypes : GLenum {
				NO_LTYPE = GL_FALSE,
				STATIC = GL_STATIC_DRAW,
				DYNAMIC = GL_DYNAMIC_DRAW,
				STREAM = GL_STREAM_DRAW
			};
			enum BufferTypes : GLenum {
				NO_BTYPE = GL_FALSE,
				VERTEX = GL_ARRAY_BUFFER, INDEX = GL_ELEMENT_ARRAY_BUFFER
			};
			struct BufferData {
				UInt count;
				size_t bytes;
				DataLoadTypes loadType;
				BufferTypes bufferType;

				BufferData(UInt _count, size_t size_in_bytes,
					DataLoadTypes load_type, BufferTypes buffer_type) :
					count(_count), bytes(size_in_bytes),
					loadType(load_type), bufferType(buffer_type) {}
			};
		public:
			virtual ~GLBuffer();

			/// Load data of some type for bufferObject by DataDrawType
			virtual void setData(float* dataPtr, UInt count,
				DataLoadTypes loadType);
			virtual void setData(UInt* dataPtr, UInt count,
				DataLoadTypes loadType);
			virtual void setData(UChar* dataPtr, UInt count,
				DataLoadTypes loadType);
			/// Call glBufferSubData() for buffer object.
			/// --Works only with DYNAMIC data drawing type
			/// --On the 21.10.2020 can't load data with offsets
			virtual void updateData(float* dataPtr, UInt count);
			virtual void updateData(UInt* dataPtr, UInt count);
			virtual void updateData(UChar* dataPtr, UInt count);

			virtual void bind();
			virtual void unbind();

			inline virtual BufferData* getBufferData() { return &m_data; }
		protected:
			/// Derrived classes have to define GLenumeration-type of the buffer
			GLBuffer(BufferTypes bufferType);

			GLuint m_id;
			bool m_isBound;

			BufferData m_data;
		};
		/// VertexBuffer class <- GLBuffer
		/// --Wrapping for VertexBufferObject of OpenGL
		/// --Can be loaded dynamically
		class CN_API VertexBuf : public GLBuffer
		{
		public:
			VertexBuf();
			~VertexBuf();

			// Modificators
			inline void addAttrib(UInt count, GLenum type, UChar normalized)
			{ m_VAttribs.push_back({ count, type, normalized }); };
			// Accessors
			inline std::vector<VertexAttrib>* getVertexAttribs() { return &m_VAttribs; }
		private:
			std::vector<CN_API VertexAttrib> m_VAttribs;
		};
		/// IndexBuffer class <- GLBuffer
		/// --Wrapping for ElementVertexObject of OpenGL
		class CN_API IndexBuf : public GLBuffer
		{
		public:
			IndexBuf();
			~IndexBuf();
		private:
		};
		/// VertexBuffer class
		/// --Wrapping for VertexArrayObject of OpenGL
		class CN_API VertexArr
		{
		public:
			VertexArr();
			~VertexArr();

			void addVBuffer(VertexBuf& vBuffer);
			void setIBuffer(IndexBuf& iBuffer);

			void bind();
			void unbind();
		private:
			bool m_isBound;
			GLuint m_id;

			std::vector<VertexBuf*> m_VBuffPtrs;
			IndexBuf* m_IBufferPtr;
			BufferLayout m_layout;
		};
	}
}

#endif