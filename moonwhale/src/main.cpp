#include "common.h"
#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include <tinyobj_loader_c.h>
#include "logger.h"
#include "math.h"
#include "graphics.h"
#include "file.h"
#include "image.h"
#include "input.h"
#include "win32.h"

int main()
{
    logger::setMaxLevel(logger::LogLevel::Fetal);
    win32::initAll();

    // TODO(illkwon): Set target fps based on monitor refresh rate
    int targetFps = 60;
    float targetDt = 1.f / (float)targetFps;

    auto lastCounter = win32::getWallClockCounter();

    bool running = true;

    win32::Window window(1280, 720, "game_lab");

    window.m_userData = &running;
    window.m_onClose = [](void* userData)
    {
        *(bool*)userData = false;
    };

    input::Input input = {};

    while (running)
    {
        for (int i = 0; i < common::arrayCount(input.m_buttons); ++i)
        {
            auto button = &input.m_buttons[i];
            button->m_isPressed = false;
            button->m_isReleased = false;
        }

        if (!win32::handleLoopMessage(&window, &input))
        {
            break;
        }

        input.m_dt = targetDt;
        POINT mousePoint;
        GetCursorPos(&mousePoint);
        ScreenToClient(window.m_window, &mousePoint);
        input.m_mouse.x = mousePoint.x;
        input.m_mouse.y = mousePoint.y;
        input.m_windowSize = win32::getWindowSize(window.m_window);

        //updateAndRenderGame(gameState, &input, &platformCallbacks);

        SwapBuffers(window.m_dc);

        auto currentCounter = win32::getWallClockCounter();
        float dt = win32::getElapsedTime(lastCounter, currentCounter);
        while (dt < targetDt)
        {
            currentCounter = win32::getWallClockCounter();
            dt = win32::getElapsedTime(lastCounter, currentCounter);
        }
        lastCounter = currentCounter;
    }

    //freeGame(gameState);

    return 1;
}
