#ifndef NW_DECL_HPP
#define NW_DECL_HPP

#include <nw_core.hpp>

namespace NW
{
	// ========<Global>========
	class NW_API Engine;
	class NW_API EngineState;
	// -- Systems
	class NW_API EvSys;
	class NW_API IOSys;
	class NW_API TimeSys;
	class NW_API MemSys;
	class NW_API DataSys;
	class NW_API GuiSys;
	// -- Objects
	class NW_API ADataRes;
	// ========</Global>========

	// ========<Events>========
	struct NW_API AEvent;
	struct NW_API WindowEvent;
	struct NW_API MouseEvent;
	struct NW_API KeyboardEvent;
	// ========</Events>========
	
	// ========<Enums>========
	enum EventTypes : UInt32 {
		ET_NONE = 0,
		ET_WINDOW_CLOSE, ET_WINDOW_RESIZE, ET_WINDOW_MOVE, ET_WINDOW_FOCUS,
		ET_APP_TICK, ET_APP_UPDATE, ET_APP_RENDER,
		ET_MOUSE_MOVE, ET_MOUSE_SCROLL, ET_MOUSE_PRESS, ET_MOUSE_RELEASE,
		ET_KEY_RELEASE, ET_KEY_PRESS, ET_KEY_TYPE
	};
	enum class KeyCodes : Char16 {
		NONE = 0,
		BELL = 7, // "\a" - sound signal
		BACKSPACE = 8, // Step backward - can be used for two symbols in one
		HTAB = 9, ENTER = 10, VTAB = 11, // Hor + Vert tabulation and EndLine
		SHIFT = 16, CTRL = 17, PAUSE = 19, CAPS_LOCK = 20, ESCAPE = 27,
		LEFT = 37, UP = 38, RIGHT = 39, DOWN = 40, INSERT = 45, DEL = 46,
		HOME = 36,
		SPACE = 32, EXCLAIM = 33, HASH = 35,
		K0 = 48, K1 = 49, K2, K3, K4, K5, K6, K7, K8, K9 = 57,
		COLON = 58, SEMICOLON = 59, LESSER = 60, EQUAL = 61, GREATER = 62,
		A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P = 80,
		Q = 81, R, S, T, U, V, W, X, Y, Z = 90,
		a = 97, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p = 112,
		q = 113, r, s, t, u, v, w, x, y, z = 122,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		RECT_DITHER = 177, QUAD = 254,
		PGUP, PGDN,
		RETURN, SCROLL,
		NP_MUL, NP_DIV, NP_ADD, NP_SUB, NP_DECIMAL, PERIOD
	};
	// ========<Enums>========
}

#endif // NW_DECL_H