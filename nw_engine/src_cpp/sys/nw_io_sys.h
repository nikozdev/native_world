#ifndef NW_IO_SYSTEM_H
#define NW_IO_SYSTEM_H
#include <lib/nw_io_lib.h>

#include <nw_decl.hpp>

namespace NW
{
	/// InputOutpuSystem static class
	class NW_API IOSys
	{
		friend class EvSys;
	public:
		static CursorState Cursor;
		static double xScrollDelta, yScrollDelta;
	public:
		// --getters
		static inline bool GetKeyPressed(UInt16 unKeyId) { return s_bsKeys[unKeyId].bPressed; }
		static inline bool GetKeyReleased(UInt16 unKeyId) { return s_bsKeys[unKeyId].bReleased; }
		static inline bool GetKeyHeld(UInt16 unKeyId) { return s_bsKeys[unKeyId].bHeld; }
		static inline bool GetMsButtonPressed(UInt16 unButtonId) { return s_bsMsBtns[unButtonId].bPressed; }
		static inline bool GetMsButtonReleased(UInt16 unButtonId) { return s_bsMsBtns[unButtonId].bReleased; }
		static inline bool GetMsButtonHeld(UInt16 unButtonId) { return s_bsMsBtns[unButtonId].bHeld; }
		
		// --core_methods
		static void Update();

		static void UpdateKeyboard();
		static void UpdateMouseButtons();
	private:
		static ButtonState s_bsKeys[NW_KEY_LAST];
		static ButtonState s_bsMsBtns[NW_MS_BTN_LAST];
	};
}

#endif // NW_IO_SYSTEM_H