#ifndef NW_IO_LIB_H
#define NW_IO_LIB_H

namespace NW
{
	struct NW_API ButtonState
	{
		bool bPressed, bReleased;
		bool bHeld;
		bool bNew, bOld;
	};
	struct CursorState
	{
		double xHeld, yHeld;
		double xMove, yMove;
		double xMoveDelta, yMoveDelta;
		bool bBlocked;
	};
}

#endif	// NW_IO_LIB_H