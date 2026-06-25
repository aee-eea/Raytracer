#pragma once
#include "glm/glm.hpp"
#include "IFrontend.h"
#include "IHit.h"
#include <vector>
#include <memory>

class Ray{
public:
    Ray(const glm::dvec3& origin, const glm::dvec3& direction);
    glm::dvec3 at(double t) const;
    const glm::dvec3& origin() const;
    const glm::dvec3& direction() const;
    PixColor rayColor(const std::vector<std::unique_ptr<IHit>>& hittableObjects,Interval rayT);
private:
    glm::dvec3 orig;
    glm::dvec3 dir;
};