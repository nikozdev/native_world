#include <cn_pch.hpp>
#include <events\ev_EvSys.h>

namespace EV
{
    bool* EvSys::keysList = nullptr;
    UInt* EvSys::framesList = nullptr;
    UInt EvSys::currFrame;

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
    int EvSys::initialize(GLFWwindow* wndPtr)
    {
#ifdef USE_EVSYS
        keysList = new bool[INPUT_SIZE];
        framesList = new UInt[INPUT_SIZE];
        // Make these arrays empty
        memset(keysList, false, sizeof(bool) * INPUT_SIZE);
        memset(framesList, 0, sizeof(UInt) * INPUT_SIZE);

        glfwSetCursorPosCallback(wndPtr, cb_mousePos);
        glfwSetMouseButtonCallback(wndPtr, cb_mouseButton);
        glfwSetKeyCallback(wndPtr, cb_key);
        return 0;
#endif
    }

    void EvSys::pollEvents()
    {
#ifdef USE_EVSYS
        currFrame++;
        msDeltaX = 0.0;
        msDeltaY = 0.0;
        glfwPollEvents();
#endif
    }

    // Keyboard Input
    bool EvSys::isPressed_key(int keyCode)
    {
        if (keyCode < 0 || keyCode >= MOUSE_BTNS)
            return false;
        return keysList[keyCode] && currFrame - framesList[keyCode] > 50;
    }
    bool EvSys::isJPressed_key(int keyCode)
    {
        if (keyCode < 0 || keyCode >= MOUSE_BTNS)
            return false;
        return keysList[keyCode] == currFrame;
    }
    // Mouse Input
    bool EvSys::isClicked(int btnCode)
    {
        return keysList[btnCode] && currFrame - framesList[btnCode] > 50;
    }
    bool EvSys::isJClicked(int btnCode)
    {
        return keysList[btnCode] == currFrame;
    }

    // Callback functions
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
    }

    void EvSys::cb_mouseScroll(GLFWwindow* wndPtr, double xPos, double yPos)
    {
    }

    void EvSys::cb_mouseButton(GLFWwindow* wndPtr,
        int button, int action, int mode)
    {// If the mouse event is gotten - set true/false in the keylist
        if (action == GLFW_PRESS)
        {
            EvSys::keysList[MOUSE_BTNS + button] = true;
            EvSys::framesList[MOUSE_BTNS + button] = EvSys::currFrame;
        }
        else if (action == GLFW_RELEASE)
        {
            EvSys::keysList[MOUSE_BTNS + button] = false;
            EvSys::framesList[MOUSE_BTNS + button] = EvSys::currFrame;
        }
    }

    void EvSys::cb_key(GLFWwindow* wndPtr,
        int key, int scanCode, int action, int mode)
    {// If the key event is gotten - set true/false in the keylist
        if (action == GLFW_PRESS)
        {
            EvSys::keysList[key] = true;
            EvSys::framesList[key] = EvSys::currFrame;
        }
        else if (action == GLFW_RELEASE)
        {
            EvSys::keysList[key] = false;
            EvSys::framesList[key] = EvSys::currFrame;
        }
    }
}