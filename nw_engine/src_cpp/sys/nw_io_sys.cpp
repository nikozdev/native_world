#include <nw_pch.hpp>
#include "nw_io_sys.h"

#include <glib/gcontext/nw_window.h>

#include <sys/nw_ev_sys.h>

#if (NW_WINDOW & NW_WINDOW_GLFW)
#include <GLFW/glfw3.h>
#endif  // NW_WINDOW

namespace NW
{
    // Cursor state
    CursorState IOSys::Cursor{ 0 };
    // Mouse buttons checking
    ButtonState IOSys::s_bsMsBtns[NW_MS_BTN_LAST] { ButtonState { false, false, false, false, false } };
    // Keyboard checking
    ButtonState IOSys::s_bsKeys[NW_KEY_LAST] { ButtonState { false, false, false, false, false } };
    
    double IOSys::xScrollDelta{ 0.0 }, IOSys::yScrollDelta{0.0f};
}

namespace NW
{
    void IOSys::Update()
    {
        Cursor.xMoveDelta = Cursor.yMoveDelta = 0.0f;
    #if (NW_WINDOW & NW_WINDOW_CONSOLE) // Get windows events and call appropriate functions
        INPUT_RECORD irEvents[32];
        ULInt ulEvLeft = 0;
        GetNumberOfConsoleInputEvents(EvSys::s_pAppWindow->GetHCin(), &ulEvLeft);
        if (ulEvLeft != 0)
        {
            ReadConsoleInputA(EvSys::s_pAppWindow->GetHCin(), irEvents, ulEvLeft, &ulEvLeft);
            for (int ev = 0; ev < ulEvLeft; ev++)
            {
                switch (irEvents[ev].EventType)
                { // Check all 5 kinds of windows events
                case NW_MS_EVT:
                    _MOUSE_EVENT_RECORD msEvt = irEvents[ev].Event.MouseEvent;
                    switch (msEvt.dwEventFlags)
                    {
                    case NW_MS_MOVED:
                        EvSys::cb_mouse_coord(static_cast<double>(msEvt.dwMousePosition.X),
                            NW_ST_CAST(msEvt.dwMousePosition.Y, double));
                        break;
                    case NW_MS_WHEELED: case NW_MS_HWHEELED:
                        EvSys::cb_mouse_scroll(static_cast<double>(msEvt.dwMousePosition.X),
                            NW_ST_CAST(msEvt.dwMousePosition.Y, double));
                        break;
                    case 0: // Process buttons
                        for (Int16 m = NW_MS_BTN_0; m < NW_MS_BTN_LAST; m++)
                            EvSys::cb_mouse_button(m, msEvt.dwButtonState & (1 << m));
                        UpdateMouseButtons();
                        break;
                    }
                    break;
                case NW_KEY_EVT:
                    _KEY_EVENT_RECORD keyEvt = irEvents[ev].Event.KeyboardEvent;
                    EvSys::cb_keyboard(keyEvt.wVirtualKeyCode, keyEvt.wVirtualScanCode, keyEvt.bKeyDown);
                    UpdateKeyboard();
                    break;
                case NW_MENU_EVT:
                    EvSys::cb_menu(irEvents[ev].Event.MenuEvent.dwCommandId);
                    break;
                case NW_WND_SIZE_EVT:
                    EvSys::cb_window_size(irEvents[ev].Event.WindowBufferSizeEvent.dwSize.X,
                        irEvents[ev].Event.WindowBufferSizeEvent.dwSize.Y);
                    break;
                case NW_FOCUS_EVT:
                    EvSys::cb_window_focus(irEvents[ev].Event.FocusEvent.bSetFocus);
                    break;
                }
            }
        }
    #elif (NW_WINDOW & NW_WINDOW_GLFW)
    #endif // Windows events processing
        UpdateKeyboard();
        UpdateMouseButtons();
    }
    void IOSys::UpdateKeyboard()
    {
        for (UInt32 ki = 0; ki < NW_KEY_LAST; ki++)
        {
            s_bsKeys[ki].bPressed = s_bsKeys[ki].bReleased = false;
            if (s_bsKeys[ki].bNew != s_bsKeys[ki].bOld) {
                if (s_bsKeys[ki].bNew == true) {
                    s_bsKeys[ki].bPressed = s_bsKeys[ki].bHeld = true;
                } else {
                    s_bsKeys[ki].bHeld = false;
                    s_bsKeys[ki].bReleased = true;
                }
            }
            s_bsKeys[ki].bOld = s_bsKeys[ki].bNew;
        }
    #if (NW_WINDOW & NW_WINDOW_GLFW)
        if (s_bsKeys[NW_KEY_M_77].bPressed)
        {
            glfwSetInputMode(EvSys::s_pNativeWindow, GLFW_CURSOR,
                Cursor.bBlocked ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
            IOSys::Cursor.bBlocked = !IOSys::Cursor.bBlocked;
        }
    #endif // NW_WINDOW
    }
    void IOSys::UpdateMouseButtons()
    {
        for (UInt32 mi = NW_MS_BTN_0; mi < NW_MS_BTN_LAST; mi++)
        {
            s_bsMsBtns[mi].bPressed = s_bsMsBtns[mi].bReleased = false;
            if (s_bsMsBtns[mi].bNew != s_bsMsBtns[mi].bOld)
            {
                if (s_bsMsBtns[mi].bNew)
                {
                    s_bsMsBtns[mi].bPressed = s_bsMsBtns[mi].bHeld = true;
                    Cursor.xHeld = Cursor.xMove;
                    Cursor.yHeld = Cursor.yMove;
                }
                else
                {
                    s_bsMsBtns[mi].bHeld = false;
                    s_bsMsBtns[mi].bReleased = true;
                }
            }
            s_bsMsBtns[mi].bOld = s_bsMsBtns[mi].bNew;
        }
    }
}