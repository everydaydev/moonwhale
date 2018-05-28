#include "graphics.h"
#include "win32.h"
#include <utility>//swap
#if _WIN32
#include <malloc.h>//alloca
#endif

namespace graphics::sgr
{

Texture::Texture(int width, int height)
{
    m_pixels = static_cast<u32*>(malloc(sizeof(*m_pixels) * width * height));
    m_width = width;
    m_height = height;
}

Texture::~Texture()
{
    if (m_pixels)
    {
        free(m_pixels);
        m_pixels = nullptr;
    }
}

void Texture::drawPoint(int x, int y, u32 color)
{
    if (!(x >= 0 && x < m_width && y >= 0 && y < m_height))
    {
        return;
    }
    m_pixels[y * m_width + x] = color;
}


void Texture::drawLine(int x0, int y0, int x1, int y1, u32 color)
{
    using std::swap;

    int dx = x1 - x0;
    int dy = y1 - y0;
    bool steep = false;

    if (abs(dy) > abs(dx))
    {
        swap(x0, y0);
        swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }

    // Calculate dx/dy with updated values
    dx = x1 - x0;
    dy = y1 - y0;
    int derr = abs(2 * dy);
    int err = 0;

    int y = y0;
    for (int x = x0; x <= x1; ++x)
    {
        if (steep)
        {
            drawPoint(y, x, color);
        }
        else
        {
            drawPoint(x, y, color);
        }
        err += derr;
        if (err > dx)
        {
            y += (dy > 0 ? 1 : -1);
            err -= 2 * dx;
        }
    }
}

void Texture::fillTriangleBarycentric(Vec2i v0, Vec2i v1, Vec2i v2, u32 color)
{
}

void Texture::fillRect(int x0, int y0, int x1, int y1, u32 color)
{
    for (int y = y0; y < y1; ++y)
    {
        for (int x = x0; x < x1; ++x)
        {
            drawPoint(x, y, color);
        }
    }
}

void Texture::flipVertically()
{
    size_t rowSize = sizeof(*m_pixels) * m_width;
    uint32_t* temp = static_cast<uint32_t*>(alloca(rowSize));
    for (int y = 0; y < (m_height / 2); ++y)
    {
        uint32_t* upperRow = m_pixels + y * m_width;
        uint32_t* lowerRow = m_pixels + (m_height - 1 - y) * m_width;
        memcpy(temp, upperRow, rowSize);
        memcpy(upperRow, lowerRow, rowSize);
        memcpy(lowerRow, temp, rowSize);
    }
}

#if 0
static
void graphics_texture_fillTriangleBresenham(Texture* texture, Vec2i v0, Vec2i v1, Vec2i v2, uint32_t color)
{
    if (v0.y > v1.y) { swap(v0, v1); }
    if (v0.y > v2.y) { swap(v0, v2); }
    if (v1.y > v2.y) { swap(v1, v2); }
    if (v0.y == v1.y && v0.x > v1.x) { swap(v0, v1); }

#if 0
    /*for (int y = v0.y; y < v1.y; ++y)
    {
        float ta = (float)(y - v0.y) / (float)(v2.y - v0.y + 1);
        int xa = (int)(v0.x * (1.f - ta) + v2.x * ta);
        float tb = (float)(y - v0.y) / (float)(v1.y - v0.y);
        int xb = (int)(v0.x * (1.f - tb) + v1.x * tb);
        graphics_texture_drawPoint(texture, xa, y, color);
        graphics_texture_drawPoint(texture, xb, y, color);
        //graphics_texture_drawLine(texture, xa, y, xb, y, color);
    }

    int dx = v2.x - v1.x;
    int dy = v2.y - v1.y;

    for (int y = v1.y; y <= v2.y; ++y)
    {
        int segmentHeight = v2.y - v1.y;
        //float ta = (float)(y - v0.y) / (float)totalHeight;
        //float tb = (float)(y - v1.y) / (float)segmentHeight;
        float ta = (float)(y - v0.y) / (float)totalHeight;
        float tb = (float)(y - v1.y) / (float)segmentHeight;
        float xa = v0.x * (1.f - ta) + v2.x * ta;
        float xb = v1.x * (1.f - tb) + v2.x * tb;
        if (xa > xb)
        {
            swap(xa, xb);
        }

        graphics_texture_drawPoint(texture, (int)xa, y, color);
        graphics_texture_drawPoint(texture, (int)xb, y, color);
        //graphics_texture_drawLine(texture, xa, y, xb, y, color);
    }*/

    // It is guaranteed both va and vb belong to octant 1~4
    Vec2i va = v0;
    Vec2i vb = v1;

    Vec2i da = vec2i_sub(v2, va);
    //if (da.x > )
    //int erra = 0;
    int derra = abs(2 * da.x);
    int xa = va.x;

    Vec2i db = vec2i_sub(v2, vb);
    int errb = 0;
    int derrb = abs(2 * db.x);
    int xb =

    for (int y = v0.y; y <= v2.y; ++y)
    {
        graphics_texture_drawPoint(texture, xa, y, color);

        erra += derra;
        if (erra > da.y)
        {
            xa += da.x > 0 ? 1 : -1;
            erra -= 2 * da.y;
        }
    }
#endif

#if 0
    graphics_texture_drawLine(texture, v0.x, v0.y, v1.x, v1.y, color);
    graphics_texture_drawLine(texture, v1.x, v1.y, v2.x, v2.y, color);
    graphics_texture_drawLine(texture, v2.x, v2.y, v0.x, v0.y, color);
#endif
}
#endif

}