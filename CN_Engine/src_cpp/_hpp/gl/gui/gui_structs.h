#ifndef GUI_STRUCTS_H
#define GUI_STRUCTS_H

#include <cn_core.hpp>
#include <gl/cn_gl_lib.hpp>

namespace CN
{
	namespace GL
	{
		namespace GUI
		{
			/// Font struct
			/// --Contains all glyphs of some font
			/// --Can load bitmaps of fonts
			struct CN_API Font
			{
				struct CN_API Glyph
				{
					char symbol;
					Rectangle* rectPtr;

					Glyph(char character, Rectangle& rect) :
						symbol(character), rectPtr(&rect) {}
				};
			};
			/// TxtBlock struct
			struct CN_API TxtBlock
			{
			};
		}
	}
}

#endif