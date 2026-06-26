#include "Camera.h"
#include "Ray.h"
#include "RandomFunc.h"
#include <glm/glm.hpp>
RaytracerCamera::RaytracerCamera(int imageWidth, int imageHeight, double viewportHeight, int samples, int recursionDepth)
: viewportHeight{viewportHeight}, samplesPerPixel{samples}, maxDepth{recursionDepth} {
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

glm::dvec3 RaytracerCamera::renderPixel(int x, int y, std::vector<std::unique_ptr<IHit>>& hittableObjects){
    glm::dvec3 finalColor = {0,0,0};
    

    for(int i = 0; i < samplesPerPixel; i++){
        glm::dvec3 offset = sampleSquare();
        glm::dvec3 pixelCenter = firstPixelPos + (pixelDeltaU * (static_cast<double>(x) + offset.x)) + (pixelDeltaV * (static_cast<double>(y) + offset.y));
        glm::dvec3 rayDir = pixelCenter - cameraPos;
        Ray ray (cameraPos, glm::normalize(rayDir));

        finalColor += ray.rayColor(hittableObjects,Interval{0.001, infinity},maxDepth);
    }

    finalColor *= (1.0 / samplesPerPixel);
    return finalColor;
}

glm::dvec3 RaytracerCamera::sampleSquare() const{
    return glm::dvec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
}