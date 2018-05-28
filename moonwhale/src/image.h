#pragma once
#include "common.h"

namespace image
{

struct Image
{
    u32* m_pixels;
    int m_width;
    int m_height;
};

#pragma pack(push, 1)
struct TGAHeader
{
    u8 idLength;
    u8 colorMapType;
    u8 dataTypeCode;
    u16 colorMapOrigin;
    u16 colorMapLength;
    u8 colorMapDepth;
    u16 xOrigin;
    u16 yOrigin;
    u16 width;
    u16 height;
    u8 bitsPerPixel;
    u8 imageDescriptor;
};
static_assert(sizeof(TGAHeader) == 18, "");
#pragma pack(pop)

void writeUncompressedTGA(char* filename, uint32_t* pixels, int width, int height);

#pragma pack(push, 1)
struct BMPHeader
{
    u16 fileType;
    u32 fileSize;
    u32 padding;
    u32 bitmapOffset;
    u32 size;
    i32 width;
    i32 height;
    u16 planes;
    u16 bitsPerPixel;
    u32 compression;
    u32 sizeofBitmap;
    i32 horizontalResolution;
    i32 verticalResolution;
    u32 colorsUsed;
    u32 colorsImportant;
    u32 redMask;
    u32 greenMask;
    u32 blueMask;
    u32 alphaMask;
    u32 colorSpaceType;
    i32 redX;
    i32 redY;
    i32 redZ;
    i32 greenX;
    i32 greenY;
    i32 greenZ;
    i32 blueX;
    i32 blueY;
    i32 blueZ;
    i32 gammaX;
    i32 gammaY;
    i32 gammaZ;
};
static_assert(sizeof(BMPHeader) == 122, "BMPHeader is not tightly packed");
#pragma pack(pop)

Image loadBMP(char* filename);

}//image