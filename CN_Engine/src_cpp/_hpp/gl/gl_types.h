#ifndef GL_TYPES_H
#define GL_TYPES_H

#include <cn_core.hpp>
#include <cn_pch.hpp>
#include <gl/cn_gl_lib.hpp>
#include <gl/gl_structs.h>

#include <extern/glm/mat4x4.hpp>
#include <extern/glm/vec3.hpp>
#include <extern/GLEW/glew.h>

namespace CN
{
	// OpenGL wrappers
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
			struct CN_API BufferData {
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
			template<typename T>
			void setData(T* dataPtr, UInt count,
				DataLoadTypes loadType)
			{
				m_data.bytes += count * sizeof(T);
				m_data.count += count;
				m_data.loadType = loadType;
				bind();
				if (!m_isBound) { CN_ERR("The buffer isn't bound"); return; }
				GL_CALL(glBufferData(m_data.bufferType, m_data.bytes, dataPtr, m_data.loadType));
				unbind();
			}
			/// Call glBufferSubData() for buffer object.
			/// --Works only with DYNAMIC data drawing type
			/// --At 21.10.2020 can't load data with offsets
			template<typename T>
			void updateData(T* dataPtr, UInt count)
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

			void setVBuffer(VertexBuf& vBuffer);
			void setIBuffer(IndexBuf& iBuffer);
			
			// Predicates
			inline bool pointsVBuffer(VertexBuf* vBuffer)
			{
				for (auto& bufferPtr : m_VBuffPtrs)
					if (bufferPtr == vBuffer)
						return true;
				return false;
			}

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
	// Drawing objects
	namespace GL
	{
		/// GraphycalObject interface
		/// --Interface for objects we can draw by single VAO, VBO
		class CN_API GraphicsObj
		{
		public:
			GraphicsObj(DimTypes dType);
			virtual ~GraphicsObj();

			virtual void draw();

			// Modificators
			inline void setCoordLoc(float x, float y, float z) { b_coord_l = MAT::Vec3(x, y, z); }
			inline void setRotateLoc(float x, float y, float z) { b_rotat_l = MAT::Vec3(x, y, z); }
			inline void setScaleLoc(float x, float y, float z) { b_scale_l = MAT::Vec3(x, y, z); }
			
			inline void setShaderPtr(Shader& shader) { b_shaderPtr = &shader; };

			// Accessors
			inline MAT::Vec3& getCoordLoc(float x, float y, float z) { return b_coord_l; }
			inline MAT::Vec3& getRotateLoc(float x, float y, float z) { return b_rotat_l; }
			inline MAT::Vec3& getScaleLoc(float x, float y, float z) { return b_scale_l; }
			inline DimTypes getDimType() const { return b_dimention; }
		protected: // Base methods
			template<typename T>
			void setDrawData(T* dataPtr, UInt count, UInt elemCount,
				GLBuffer::DataLoadTypes loadType)
			{
				if (typeid(T) == typeid(UInt))
				{
					b_vb.setData<T>(dataPtr, count, loadType);
					b_vb.addAttrib(elemCount, GL_UNSIGNED_INT, GL_FALSE);
					b_va.setVBuffer(b_vb);
				}
				else if (typeid(T) == typeid(float))
				{
					b_vb.setData<T>(dataPtr, count, loadType);
					b_vb.addAttrib(elemCount, GL_FLOAT, GL_FALSE);
					b_va.setVBuffer(b_vb);
				}
			}
			void setIndexData(UInt* dataPtr, UInt count,
				GLBuffer::DataLoadTypes loadType)
			{
				b_ib.setData<UInt>(dataPtr, count, loadType);
				b_va.setIBuffer(b_ib);
			}

			void updateModelMat();
			void updateUniforms();
		protected: // Base fields
			VertexArr b_va;
			VertexBuf b_vb;
			IndexBuf b_ib;

			Shader* b_shaderPtr;
			
			MAT::Vec3 b_coord_l;
			MAT::Vec3 b_rotat_l;
			MAT::Vec3 b_scale_l;
			glm::mat4 b_modelMat;

			DimTypes b_dimention;

			float* b_vData;
			UInt* b_indices;
			
			UInt b_texID;
		};
		/// Primitive interface : public GraphObject
		/// --Interface for drawing simple shapes
		class CN_API Primitive : public GraphicsObj
		{
		public:
			enum class PrimTypes {
				DEFAULT = 0, LINE = 1, TRIANGLE, RECTANGLE, POLY
			};
		public:
			Primitive(PrimTypes primType);
			virtual ~Primitive();
		
			// Modificators
			inline PrimTypes getPrimType() const { return b_primType; }
		protected:
			PrimTypes b_primType;
		};
		/// Rectangle class : public Primitive
		/// --4 angled primitive
		class CN_API Rectangle : public Primitive
		{
		public:
			Rectangle();
			~Rectangle();
		private:
		};
	}
}

#endif