#ifndef GL_STRUCTS_H
#define GL_STRUCTS_H

#include <cn_core.hpp>

#include <gl/gl_declarations.hpp>

namespace CN
{
	namespace GL
	{
		/// Buffer Element struct
		/// --Means the elements in the OpenGL shader layout
		/// Any new data loaded into the  is a 
		struct BufferElement
		{
			UInt type;
			UInt size;
			UChar normald;

			BufferElement(UInt newType = GL_FLOAT,
				UInt newSize = sizeof(GL_FLOAT), UChar newNormald = GL_FALSE) :
				type(newType), size(newSize), normald(newNormald) {}
			
			static UInt getTypeSz(UInt type)
			{
				switch (type)
				{
				case GL_FLOAT: return 4;
				case GL_UNSIGNED_INT: return 4;
				case GL_UNSIGNED_BYTE: return 1;
				}
			}
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