#pragma once
#include <glm/glm.hpp>

namespace rt{
class Ray;
class HitRecord;
class Raytracer;

class IMaterial {
    public:
    virtual ~IMaterial() = default;

    virtual bool scatter(const Ray& ray, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered, const Raytracer& env) = 0;
};
}