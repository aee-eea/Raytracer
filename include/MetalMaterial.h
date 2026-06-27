#pragma once
#include <glm/glm.hpp>
#include "IMaterial.h"

class MetalMaterial : public IMaterial{
public:
    MetalMaterial(const glm::dvec3 color, double fuzz);
    bool scatter(const Ray& ray, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered) override;

private:
    glm::dvec3 albedo;
    double fuzz;
};