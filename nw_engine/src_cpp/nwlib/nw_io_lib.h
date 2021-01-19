#ifndef NW_IO_LIB_H
#define NW_IO_LIB_H

#include <nw_core.hpp>
#include <nw_decl.hpp>

namespace NW
{
	struct NW_API ButtonState
	{
		bool bPressed = false, bReleased = false;
		bool bHeld = false;
		bool bNew = false, bOld = false;
	};
	struct NW_API KeyboardState
	{
		ButtonState bsKeys[KC_COUNT]{ 0 };
		InputModes iMode = IM_KEYBOARD_NORMAL;
	};
	struct NW_API MouseState
	{
		double xHeld = 0.0, yHeld = 0.0;
		double xMove = 0.0, yMove = 0.0;
		double xMoveDelta = 0.0, yMoveDelta = 0.0;
		double xScroll = 0.0, yScroll = 0.0;
		ButtonState bsButtons[MB_BUTTON_COUNT]{ 0 };
		InputModes iMode = IM_CURSOR_NORMAL;
	};
}

#endif	// NW_IO_LIB_H