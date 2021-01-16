#include <nw_pch.hpp>
#include "nw_io_sys.h"

#include <glib/core/nw_gengine.h>
#include <glib/gcontext/nw_window.h>

#if (NW_WINDOW & NW_WINDOW_GLFW)
#include <GLFW/glfw3.h>
#endif  // NW_WINDOW

NW::CursorState NW::IOSys::s_Cursor{ 0 };
NW::ButtonState NW::IOSys::s_bsMsBtns[NW_MS_BTN_LAST] { NW::ButtonState { false, false, false, false, false } };
NW::ButtonState NW::IOSys::s_bsKeys[NW_KEY_LAST] { NW::ButtonState { false, false, false, false, false } };
double NW::IOSys::s_xScroll{ 0.0 }, NW::IOSys::s_yScroll{0.0f};

namespace NW
{
    // --setters
    void IOSys::SetInputMode(InputModes IMode) {
        switch (IMode) {
        case IM_CURSOR_NORMAL:
            glfwSetInputMode(static_cast<GLFWwindow*>(GEngine::Get().GetWindow()->GetNative()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case IM_CURSOR_DISABLED:
            glfwSetInputMode(static_cast<GLFWwindow*>(GEngine::Get().GetWindow()->GetNative()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        case IM_CURSOR_HIDDEN:
            glfwSetInputMode(static_cast<GLFWwindow*>(GEngine::Get().GetWindow()->GetNative()), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        }
    }
    // --==<core_methods>==--
    void IOSys::Update()
    {
        s_Cursor.xMoveDelta = s_Cursor.yMoveDelta = 0.0f;
        UpdateKeyboard();
        UpdateMouseButtons();
    }
    // --==</core_methods>==--
    inline void IOSys::UpdateKeyboard()
    {
        for (UInt16 ki = 0; ki < NW_KEY_LAST; ki++) {
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
    }
    inline void IOSys::UpdateMouseButtons()
    {
        for (UInt16 mi = NW_MS_BTN_0; mi < NW_MS_BTN_LAST; mi++)
        {
            s_bsMsBtns[mi].bPressed = s_bsMsBtns[mi].bReleased = false;
            if (s_bsMsBtns[mi].bNew != s_bsMsBtns[mi].bOld) {
                if (s_bsMsBtns[mi].bNew) {
                    s_bsMsBtns[mi].bPressed = s_bsMsBtns[mi].bHeld = true;
                    s_Cursor.xHeld = s_Cursor.xMove;
                    s_Cursor.yHeld = s_Cursor.yMove;
                }
                else {
                    s_bsMsBtns[mi].bHeld = false;
                    s_bsMsBtns[mi].bReleased = true;
                }
            }
            s_bsMsBtns[mi].bOld = s_bsMsBtns[mi].bNew;
        }
    }
}