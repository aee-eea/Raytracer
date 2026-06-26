#pragma once
#include "glm/glm.hpp"
#include "IFrontend.h"
#include "IHit.h"
#include <vector>
#include <memory>

struct Ray{
public:
    const glm::dvec3 orig;
    const glm::dvec3 dir;

    Ray(const glm::dvec3& origin, const glm::dvec3& direction);
    glm::dvec3 at(double t) const;
    glm::dvec3 rayColor(const std::vector<std::unique_ptr<IHit>>& hittableObjects,Interval rayT) const;
};