#include "Ray.h"
#include "glm/glm.hpp"
#include "IHit.h"
#include "VectorHelpers.h"
#include "Interval.h"
#include "IMaterial.h"
#include "Raytracer.h"
#include <vector>
#include <cstdint>
#include <iostream>
#include <memory>

Ray::Ray(glm::dvec3 origin, glm::dvec3 direction): orig{origin},dir{direction}{}

glm::dvec3 Ray::at(double t) const {
    return orig + dir * t;
}

glm::dvec3 Ray::rayColor(const Raytracer& env,Interval rayT, int depth) const{
    if(depth <= 0){return glm::dvec3{0,0,0};}

    glm::dvec3 outputColor{0,0,0};
    double closest = rayT.max;
    bool hitted = false;

    HitRecord rec{};

    //find the closest
    for(const auto& surface : env.getHittables()){
        if(surface->hit(*this,Interval{rayT.min,closest},rec)){
            hitted = true;
            closest = rec.t;
        }
    }

    if(hitted){
        Ray scattered(glm::dvec3{0,0,0},glm::dvec3{0,0,0});
        glm::dvec3 attentuation{0,0,0};
        if(env.getAssets().getMaterial(rec.material).scatter(*this,rec,attentuation,scattered,env)){
            Ray nextRay(scattered.orig, scattered.dir);
            return attentuation * nextRay.rayColor(env,Interval{0.001,infinity},depth - 1);
        }
    }

    double blendFactor = 0.5 * (dir.y + 1.0);
    outputColor = (1.0 - blendFactor) * glm::dvec3(1.0,1.0,1.0) + blendFactor * glm::dvec3(0.5,0.7,1.0);

    return outputColor;

}