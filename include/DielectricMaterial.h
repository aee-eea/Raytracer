#pragma once
#include <glm/glm.hpp>
#include "IMaterial.h"

namespace rt{
    
class Ray;
class HitRecord;
class Raytracer;




class DielectricMaterial : public IMaterial{
public:
    DielectricMaterial(float refractionIndex);
    bool scatter(const Ray& ray, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered,const Raytracer& env) const override;

private:
    float refractionIndex;
};
}