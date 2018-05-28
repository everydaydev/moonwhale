#include "image.h"
#include "file.h"
#include <cstring>
#ifdef _WIN32
#include <Windows.h>
#endif

namespace image
{

void writeUncompressedTGA(char* filename, uint32_t* pixels, int width, int height)
{
    TGAHeader header = { 0 };
    header.dataTypeCode = 2;// Uncompressed true color image
    header.xOrigin = 0;
    header.yOrigin = 0;
    header.width = static_cast<u16>(width);
    header.height = (u16)height;
    header.bitsPerPixel = 32;
    header.imageDescriptor = 0b00100000;

    uint imageSize = sizeof(uint32_t) * width * height;
    uint fileSize = sizeof(TGAHeader) + imageSize;
    void* fileData = malloc(fileSize);
    memcpy(fileData, &header, sizeof(header));
    memcpy((u8*)fileData + sizeof(header), pixels, imageSize);
    file::writeAll(filename, fileData, fileSize);
    free(fileData);
}

Image loadBMP(char* filename)
{
    auto[success, bmpFileData] = file::readAll(filename);

    auto header = (BMPHeader*)bmpFileData.m_contents;
    assert(header->bitsPerPixel == 32);
    uint32_t* pixels = (uint32_t*)((uint8_t*)bmpFileData.m_contents + header->bitmapOffset);
    DWORD redBitOffset, greenBitOffset, blueBitOffset, alphaBitOffset;
    _BitScanForward(&redBitOffset, header->redMask);
    _BitScanForward(&greenBitOffset, header->greenMask);
    _BitScanForward(&blueBitOffset, header->blueMask);
    _BitScanForward(&alphaBitOffset, header->alphaMask);

    for (int i = 0; i < header->width * header->height; ++i)
    {
        uint32_t pixel = pixels[i];
        uint8_t r = (uint8_t)((pixel & header->redMask) >> redBitOffset);
        uint8_t g = (uint8_t)((pixel & header->greenMask) >> greenBitOffset);
        uint8_t b = (uint8_t)((pixel & header->blueMask) >> blueBitOffset);
        uint8_t a = (uint8_t)((pixel & header->alphaMask) >> alphaBitOffset);
        pixels[i] = (a << 24) | (b << 16) | (g << 8) | (r << 0);
    }

    Image result =
    {
        pixels,
        header->width,
        header->height
    };
    return result;
}

}//image