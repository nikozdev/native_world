#ifndef NW_DECL_HPP
#define NW_DECL_HPP

#include <nw_core.hpp>

namespace NW
{
	// --==<global>==--
	class NW_API CoreEngine;
	class NW_API CoreState;
	// --systems
	class NW_API IOSys;
	class NW_API TimeSys;
	class NW_API MemSys;
	class NW_API DataSys;
	class NW_API GuiSys;
	// --objects
	class NW_API ADataRes;
	// --==</global>==--

	// --==<events>==--
	struct NW_API AEvent;
	struct NW_API WindowEvent;
	struct NW_API MouseEvent;
	struct NW_API KeyboardEvent;
	// --==</events>==--
	
	// --==<enums>==--
	enum EventTypes : UInt32 {
		ET_NONE = 0,
		ET_WINDOW_CLOSE, ET_WINDOW_RESIZE, ET_WINDOW_MOVE, ET_WINDOW_FOCUS,
		ET_APP_TICK, ET_APP_UPDATE, ET_APP_RENDER,
		ET_MOUSE_MOVE, ET_MOUSE_SCROLL, ET_MOUSE_PRESS, ET_MOUSE_RELEASE,
		ET_KEY_RELEASE, ET_KEY_PRESS, ET_KEY_TYPE
	};
	enum KeyCodes : Char16 {
		KC_NONE = 0,
		KC_BELL = 7, // "\a" - sound signal
		KC_BACKSPACE = 8, // Step backward - can be used for two symbols in one
		KC_HTAB = 9, KC_ENTER = 10, KC_VTAB = 11, // Hor + Vert tabulation and EndLine
		KC_SHIFT = 16, KC_CTRL = 17, KC_PAUSE = 19, KC_CAPS_LOCK = 20, KC_ESCAPE = 27,
		KC_LEFT = 37, KC_UP = 38, KC_RIGHT = 39, KC_DOWN = 40, KC_INSERT = 45, KC_DEL = 46,
		KC_HOME = 36,
		KC_SPACE = 32, KC_EXCLAIM = 33, KC_HASH = 35,
		KC_K0 = 48, KC_K1 = 49, KC_K2, KC_K3, KC_K4, KC_K5, KC_K6, KC_K7, KC_K8, KC_K9 = 57,
		KC_COLON = 58, KC_SEMICOLON = 59, KC_LESSER = 60, KC_EQUAL = 61, KC_GREATER = 62,
		KC_A = 65, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I, KC_J, KC_K, KC_L, KC_M, KC_N, KC_O, KC_P = 80,
		KC_Q = 81, KC_R, KC_S, KC_T, KC_U, KC_V, KC_W, KC_X, KC_Y, KC_Z = 90,
		KC_a = 97, KC_b, KC_c, KC_d, KC_e, KC_f, KC_g, KC_h, KC_i, KC_j, KC_k, KC_l, KC_m, KC_n, KC_o, KC_p = 112,
		KC_q = 113, KC_r, KC_s, KC_t, KC_u, KC_v, KC_w, KC_x, KC_y, KC_z = 122,
		KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,
		KC_RECT_DITHER = 177, KC_QUAD = 254,
		KC_PGUP, KC_PGDN,
		KC_RETURN, KC_SCROLL,
		KC_NP_MUL, KC_NP_DIV, KC_NP_ADD, KC_NP_SUB, KC_NP_DECIMAL, KC_PERIOD,
		KC_COUNT = 255
	};
	// --==<enums>==--
}

#endif // NW_DECL_H