#pragma once
#include <cstdint>

struct PixColor{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

class IFrontend{
public:
    virtual ~IFrontend() = default;
    virtual void putPixel(int x, int y, PixColor color) = 0;
    virtual int getImageWidth() const = 0;
    virtual int getImageHeight() const = 0;
};