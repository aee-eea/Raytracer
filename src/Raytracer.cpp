#include "Raytracer.h"
#include "IFrontend.h"
#include "Ray.h"
#include "Sphere.h"
#include <limits>

Raytracer::Raytracer(const IFrontend& frontend,double viewportHeight)
    :viewportHeight{viewportHeight} {
        viewportWidth = viewportHeight * (static_cast<double>(frontend.getImageWidth()) / frontend.getImageHeight());

        viewportU = glm::dvec3(viewportWidth,0,0);
        viewportV = glm::dvec3(0,-viewportHeight,0);

        pixelDeltaU = viewportU / static_cast<double>(frontend.getImageWidth());
        pixelDeltaV = viewportV / static_cast<double>(frontend.getImageHeight());

        viewportUpperLeftPos = cameraPos
                                - glm::dvec3(0,0,focalLenght)
                                - (viewportU / 2.0)
                                - (viewportV / 2.0);
        firstPixelPos = viewportUpperLeftPos + (pixelDeltaU * 0.5) + (pixelDeltaV * 0.5);

        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{0,0,-1.0},0.5));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{0,-100.5,-1.0},100));
}

void Raytracer::render(IFrontend& frontend){
    int const imgWidth = frontend.getImageWidth();
    int const imgHeight = frontend.getImageHeight();

    for(int y = 0; y < imgHeight; y++){
        for(int x = 0; x < imgWidth; x++){
            glm::dvec3 pixelCenter = firstPixelPos + (pixelDeltaU * static_cast<double>(x)) + (pixelDeltaV * static_cast<double>(y));
            glm::dvec3 rayDir = pixelCenter - cameraPos;
            Ray ray (cameraPos, glm::normalize(rayDir));

            frontend.putPixel(x,y,ray.rayColor(hittableObjects,Interval{0,infinity}));
        }
    }
}