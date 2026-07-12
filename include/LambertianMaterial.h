#pragma once
#include "IMaterial.h"
#include "Handles.h"
#include <glm/glm.hpp>

namespace rt{
class Ray;
class HitRecord;
class Raytracer;

class LambertianMaterial : public IMaterial{
public:
    LambertianMaterial(TextureHandle albedo);
    bool scatter(const Ray& ray, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered, const Raytracer& env) override;

private:
    TextureHandle albedo;
};
}