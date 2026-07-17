#pragma once
#include <glm/glm.hpp>
#include "IMaterial.h"
#include "Handles.h"

namespace rt{
class Ray;
class HitRecord;
class Raytracer;

class MetalMaterial : public IMaterial{
public:
    MetalMaterial(TextureHandle albedo, float fuzz);
    bool scatter(const Ray& ray, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered, const Raytracer& env) const override;

private:
    TextureHandle albedo;
    float fuzz;
};
}