#pragma once
#include <glm/glm.hpp>
#include "IMaterial.h"

namespace rt{
    
class Ray;
class HitRecord;
class Raytracer;




class DielectricMaterial : public IMaterial{
public:
    DielectricMaterial(double refractionIndex);
    bool scatter(const Ray& ray, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered,const Raytracer& env) override;

private:
    double refractionIndex;
};
}