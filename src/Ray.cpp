#include "Ray.h"
#include "glm/glm.hpp"
#include "IHit.h"
#include <vector>
#include <cstdint>
#include <iostream>
#include <memory>

Ray::Ray(const glm::dvec3& origin, const glm::dvec3& direction): orig{origin},dir{direction}{}

glm::dvec3 Ray::at(double t) const {
    return orig + dir * t;
}

const glm::dvec3& Ray::origin() const  { return orig; }

const glm::dvec3& Ray::direction() const { return dir; }

PixColor Ray::rayColor(const std::vector<std::unique_ptr<IHit>>& hittableObjects,Interval rayT){
    glm::dvec3 outputColor{0,0,0};
    double closest = rayT.max;

    double blendFactor = 0.5 * (dir.y + 1.0);
    outputColor = (1.0 - blendFactor) * glm::dvec3(1.0,1.0,1.0) + blendFactor * glm::dvec3(0.5,0.7,1.0);

    for(const auto& surface : hittableObjects){
        HitRecord rec{};
        if(surface->hit(*this,Interval{rayT.min,closest},rec)){
            closest = rec.t;
            outputColor = 0.5 * glm::dvec3{rec.normal.x + 1.0,rec.normal.y + 1.0,rec.normal.z + 1.0};
        }
    }
    return PixColor{.r = static_cast<uint8_t>(outputColor.x * 255.999),.g = static_cast<uint8_t>(outputColor.y * 255.999),.b = static_cast<uint8_t>(outputColor.z * 255.999), .a = 255};

}