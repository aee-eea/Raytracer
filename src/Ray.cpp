#include "Ray.h"
#include "glm/glm.hpp"
#include "IHit.h"
#include "RandomFunc.h"
#include "Interval.h"
#include <vector>
#include <cstdint>
#include <iostream>
#include <memory>

Ray::Ray(const glm::dvec3& origin, const glm::dvec3& direction): orig{origin},dir{direction}{}

glm::dvec3 Ray::at(double t) const {
    return orig + dir * t;
}

glm::dvec3 Ray::rayColor(const std::vector<std::unique_ptr<IHit>>& hittableObjects,Interval rayT, int depth) const{
    if(depth <= 0){return glm::dvec3{0,0,0};}

    glm::dvec3 outputColor{0,0,0};
    double closest = rayT.max;
    bool hitted = false;

    HitRecord rec{};

    //find the closest
    for(const auto& surface : hittableObjects){
        if(surface->hit(*this,Interval{rayT.min,closest},rec)){
            hitted = true;
            closest = rec.t;
        }
    }

    if(hitted){
        glm::dvec3 dirBounce = randomUnitOnHemisphere(rec.normal);
        Ray bounce(rec.point,dirBounce);
        return  0.5 * bounce.rayColor((hittableObjects), Interval{0.001,infinity}, depth - 1);
    }

    double blendFactor = 0.5 * (dir.y + 1.0);
    outputColor = (1.0 - blendFactor) * glm::dvec3(1.0,1.0,1.0) + blendFactor * glm::dvec3(0.5,0.7,1.0);

    return outputColor;

}