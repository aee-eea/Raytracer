#pragma once
#include <glm/glm.hpp>
#include "IHit.h"
#include "Interval.h"
#include "Handles.h"
#include "AABB.h"

namespace rt{

class Ray;
class HitRecord;

class Sphere : public IHit{
public:
    Sphere(const glm::dvec3& center, double radius, MaterialHandle matHandle);
    bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override;
    AABB boundingBox() const override;

private:
    glm::dvec3 center;
    double radius;
    MaterialHandle matHandle;
    AABB bbox;
};
}