#ifndef K_GRAPHICS_H
#define K_GRAPHICS_H
#include "common.h"
#include "math.h"

namespace graphics::gl
{

struct Renderable
{
    uint m_vao;
    uint m_vbo;
    uint m_ebo;
};

}

namespace graphics::sgr
{

struct Texture
{
    u32* m_pixels;
    int m_width;
    int m_height;

    Texture(int width, int height);
    ~Texture();
    void drawPoint(int x, int y, u32 color);
    void drawLine(int x0, int y0, int x1, int y1, u32 color);
    void fillTriangleBarycentric(Vec2i v0, Vec2i v1, Vec2i v2, u32 color);
    void fillRect(int x0, int y0, int x1, int y1, u32 color);
    void flipVertically();
};

}

#endif//K_GRAPHICS_H
