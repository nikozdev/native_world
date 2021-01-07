#include <nw_pch.hpp>
#include "nw_ev_sys.h"

#include <core/nw_event.h>

#include <gl/gcontext/nw_window.h>

#include <core/nw_core_engine.h>

#include <sys/nw_gui_sys.h>
#include <sys/nw_log_sys.h>
#include <sys/nw_io_sys.h>

#if (NW_WINDOW & NW_WINDOW_GLFW)
#include <GLFW/glfw3.h>
#endif  // NW_WINDOW

namespace NW
{
    AppWindow* EvSys::s_pAppWindow(nullptr);
    NativeWindow EvSys::s_pNativeWindow(nullptr);
    bool EvSys::s_bWindowFocused(false);
}
namespace NW
{
    // --==<core_methods>==--
    bool EvSys::OnInit()
    {
        s_pAppWindow = dynamic_cast<WindowOgl*>(CoreEngine::Get().GetWindow());
        s_pNativeWindow = (NativeWindow)(s_pAppWindow->GetNativeWindow());
    #if (NW_WINDOW & NW_WINDOW_GLFW)
        if (s_pNativeWindow == nullptr || s_pAppWindow == nullptr) return false;
        glfwSetWindowCloseCallback(s_pNativeWindow, cb_window_close);

        glfwSetCursorPosCallback(s_pNativeWindow, cb_mouse_coord);
        glfwSetScrollCallback(s_pNativeWindow, cb_mouse_scroll);
        glfwSetMouseButtonCallback(s_pNativeWindow, cb_mouse_button);

        glfwSetKeyCallback(s_pNativeWindow, cb_keyboard);
        glfwSetCharCallback(s_pNativeWindow, cb_keyboard_char);

        glfwSetFramebufferSizeCallback(s_pNativeWindow, cb_window_size);
        glfwSetWindowFocusCallback(s_pNativeWindow, cb_window_focus);

        glfwSetErrorCallback(cb_error);
    #endif // NW_WINDOW
    #if (NW_WINDOW & NW_WINDOW_CONSOLE)
        if ((s_pCoutWindow = dynamic_cast<CoutWindow*>(windowPtr)) == nullptr)
            return false;
    #endif // NW_WINDOW
        return true;
    }
    void EvSys::Update()
    {
    #if (NW_WINDOW & NW_WINDOW_GLFW)
        glfwPollEvents();
    #endif // NW_WINDOW
    }
    // --==</core_methods>==--

#if (NW_WINDOW & NW_WINDOW_CONSOLE)
    // --==<Console Callback functions>==--
    // Windows callbacks
    void EvSys::cb_menu(Int32 nCmdId)
    {
        switch (nCmdId)
        {
        default:
            NW_ERR("Don't touch the menu now! It is not ready still!");
            break;
        }
    }
    void EvSys::cb_window_size(Int32 nW, Int32 nH)
    {
        //
    }
    void EvSys::cb_window_focus(Int32 nFocus)
    {
        s_bWindowFocused = nFocus;
    }
    // Input callbacks
    void EvSys::cb_mouse_coord(double nX, double nY)
    {
        s_xyMsDelta.x = nX - s_xyMsMove.x;
        s_xyMsDelta.y = nY - s_xyMsMove.y;
        s_xyMsMove = { nX, nY };
    }
    void EvSys::cb_mouse_scroll(double nX, double nY)
    {
        s_xyMsScrollDelta.x = nX;
        s_xyMsScrollDelta.y = nY;
    }
    void EvSys::cb_mouse_button(Int32 nButton, Int32 nAction)
    {
        s_bMsNew[nButton] = nAction;
    }
    void EvSys::cb_key(Int32 nKey, Int32 nScanCode, Int32 nAction)
    {
        s_bKeysNew[nKey] = nAction;
    }
    void EvSys::cb_key_char(UInt32 unChar)
    {
    }
    // --==</Console Callback functions>==--
#endif // NW_WINDOW
#if (NW_WINDOW & NW_WINDOW_GLFW)
    // --==<GLFW Callback functions>==--
    // Input callbacks
    void EvSys::cb_mouse_coord(GLFWwindow* pWindow,
        double nCoordX, double nCoordY)
    {
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        MouseEvent mEvt = MouseEvent(ET_MOUSE_MOVE, nCoordX, nCoordY);

        IOSys::Cursor.xMoveDelta = nCoordX - IOSys::Cursor.xMove;
        IOSys::Cursor.yMoveDelta = nCoordY - IOSys::Cursor.yMove;
        IOSys::Cursor.xMove = nCoordX; IOSys::Cursor.yMove = nCoordY;
        rWindowInfo.fnEvCallback(mEvt);
        GuiSys::OnEvent(mEvt);
    }
    void EvSys::cb_mouse_scroll(GLFWwindow* pWindow, double xScrollDelta, double yScrollDelta)
    {
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        MouseEvent mEvt = MouseEvent(ET_MOUSE_SCROLL, xScrollDelta, yScrollDelta);

        IOSys::xScrollDelta = xScrollDelta;
        IOSys::yScrollDelta = yScrollDelta;
        rWindowInfo.fnEvCallback(mEvt);
        GuiSys::OnEvent(mEvt);
    }
    void EvSys::cb_mouse_button(GLFWwindow* pWindow,
        Int32 nButton, Int32 nAction, Int32 nMode)
    {   // If the mouse event is gotten - set true/false in the keylist
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        if (nAction == GLFW_PRESS) {
            IOSys::s_bsMsBtns[nButton].bNew = true;
            MouseEvent mEvt(ET_MOUSE_PRESS, nButton);
            
            rWindowInfo.fnEvCallback(mEvt);
            GuiSys::OnEvent(mEvt);
        }
        else if (nAction == GLFW_RELEASE) {
            IOSys::s_bsMsBtns[nButton].bNew = false;
            MouseEvent mEvt(ET_MOUSE_RELEASE, nButton);
            
            rWindowInfo.fnEvCallback(mEvt);
            GuiSys::OnEvent(mEvt);
        }
    }
    void EvSys::cb_keyboard(GLFWwindow* pWindow, Int32 nKeyCode, Int32 nScanCode, Int32 nAction, Int32 nMode)
    {   // If the key event is gotten - set true/false in the keylist
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        if (nAction == GLFW_PRESS) {
            IOSys::s_bsKeys[nKeyCode].bNew = true;
            KeyboardEvent kEvt(ET_KEY_PRESS, nKeyCode);
            
            rWindowInfo.fnEvCallback(kEvt);
            GuiSys::OnEvent(kEvt);
        }
        else if (nAction == GLFW_RELEASE) {
            IOSys::s_bsKeys[nKeyCode].bNew = false;
            KeyboardEvent kEvt(ET_KEY_RELEASE, nKeyCode);
            
            rWindowInfo.fnEvCallback(kEvt);
            GuiSys::OnEvent(kEvt);
        }
    }
    void EvSys::cb_keyboard_char(GLFWwindow* pWindow, UInt32 unChar)
    {
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        KeyboardEvent kEvt(ET_KEY_TYPE, unChar);
        
        rWindowInfo.fnEvCallback(kEvt);
        GuiSys::OnEvent(kEvt);
    }
    // Window callbacks
    void EvSys::cb_window_close(GLFWwindow* pWindow)
    {
        WindowEvent wEvt = WindowEvent(ET_WINDOW_CLOSE);
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));

        glfwSetWindowShouldClose(s_pNativeWindow, true);
        rWindowInfo.fnEvCallback(wEvt);
        GuiSys::OnEvent(wEvt);
    }
    void EvSys::cb_window_size(GLFWwindow* pWindow, Int32 nWidth, Int32 nHeight)
    {
        WindowEvent wEvt = WindowEvent(ET_WINDOW_RESIZE, nWidth, nHeight);
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        rWindowInfo.unWidth = nWidth; rWindowInfo.unHeight = nHeight;
        
        glfwSetWindowSize(pWindow, nWidth, nHeight);
        rWindowInfo.fnEvCallback(wEvt);
        GuiSys::OnEvent(wEvt);
    }
    void EvSys::cb_window_focus(GLFWwindow* pWindow, Int32 nFocus)
    {
        WindowInfo& rWindowInfo = *(WindowInfo*)(glfwGetWindowUserPointer(pWindow));
        WindowEvent wEvt(ET_WINDOW_FOCUS, nFocus);
        s_bWindowFocused = nFocus;
        
        rWindowInfo.fnEvCallback(wEvt);
        GuiSys::OnEvent(wEvt);
    }
    // Other callbacks
    void EvSys::cb_error(Int32 errId, const char* errMsg)
    {
        LogSys::WriteErrStr(errId, "{str}", errMsg);
    }
    // --==</GLFW Callback functions>==--
#endif // NW_WINDOW
}