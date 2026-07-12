#pragma once
#include <glm/glm.hpp>
#include <Interval.h>
#include "Handles.h"

namespace rt{
class Ray;
class HitRecord;


struct HitRecord{
    glm::dvec3 point;
    glm::dvec3 normal;
    double t;
    bool front_face;
    MaterialHandle material;
    double u;
    double v;

    void setFaceNormal(const Ray& ray, const glm::dvec3 outwardNormal);
};

class IHit{
public:
    virtual ~IHit() = default;
    virtual bool hit(const Ray& ray, Interval rayT, HitRecord& record) const = 0;
};
}