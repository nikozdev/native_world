#include <nw_pch.hpp>
#include "nw_io_sys.h"

#include <glib/core/nw_gengine.h>
#include <glib/gcontext/nw_window.h>

#if (NW_WINDOW & NW_WINDOW_GLFW)
#include <GLFW/glfw3.h>
#endif  // NW_WINDOW

NW::MouseState NW::IOSys::s_Mouse{ 0 };
NW::KeyboardState NW::IOSys::s_Keyboard { NW::ButtonState { false, false, false, false, false } };

namespace NW
{
    // --setters
    void IOSys::SetCursorIMode(InputModes iMode) {
        s_Mouse.iMode = iMode;
        switch (iMode) {
        case IM_CURSOR_NORMAL:
            glfwSetInputMode(static_cast<GLFWwindow*>(GEngine::Get().GetWindow()->GetNative()), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case IM_CURSOR_DISABLED:
            glfwSetInputMode(static_cast<GLFWwindow*>(GEngine::Get().GetWindow()->GetNative()), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        case IM_CURSOR_HIDDEN:
            glfwSetInputMode(static_cast<GLFWwindow*>(GEngine::Get().GetWindow()->GetNative()), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        default:    break;
        }
    }
    void IOSys::SetKeyboardIMode(InputModes iMode) {
        s_Keyboard.iMode = iMode;
        switch (iMode) {
        case IM_KEYBOARD_STICK:
            glfwSetInputMode(static_cast<GLFWwindow*>(GEngine::Get().GetWindow()->GetNative()), GLFW_STICKY_KEYS, GLFW_TRUE);
            break;
        case IM_KEYBOARD_LOCK:
            glfwSetInputMode(static_cast<GLFWwindow*>(GEngine::Get().GetWindow()->GetNative()), GLFW_LOCK_KEY_MODS, GLFW_TRUE);
            break;
        default:    break;
        }
    }
    // --==<core_methods>==--
    void IOSys::Update()
    {
        s_Mouse.xMoveDelta = s_Mouse.yMoveDelta = 0.0f;
        UpdateKeyboard();
        UpdateMouseButtons();
    }
    // --==</core_methods>==--
    inline void IOSys::UpdateKeyboard()
    {
        for (UInt16 ki = 0; ki < NW_KEY_LAST; ki++) {
            ButtonState& rBs = s_Keyboard.bsKeys[ki];
            rBs.bPressed = rBs.bReleased = false;
            if (rBs.bNew != rBs.bOld) {
                if (rBs.bNew == true) {
                    rBs.bPressed = rBs.bHeld = true;
                } else {
                    rBs.bHeld = false;
                    rBs.bReleased = true;
                }
            }
            rBs.bOld = rBs.bNew;
        }
    }
    inline void IOSys::UpdateMouseButtons()
    {
        for (UInt16 mi = NW_MS_BTN_0; mi < NW_MS_BTN_LAST; mi++) {
            ButtonState& rBs = s_Mouse.bsButtons[mi];
            rBs.bPressed = rBs.bReleased = false;
            if (rBs.bNew != rBs.bOld) {
                if (rBs.bNew) {
                    rBs.bPressed = rBs.bHeld = true;
                    s_Mouse.xHeld = s_Mouse.xMove;
                    s_Mouse.yHeld = s_Mouse.yMove;
                }
                else {
                    rBs.bHeld = false;
                    rBs.bReleased = true;
                }
            }
            rBs.bOld = rBs.bNew;
        }
    }
}