#include "Raytracer.h"
#include "IFrontend.h"
#include "Ray.h"
#include "Sphere.h"
#include <limits>
#include "Camera.h"
#include "PixColor.h"

Raytracer::Raytracer(const IFrontend& frontend)
    : cam(frontend.getImageWidth(),frontend.getImageHeight(),2.0,10) {
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{0,0,-1.0},0.5));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{0,-100.5,-1.0},100));
}

void Raytracer::render(IFrontend& frontend){
    int const imgWidth = frontend.getImageWidth();
    int const imgHeight = frontend.getImageHeight();
    static const Interval intensity(0.000,0.999);

    for(int y = 0; y < imgHeight; y++){
        for(int x = 0; x < imgWidth; x++){
            glm::dvec3 pixelColor = cam.renderPixel(x,y,hittableObjects);
            pixelColor.x = intensity.clamp(pixelColor.x);
            pixelColor.y = intensity.clamp(pixelColor.y);
            pixelColor.z = intensity.clamp(pixelColor.z);
            frontend.putPixel(x,y,PixColor{.r = static_cast<uint8_t>(256 * pixelColor.x),
                                            .g = static_cast<uint8_t>(256 * pixelColor.y),
                                            .b = static_cast<uint8_t>(256 * pixelColor.z),
                                            .a = 255});
        }
    }
}