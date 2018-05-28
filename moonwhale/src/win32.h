#pragma once

#ifndef _WIN32
#error The platform is not Windows
#endif//_WIN32

#include <Windows.h>
#include "common.h"
#include "logger.h"
#include "math.h"

namespace input
{
struct Input;
}

namespace win32
{

bool initAll();

#define on_window_close_callback(name) void name(void*)
typedef on_window_close_callback(OnWindowCloseCallback);
inline on_window_close_callback(OnWindowCloseCallbackStub) {}

struct Window
{
    HWND m_window;
    HDC m_dc;
    int m_width;
    int m_height;

    OnWindowCloseCallback* m_onClose = &OnWindowCloseCallbackStub;
    void* m_userData = nullptr;

    Window(int width, int height, const char* windowName);
};

Vec2i getWindowSize(HWND window);
bool handleLoopMessage(Window* window, input::Input* input);
LARGE_INTEGER getWallClockCounter();
float getElapsedTime(LARGE_INTEGER start, LARGE_INTEGER end);

LRESULT CALLBACK mainWindowCallback(HWND window, UINT message, WPARAM wp, LPARAM lp);

}//win32