#include "Image.h"
#include <cstdlib>
#include <stb/stb_image.h>

Image::Image() : height(IMAGE_DEFAULT_SIZE), width(IMAGE_DEFAULT_SIZE), comps(IMAGE_DEFAULT_COMPS)
{
    buffer = static_cast<unsigned char *>(malloc(height * width * comps));
}

Image::Image(int height, int width, int comps) : height(height), width(width), comps(comps)
{
    buffer = static_cast<unsigned char *>(malloc(height * width * comps));
}

Image::~Image()
{
    if (buffer)
    {
        free(buffer);
        buffer = nullptr;
    }
}

unsigned char *Image::getBuffer() const { return buffer; }
int Image::getHeight() const { return height; }
int Image::getWidth() const { return width; }
int Image::getComps() const { return comps; }