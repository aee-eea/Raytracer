#include "Image.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//fixes collision with raylib's stb_image 

#include <iostream>
#include <glm/glm.hpp>
#include <filesystem>
#include <fstream>

static int clamp(int x, int low, int high);

using namespace rt;

Image::~Image(){
    if(fdata){
        stbi_image_free(fdata);
    }
}

bool Image::load(const std::string& filePath){
    int n = 0;
    fdata = stbi_loadf(filePath.c_str(),&imageWidth,&imageHeight,&n,floatsPerPixel);
    std::cout << std::filesystem::absolute(filePath) << std::endl;
    if(!fdata){std::cout << filePath << " " << stbi_failure_reason() << std::endl; return false;}
    if(n != floatsPerPixel){std::cout << "Wrong format, expected:  " << floatsPerPixel << " received: " << n << std::endl; return false;}

    floatsPerScanline = imageWidth * floatsPerPixel;
    return true;
}
glm::dvec3 Image::getColor(int x, int y) const{
    if(!fdata){ return glm::dvec3{1,0,1};}

    x = clamp(x,0,imageWidth);
    y = clamp(y,0,imageHeight);

    double colorR = fdata[y*floatsPerScanline + x * floatsPerPixel];
    double colorG = fdata[y*floatsPerScanline + x * floatsPerPixel + 1];
    double colorB = fdata[y*floatsPerScanline + x * floatsPerPixel + 2];

    return glm::dvec3{colorR,colorG,colorB};
}

int Image::getHeight() const{
    return (fdata) ? imageHeight : 0;
}
int Image::getWidth() const{
    return (fdata) ? imageWidth : 0;
}

Image::Image(Image&& move) noexcept
    :fdata{move.fdata},
    imageHeight{move.imageHeight},
    imageWidth{move.imageWidth},
    floatsPerScanline{move.floatsPerScanline}{
        move.fdata = nullptr;
        move.imageHeight = 0;
        move.imageWidth = 0;
        move.floatsPerScanline = 0;
}
Image& rt::Image::operator=(Image&& move) noexcept{
    if(this != &move){
        stbi_image_free(fdata);
        fdata = move.fdata;
        imageHeight = move.imageHeight;
        imageWidth = move.imageWidth;
        floatsPerScanline = move.floatsPerScanline;

        move.fdata = nullptr;
        move.imageHeight = 0;
        move.imageWidth = 0;
        move.floatsPerScanline = 0;
    }
    return *this;
}


static int clamp(int x, int low, int high) {
    // [low, high).
    if (x < low) return low;
    if (x < high) return x;
    return high - 1;
}