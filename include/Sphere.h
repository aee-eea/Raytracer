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
    Sphere(const glm::vec3& center, float radius, MaterialHandle matHandle);
    bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override;
    AABB boundingBox() const override;

private:
    glm::vec3 center;
    float radius;
    MaterialHandle matHandle;
    AABB bbox;
};
}