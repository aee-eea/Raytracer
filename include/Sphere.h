#pragma once
#include <glm/glm.hpp>
#include "IHit.h"
#include "AssetManager.h"

class Sphere : public IHit{
public:
    Sphere(const glm::dvec3& center, double radius, MaterialHandle matHandle);
    bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override;

private:
    glm::dvec3 center;
    double radius;
    MaterialHandle matHandle;
};