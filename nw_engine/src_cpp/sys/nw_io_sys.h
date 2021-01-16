#ifndef NW_IO_SYSTEM_H
#define NW_IO_SYSTEM_H
#include <nwlib/nw_io_lib.h>

#include <nw_decl.hpp>

namespace NW
{
	enum InputModes {
		IM_CURSOR_NORMAL = 0, IM_CURSOR_DISABLED, IM_CURSOR_HIDDEN,
	};
	/// InputOutputSystem static class
	class NW_API IOSys
	{
	public:
		static CursorState s_Cursor;
		static double s_xScroll, s_yScroll;
		static ButtonState s_bsKeys[NW_KEY_LAST];
		static ButtonState s_bsMsBtns[NW_MS_BTN_LAST];
	public:
		// --getters
		static inline bool GetKeyPressed(UInt16 unKeyId) { return s_bsKeys[unKeyId].bPressed; }
		static inline bool GetKeyReleased(UInt16 unKeyId) { return s_bsKeys[unKeyId].bReleased; }
		static inline bool GetKeyHeld(UInt16 unKeyId) { return s_bsKeys[unKeyId].bHeld; }
		static inline bool GetMsButtonPressed(UInt16 unButtonId) { return s_bsMsBtns[unButtonId].bPressed; }
		static inline bool GetMsButtonReleased(UInt16 unButtonId) { return s_bsMsBtns[unButtonId].bReleased; }
		static inline bool GetMsButtonHeld(UInt16 unButtonId) { return s_bsMsBtns[unButtonId].bHeld; }
		// --setters
		static void SetInputMode(InputModes IMode);
		
		// --core_methods
		static void Update();
	private:
		static inline void UpdateKeyboard();
		static inline void UpdateMouseButtons();
	};
}

#endif // NW_IO_SYSTEM_H