#include <cn_pch.hpp>
#include <cn/window/WinWindow.h>

#include <events/ev_EvSys.h>
#include <events/ev_KeyEvent.h>
#include <events/ev_MouseEvent.h>
#include <events/ev_WndEvent.h>

#include <extern/GLFW/glfw3.h>

namespace CN
{
    namespace EV
    {
        /*bool* EvSys::keysList = nullptr;
        UInt* EvSys::framesList = nullptr;
        UInt EvSys::currFrame;*/

        double EvSys::msDeltaX = 0.0;
        double EvSys::msDeltaY = 0.0;
        double EvSys::msPosX = 0.0;
        double EvSys::msPosY = 0.0;

        bool EvSys::cursorBlocked = true;
        bool EvSys::cursorFirst = true;
    }
    // EventSystem class
    namespace EV
    {
        int EvSys::init(GLFWwindow* wndPtr)
        {
            //keysList = new bool[INPUT_SIZE];
            //framesList = new UInt[INPUT_SIZE];
            // Make these arrays empty
            //memset(keysList, false, sizeof(bool) * INPUT_SIZE);
            //memset(framesList, 0, sizeof(UInt) * INPUT_SIZE);

            glfwSetWindowCloseCallback(wndPtr, cb_wndClose);

            glfwSetCursorPosCallback(wndPtr, cb_mousePos);
            glfwSetMouseButtonCallback(wndPtr, cb_mouseButton);
            
            glfwSetFramebufferSizeCallback(wndPtr, cb_wndSize);

            glfwSetErrorCallback(cb_onError);
            glfwSetKeyCallback(wndPtr, cb_key);

            return 0;
        }

        void EvSys::pollEvents()
        {
            //currFrame++;
            msDeltaX = 0.0;
            msDeltaY = 0.0;
            glfwPollEvents();
        }

        // Keyboard Input
        bool EvSys::isPressed_key(int keyCode)
        {
            if (keyCode < 0 || keyCode >= MOUSE_BTNS)
                return false;
            //return keysList[keyCode] && currFrame - framesList[keyCode] > 50;
            return false;
        }
        bool EvSys::isJPressed_key(int keyCode)
        {
            if (keyCode < 0 || keyCode >= MOUSE_BTNS)
                return false;
            //return keysList[keyCode] == currFrame;
            return false;
        }
        // Mouse Input
        bool EvSys::isClicked(int btnCode)
        {
            if (btnCode < MOUSE_BTNS || btnCode > INPUT_SIZE)
                return false;
            //return keysList[btnCode] && currFrame - framesList[btnCode] > 50;
            return false;
        }
        bool EvSys::isJClicked(int btnCode)
        {
            if (btnCode < MOUSE_BTNS || btnCode > INPUT_SIZE)
                return false;
            //return keysList[btnCode] == currFrame;
            return false;
        }
        // Mouse config
        void EvSys::toggleCursor(GLFWwindow* wndPtr)
        {
            cursorBlocked = !cursorBlocked;
            glfwSetInputMode(wndPtr, GLFW_CURSOR,
                cursorBlocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        }

        // --------<Callback functions>--------
        // Window callbacks
        void EvSys::cb_wndClose(GLFWwindow* wndPtr)
        {
            WinWindow::WndData& wndData = *(WinWindow::WndData*)(glfwGetWindowUserPointer(wndPtr));
            WndCloseEvent event;
            wndData.ev_Callback(event);
        }

        void EvSys::cb_wndSize(GLFWwindow* wndPtr, int width, int height)
        {
            WinWindow::WndData& wndData = *(WinWindow::WndData*)(glfwGetWindowUserPointer(wndPtr));
            WndResizeEvent event(width, height);
            wndData.Width = width;
            wndData.Height = height;
            wndData.ev_Callback(event);
        }

        // Mouse callbacks
        void EvSys::cb_mousePos(GLFWwindow* wndPtr,
            double xPos, double yPos)
        {
            if (cursorFirst)
            {
                msDeltaX += xPos - msPosX;
                msDeltaY += yPos - msPosY;
            }
            else
            {
                cursorFirst = true;
            }
            msPosX = xPos;
            msPosX = yPos;

            WinWindow::WndData& wndData = *(WinWindow::WndData*)(glfwGetWindowUserPointer(wndPtr));
            MouseMoveEvent event(xPos, yPos);
            wndData.ev_Callback(event);
        }

        void EvSys::cb_mouseScroll(GLFWwindow* wndPtr, double offsetX, double offsetY)
        {
            WinWindow::WndData& wndData = *(WinWindow::WndData*)(glfwGetWindowUserPointer(wndPtr));
            MouseScrollEvent event(offsetX, offsetY);
            wndData.ev_Callback(event);
        }

        void EvSys::cb_mouseButton(GLFWwindow* wndPtr,
            int button, int action, int mode)
        {// If the mouse event is gotten - set true/false in the keylist
            WinWindow::WndData& wndData = *(WinWindow::WndData*)(glfwGetWindowUserPointer(wndPtr));
            if (action == GLFW_PRESS)
            {
                //EvSys::keysList[MOUSE_BTNS + button] = true;
                //EvSys::framesList[MOUSE_BTNS + button] = EvSys::currFrame;
                MousePressedEvent event(button);
                wndData.ev_Callback(event);
            }
            else if (action == GLFW_RELEASE)
            {
                //EvSys::keysList[MOUSE_BTNS + button] = false;
                //EvSys::framesList[MOUSE_BTNS + button] = EvSys::currFrame;
                MouseReleasedEvent event(button);
                wndData.ev_Callback(event);
            }
        }
        
        // Keyboard callbacks
        void EvSys::cb_key(GLFWwindow* wndPtr,
            int key, int scanCode, int action, int mode)
        {// If the key event is gotten - set true/false in the keylist
            WinWindow::WndData& wndData = *(WinWindow::WndData*)(glfwGetWindowUserPointer(wndPtr));
            if (action == GLFW_PRESS)
            {
                KeyPressedEvent event(key, scanCode);
                wndData.ev_Callback(event);
                //EvSys::keysList[key] = true;
                //EvSys::framesList[key] = EvSys::currFrame;
            }
            else if (action == GLFW_RELEASE)
            {
                KeyReleasedEvent event(key);
                wndData.ev_Callback(event);
                //EvSys::keysList[key] = false;
                //EvSys::framesList[key] = EvSys::currFrame;
                //
            }
        }
        void EvSys::cb_onError(int errId, const char* errMsg)
        {
            printf("EV::CB_ON_ERROR: GLFW error occured:\nID_%d::MSG: %s\n", errId, errMsg);
        }
        // --------<Callback functions>--------
    }
}