#pragma once
#include "IFrontend.h"
#include <glm/glm.hpp>
#include "IHit.h"
#include <vector>
#include <memory>

class Raytracer{
public:
    Raytracer(const IFrontend& frontend, double viewportHeight = 2.0);
    void render(IFrontend& frontend);

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

    std::vector<std::unique_ptr<IHit>> hittableObjects;
};