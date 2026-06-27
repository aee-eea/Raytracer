#pragma once
#include "IMaterial.h"
#include <glm/glm.hpp>

class LambertianMaterial : public IMaterial{
public:
    LambertianMaterial(glm::dvec3 albedo);
    bool scatter(const Ray& ray, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered) override;

private:
    glm::dvec3 albedo;
};