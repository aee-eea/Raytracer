#pragma once
#include "glm/glm.hpp"
#include "IHit.h"
#include "Raytracer.h"
#include <vector>
#include <memory>

struct Ray{
    glm::dvec3 orig;
    glm::dvec3 dir;

    Ray(const glm::dvec3& origin, const glm::dvec3& direction);
    glm::dvec3 at(double t) const;
    glm::dvec3 rayColor(const Raytracer& env,Interval rayT,int depth) const;
};