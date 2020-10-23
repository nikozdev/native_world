#ifndef GL_STRUCTS_H
#define GL_STRUCTS_H

#include <cn_core.hpp>
#include <gl/cn_gl_lib.hpp>

namespace CN
{
	namespace GL
	{
		/// Vertex for 2D world
		/// --Contains xy coordinates for position
		/// --Contains uv coordinates for texture coordinates
		struct Vertex2
		{
			float vCoords[2];
			float uvs[2];

			Vertex2(float* position = nullptr, float* texCoord = nullptr) :
				vCoords{ 0.0f, 0.0f }, uvs{ 0.0f, 0.0f } {}
		};
		/// Vertex for 3D world
		/// --Contains xyz coordinates for position and normal
		/// --Contains uv coordinates for texture coordinates
		struct Vertex3
		{
			float vCoords[3];
			float normal[3];
			float uvs[2];

			Vertex3(float* position = nullptr,
				float* normalVec = nullptr, float* texCoord = nullptr) :
				vCoords{ 0.0f, 0.0f, 0.0f },
				normal{ 0.0f, 0.0f, 0.0f }, uvs{0.0f, 0.0f} {}
		};
		/// VertexAttrib struct
		/// --Means the elements in the OpenGL shader layout
		/// --Whenever we create a pointer to the VBO data, we need to know it's parameters
		/// --glVertexAttribArrayPointer
		/// -->(layoutNum, countOfAttributes, type, normalization, sizeof(stride))
		struct CN_API VertexAttrib
		{
			UInt count;
			GLenum type;
			UChar normalized;

			VertexAttrib(UInt elementsCount = 0, GLenum newType = GL_FLOAT, UChar normalize = GL_FALSE) :
				count(elementsCount), type(newType), normalized(normalize) {}
		};
		/// BufferLayout class
		/// --Layout size and stride tracker for GL buffers
		struct CN_API BufferLayout
		{
		public:
			BufferLayout() : m_stride(0) {}
			
			// Accessors
			inline VertexAttrib* getAttrib(int index) { return m_attribs[index]; }
			inline size_t getStride() { return m_stride; }
			// Modificators
			void addAttrib(VertexAttrib& attribute);
		private:
			/// Stride grows with every new attribute
			size_t m_stride;
			/// Layout points to all vbo's attributes that they have
			std::vector<VertexAttrib*> m_attribs;
		};
		/// RenderLayer structure
		/// --It needs to draw in 2D and 2.5D with depth sorting
		/// --Every sprite has to be on the certain layer
		struct RenderLayer2D
		{
			// Tile_Y* wallTiles;
			// Tile_XZ* groundTiles;
			Sprite* backObjects;
			Sprite* faceObjects;
		};
	}
}

#endif