#pragma once
#include "glm/glm.hpp"
#include "Interval.h"

namespace rt{

class Raytracer;

struct Ray{
    glm::dvec3 orig;
    glm::dvec3 dir;

    Ray(glm::dvec3 origin, glm::dvec3 direction);
    glm::dvec3 at(double t) const;
    glm::dvec3 rayColor(const Raytracer& env,Interval rayT,int depth) const;
};
}