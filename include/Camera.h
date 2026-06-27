#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "IHit.h"
#include "VectorHelpers.h"

class Raytracer;// circular dependency

class RaytracerCamera{
public:
    RaytracerCamera(int imageWidth, int imageHeight, double viewportHeight = 2.0, int samples=10,int recursionDepth=50);

    glm::dvec3 renderPixel(int x, int y, const Raytracer& env);

    glm::dvec3 sampleSquare() const;

private:

    double focalLenght{1.0};
    double viewportHeight;
    double viewportWidth;
    int samplesPerPixel{1};
    int maxDepth{50};
    glm::dvec3 cameraPos{0,0,0};

    glm::dvec3 viewportU;
    glm::dvec3 viewportV;
    glm::dvec3 pixelDeltaU;
    glm::dvec3 pixelDeltaV;
    glm::dvec3 viewportUpperLeftPos;
    glm::dvec3 firstPixelPos;
};