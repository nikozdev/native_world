#ifndef GUI_DECL_HPP
#define GUI_DECL_HPP

#include <nw_core.hpp>

namespace NW
{
	// -- Types
	class NW_API GuiFont;
	class NW_API GuiGlyph;
	// -- Widgets
	class NW_API AGuiWidget;
	class NW_API GuiText;
	class NW_API GuiButton;
	class NW_API GuiImage;
	// -- Drawers
	template <typename GuiOfType> struct NW_API AGuiOf;
	struct  NW_API GuiOfEngine;
	struct  NW_API GuiOfDataSys;
	struct  NW_API GuiOfMemSys;
	struct  NW_API GuiOfTimeSys;
	struct  NW_API GuiOfLogSys;
	struct  NW_API GuiOfRandSys;

	struct NW_API GuiOfCodeEditor;
	struct NW_API GuiOfSpriteEditor;
	struct NW_API GuiOfSceneEditor;
}

#endif	//GUI_DECL_HPP