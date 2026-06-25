#include "Camera.h"
#include "Ray.h"
#include <glm/glm.hpp>
RaytracerCamera::RaytracerCamera(int imageWidth, int imageHeight, double viewportHeight)
: viewportHeight{viewportHeight}{
        viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);

        viewportU = glm::dvec3(viewportWidth,0,0);
        viewportV = glm::dvec3(0,-viewportHeight,0);

        pixelDeltaU = viewportU / static_cast<double>(imageWidth);
        pixelDeltaV = viewportV / static_cast<double>(imageHeight);

        viewportUpperLeftPos = cameraPos
                                - glm::dvec3(0,0,focalLenght)
                                - (viewportU / 2.0)
                                - (viewportV / 2.0);
        firstPixelPos = viewportUpperLeftPos + (pixelDeltaU * 0.5) + (pixelDeltaV * 0.5);
}

PixColor RaytracerCamera::renderPixel(int x, int y, std::vector<std::unique_ptr<IHit>>& hittableObjects){
    glm::dvec3 pixelCenter = firstPixelPos + (pixelDeltaU * static_cast<double>(x)) + (pixelDeltaV * static_cast<double>(y));
    glm::dvec3 rayDir = pixelCenter - cameraPos;
    Ray ray (cameraPos, glm::normalize(rayDir));
    return ray.rayColor(hittableObjects,Interval{0,infinity});
}