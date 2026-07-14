#pragma once
#include <string>
#include <glm/glm.hpp>

namespace rt{

class Image{
public:
    Image() = default;
    ~Image();

    bool load(const std::string& filePath);
    glm::dvec3 getColor(int x, int y) const;
    int getHeight() const;
    int getWidth() const;

    Image(const Image& copy)=delete;
    Image& operator=(const Image& copy)=delete;
    Image(Image&& move) noexcept;
    Image& operator=(Image&& move) noexcept;


private:
    const int floatsPerPixel{3};
    float* fdata{nullptr};
    int imageWidth{0};
    int imageHeight{0};
    int floatsPerScanline{0};
};
}
