#include "Raytracer.h"
#include "IFrontend.h"
#include "Ray.h"
#include "Sphere.h"
#include <limits>
#include "Camera.h"
#include "PixColor.h"
#include "Handles.h"
#include "LambertianMaterial.h"
#include "MetalMaterial.h"
#include "DielectricMaterial.h"

static double linearToGamma(double linear);


Raytracer::Raytracer(const IFrontend& frontend)
    : cam(frontend.getImageWidth(),frontend.getImageHeight()) {
        MaterialHandle ground = assets.addMaterial<LambertianMaterial>(glm::dvec3{0.8,0.8,0});
        MaterialHandle center = assets.addMaterial<LambertianMaterial>(glm::dvec3{0.1,0.2,0.5});
        MaterialHandle left = assets.addMaterial<DielectricMaterial>(1.50);
        MaterialHandle bubble = assets.addMaterial<DielectricMaterial>(1.00/1.50);
        MaterialHandle right = assets.addMaterial<MetalMaterial>(glm::dvec3{0.8,0.8,0.8},0.2);

        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{0,-100.5,-1},100,ground));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{0,0,-1.2},0.5,center));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{-1,0,-1},0.5,left));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{-1.0,0,-1.0},0.4,bubble));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{1.0,0,-1},0.5,right));
}

void Raytracer::render(IFrontend& frontend){
    int const imgWidth = frontend.getImageWidth();
    int const imgHeight = frontend.getImageHeight();
    static const Interval intensity(0.000,0.999);

    for(int y = 0; y < imgHeight; y++){
        for(int x = 0; x < imgWidth; x++){
            glm::dvec3 pixelColor = cam.renderPixel(x,y,*this);

            pixelColor.x = linearToGamma(pixelColor.x);
            pixelColor.y = linearToGamma(pixelColor.y);
            pixelColor.z = linearToGamma(pixelColor.z);

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



static double linearToGamma(double linear){
    if(linear > 0){
        return glm::sqrt(linear);
    }
    return 0;
}

const std::vector<std::unique_ptr<IHit>>& Raytracer::getHittables() const{
    return hittableObjects;
}
const AssetManager& Raytracer::getAssets() const{
    return assets;
}