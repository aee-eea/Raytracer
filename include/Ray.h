#pragma once
#include "glm/glm.hpp"
#include "IFrontend.h"

class Ray{
public:
    Ray(const glm::dvec3& origin, const glm::dvec3& direction);
    glm::dvec3 at(double t) const;
    const glm::dvec3& origin() const;
    const glm::dvec3& direction() const;
    PixColor rayColor();
private:
    glm::dvec3 orig;
    glm::dvec3 dir;
};