#pragma once
#include "IHit.h"
#include <glm/glm.hpp>
#include "Handles.h"
#include "AABB.h"

namespace rt{
class Parallelogram : public IHit{
public:
    Parallelogram(glm::vec3 startPoint, glm::vec3 u, glm::vec3 v, MaterialHandle matHandle);
    bool hit(const Ray& ray, Interval rayT, HitRecord& record) const;
    AABB boundingBox() const;

private:
    glm::vec3 startPoint; 
    glm::vec3 u,v;
    MaterialHandle matHandle;
    AABB bbox;
    glm::vec3 normal;
    float D;
    glm::vec3 w;
};
}