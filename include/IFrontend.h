#pragma once
#include <cstdint>
#include "PixColor.h"

class IFrontend{
public:
    virtual ~IFrontend() = default;
    virtual void putPixel(int x, int y, PixColor color) = 0;
    virtual int getImageWidth() const = 0;
    virtual int getImageHeight() const = 0;
};