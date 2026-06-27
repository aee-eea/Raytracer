#pragma once
#include "IMaterial.h"
#include <glm/glm.hpp>

class DielectricMaterial : public IMaterial{
public:
    DielectricMaterial(double refractionIndex);
    bool scatter(const Ray& ray, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered) override;

private:
    double refractionIndex;
};