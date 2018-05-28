#include "win32.h"
#include "input.h"
#define GLAD_IMPL
#include <glad/glad.h>

namespace win32::detail
{

struct GlobalContext
{
    bool m_inited;
    HINSTANCE m_instance;
    LPCSTR m_windowClassName;
    Window* m_window;
    LARGE_INTEGER m_perfFrequency;
};

}

static win32::detail::GlobalContext g_ctx;

namespace win32::detail
{

bool initGL()
{
    HWND fakeWindow = CreateWindowEx(0, g_ctx.m_windowClassName,
            "fake", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, g_ctx.m_instance, 0);
    if (!fakeWindow)
    {
        l_error("Fail to create fake window for opengl init");
        return false;
    }

    HDC fakeDC = GetDC(fakeWindow);
    PIXELFORMATDESCRIPTOR fakePfd = {};
    fakePfd.nSize = sizeof(fakePfd);
    fakePfd.nVersion = 1;
    fakePfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    fakePfd.iPixelType = PFD_TYPE_RGBA;
    fakePfd.cColorBits = 32;
    fakePfd.cDepthBits = 24;
    fakePfd.cStencilBits = 8;
    fakePfd.iLayerType = PFD_MAIN_PLANE;

    int fakePixelFormat = ChoosePixelFormat(fakeDC, &fakePfd);
    if (fakePixelFormat == 0)
    {
        l_error("Failed to get pixel format");
        return false;
    }

    SetPixelFormat(fakeDC, fakePixelFormat, &fakePfd);
    HGLRC fakeRC = wglCreateContext(fakeDC);
    if (fakeRC == 0)
    {
        l_error("Failed to create wgl context");
        return false;
    }

    if (!gladLoadGL())
    {
        l_error("Failed to load extensions");
        return false;
    }

    wglMakeCurrent(0, 0);
    wglDeleteContext(fakeRC);
    ReleaseDC(fakeWindow, fakeDC);
    DestroyWindow(fakeWindow);

    return true;
}

}

namespace win32
{

bool initAll()
{
    if (g_ctx.m_inited)
    {
        l_error("Already initialized");
        return true;
    }

    timeBeginPeriod(1);
    QueryPerformanceFrequency(&g_ctx.m_perfFrequency);

    HINSTANCE instance = GetModuleHandle(0);

    WNDCLASSEX windowClass = {};
    windowClass.cbSize = sizeof(windowClass);
    windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    windowClass.lpfnWndProc = &mainWindowCallback;
    windowClass.hInstance = instance;
    windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = "WindowClass";

    if (!RegisterClassEx(&windowClass))
    {
        DWORD error = GetLastError();
        l_error("RegisterClassEx failed: %d", error);
        return false;
    }

    g_ctx.m_instance = instance;
    g_ctx.m_windowClassName = windowClass.lpszClassName;

    /*if (!detail::initGL())
    {
        l_error("Failed to init gl");
        return false;
    }*/

    g_ctx.m_inited = true;
    return true;
}

Window::Window(int width, int height, const char* windowName)
{
    assert(g_ctx.m_inited);
    assert(g_ctx.m_window == nullptr);
    g_ctx.m_window = this;

    m_window = CreateWindowEx(0, g_ctx.m_windowClassName,
        windowName,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        0, 0, g_ctx.m_instance, 0);
    if (!m_window)
    {
        l_error("CreateWindowEx failed");
        assert(false);
    }

    m_dc = GetDC(m_window);
    m_width = width;
    m_height = height;
}

Vec2i getWindowSize(HWND window)
{
    RECT clientRect; GetClientRect(window, &clientRect);
    auto result = Vec2i{
        clientRect.right - clientRect.left,
        clientRect.bottom - clientRect.top };
    return result;
}

bool handleLoopMessage(Window* window, input::Input* input)
{
    MSG msg;
    bool running = true;

    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
        WPARAM wp = msg.wParam;
        LPARAM lp = msg.lParam;

        switch (msg.message)
        {
        case WM_QUIT:
        {
            running = false;
        } break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            uint keyCode = static_cast<uint>(wp);
            bool wasDown = ((lp & (1 << 30)) != 0);
            bool isDown = ((lp & (1 << 31)) == 0);
            if (wasDown != isDown)
            {
                switch (keyCode)
                {
                case 'W':
                    input->m_moveUp.processEvent(isDown); break;
                case 'A':
                    input->m_moveLeft.processEvent(isDown); break;
                case 'S':
                    input->m_moveDown.processEvent(isDown); break;
                case 'D':
                    input->m_moveRight.processEvent(isDown); break;
                case VK_UP:
                    input->m_actionUp.processEvent(isDown); break;
                case VK_LEFT:
                    input->m_actionLeft.processEvent(isDown); break;
                case VK_DOWN:
                    input->m_actionDown.processEvent(isDown); break;
                case VK_RIGHT:
                    input->m_actionRight.processEvent(isDown); break;
                case VK_F1:
                case VK_F2:
                case VK_F3:
                case VK_F4:
                case VK_F5:
                case VK_F6:
                    input->m_debugButton[keyCode - VK_F1].processEvent(isDown); break;
                }
            }

            bool altWasDown = ((lp & (1 << 29)) != 0);
            if ((keyCode == VK_F4 && altWasDown) || keyCode == VK_ESCAPE)
            {
                running = false;
            }
        } break;

        case WM_LBUTTONDOWN:
            input->m_mouseLeft.processEvent(true);
            break;
        case WM_LBUTTONUP:
            input->m_mouseLeft.processEvent(false);
            break;
        default:
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        }
    }

    return running;
}

LARGE_INTEGER getWallClockCounter()
{
    LARGE_INTEGER result;
    QueryPerformanceCounter(&result);
    return result;
}

float getElapsedTime(LARGE_INTEGER startCounter, LARGE_INTEGER endCounter)
{
    float result =
        static_cast<float>(endCounter.QuadPart - startCounter.QuadPart) /
        static_cast<float>(g_ctx.m_perfFrequency.QuadPart);
    return result;
}

LRESULT CALLBACK mainWindowCallback(HWND window, UINT message, WPARAM wp, LPARAM lp)
{
    LRESULT result = 0;

    switch (message)
    {
    case WM_EXITSIZEMOVE:
    {
        //auto dim = win32_get_window_dim(window);
        //LOG("WM_EXITSIZEMOVE from main_window_callback: %d %d\n", dim.x, dim.y);
    } break;
    case WM_SIZE:
    {
    } break;

    case WM_CLOSE:
    case WM_DESTROY:
    {
        g_ctx.m_window->m_onClose(g_ctx.m_window->m_userData);
    } break;

    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP:
    {
        assert(!"키입력 이벤트가 이 콜백으로 들어오면 안됨");
    } break;

    default:
    {
        result = DefWindowProc(window, message, wp, lp);
    } break;
    }

    return result;
}

}//win32
