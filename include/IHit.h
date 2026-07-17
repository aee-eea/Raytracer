#pragma once
#include <glm/glm.hpp>
#include <Interval.h>
#include "Handles.h"
#include "Ray.h"

namespace rt{
class HitRecord;
class AABB;


struct HitRecord{
    glm::vec3 point;
    glm::vec3 normal;
    float t;
    bool front_face;
    MaterialHandle material;
    float u;
    float v;

    void setFaceNormal(const Ray& ray, const glm::vec3 outwardNormal){
        front_face = dot(ray.dir, outwardNormal) < 0;
        normal = front_face ? outwardNormal : -outwardNormal;
    }
};

class IHit{
public:
    virtual ~IHit() = default;
    virtual bool hit(const Ray& ray, Interval rayT, HitRecord& record) const = 0;
    virtual AABB boundingBox() const = 0;
};
}