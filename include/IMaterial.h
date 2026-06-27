#pragma once
#include "IHit.h"
#include <glm/glm.hpp>

class IMaterial {
    public:
    virtual ~IMaterial() = default;

    virtual bool scatter(const Ray& ray, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered) = 0;
};