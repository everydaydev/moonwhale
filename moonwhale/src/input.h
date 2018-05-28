#pragma once
#include "common.h"
#include "math.h"

namespace input
{

struct ButtonState
{
    bool m_isDown;
    bool m_isPressed;
    bool m_isReleased;

    void processEvent(bool isDown)
    {
        m_isDown = isDown;
        if (m_isDown)
        {
            m_isPressed = true;
        }
        else
        {
            m_isReleased = true;
        }
    }
};

struct Input
{
    union
    {
        struct
        {
            ButtonState m_moveUp;
            ButtonState m_moveDown;
            ButtonState m_moveLeft;
            ButtonState m_moveRight;
            ButtonState m_actionUp;
            ButtonState m_actionDown;
            ButtonState m_actionLeft;
            ButtonState m_actionRight;
            ButtonState m_mouseLeft;
            ButtonState m_mouseRight;
            ButtonState m_debugButton[6];
        };

        ButtonState m_buttons[16];
    };

    Vec2i m_mouse;
    float m_dt;
    Vec2i m_windowSize;
};

}//input