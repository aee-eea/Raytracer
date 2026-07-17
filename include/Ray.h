#pragma once
#include "glm/glm.hpp"
#include "Interval.h"

namespace rt{

class Raytracer;

struct Ray{
    glm::vec3 orig;
    glm::vec3 dir;

    Ray(glm::vec3 origin, glm::vec3 direction);
    glm::vec3 at(float t) const;
    glm::dvec3 rayColor(const Raytracer& env,Interval rayT,int depth,int& rayCounter) const;
};
}