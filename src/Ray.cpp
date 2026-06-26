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

glm::dvec3 Ray::rayColor(const std::vector<std::unique_ptr<IHit>>& hittableObjects,Interval rayT) const{
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
    return outputColor;

}