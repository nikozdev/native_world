#ifndef GL_TEXT_RENDERER_H
#define GL_TEXT_RENDERER_H

#include <cn_pch.hpp>
#include "gui_types.h"

#include <gl/cn_gl_lib.hpp>

namespace CN
{
	namespace GL
	{
		namespace GUI
		{
			/// TextRenderer class
			/// --Can draw glyphs and texts
			class CN_API TextRenderer
			{
			public:
				// Constructor&Destructor
				TextRenderer();
				~TextRenderer();

				// Accessors
			private: // Fields
				VertexArr* m_va;
				VertexBuf* m_vb;
				IndexBuf* m_ib;
				
				Shader* m_shader;
				Font m_font;
			private: // Methods
				/// 
				bool setFont(const std::string& path);
				/// 
				void drawLine(const std::string& text,
					int posX, int posY, float scale, float color[4]);
			};
		}
	}
}

#endif