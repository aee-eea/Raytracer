#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "IHit.h"
#include "PixColor.h"

class RaytracerCamera{
public:
    RaytracerCamera(int imageWidth, int imageHeight, double viewportHeight = 2.0);

    PixColor renderPixel(int x, int y, std::vector<std::unique_ptr<IHit>>& hittableObjects);

private:

    double focalLenght{1.0};
    double viewportHeight;
    double viewportWidth;
    glm::dvec3 cameraPos{0,0,0};

    glm::dvec3 viewportU;
    glm::dvec3 viewportV;
    glm::dvec3 pixelDeltaU;
    glm::dvec3 pixelDeltaV;
    glm::dvec3 viewportUpperLeftPos;
    glm::dvec3 firstPixelPos;
};