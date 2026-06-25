#include "Raytracer.h"
#include "IFrontend.h"
#include "Ray.h"
#include "Sphere.h"
#include <limits>
#include "Camera.h"

Raytracer::Raytracer(const IFrontend& frontend)
    : cam(frontend.getImageWidth(),frontend.getImageHeight(),2.0) {
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{0,0,-1.0},0.5));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{0,-100.5,-1.0},100));
}

void Raytracer::render(IFrontend& frontend){
    int const imgWidth = frontend.getImageWidth();
    int const imgHeight = frontend.getImageHeight();

    for(int y = 0; y < imgHeight; y++){
        for(int x = 0; x < imgWidth; x++){
            frontend.putPixel(x,y,cam.renderPixel(x,y,hittableObjects));
        }
    }
}