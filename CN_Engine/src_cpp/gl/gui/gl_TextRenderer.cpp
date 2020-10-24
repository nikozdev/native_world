#include <cn_pch.hpp>
#include <gl/gui/gui_TextRenderer.h>
#include <gl/gl_structs.h>
#include <gl/gl_Shader.h>
#include <cn/cn_LoadMaster.h>

namespace CN
{
	namespace GL
	{
		// TextRenderer
		namespace GUI
		{
			TextRenderer::TextRenderer()
			{
				CN_LOG("TextRenderer has been created");
			}
			TextRenderer::~TextRenderer()
			{
				CN_LOG("TextRenderer has been destroyed");
			}
		}
	}
}