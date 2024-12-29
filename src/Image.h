#pragma once
#include <iostream>

const int IMAGE_DEFAULT_SIZE = 800;
const int IMAGE_DEFAULT_COMPS = 4;

class Image
{
private:
    unsigned char *buffer; // Pointer to hold the pixel data
    int height;            // Height of the image
    int width;             // Width of the image
    int comps;             // Number of components per pixel (e.g., 4 for RGBA)

public:
    Image();
    Image(int height, int width, int comps);
    ~Image();
    int getHeight() const;
    int getWidth() const;
    int getComps() const;
    unsigned char *getBuffer() const;
};
