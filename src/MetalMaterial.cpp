#include "MetalMaterial.h"
#include "VectorHelpers.h"
#include "glm/glm.hpp"
#include "Ray.h"
#include "IHit.h"

MetalMaterial::MetalMaterial(const glm::dvec3 color, double fuzz): albedo{color}, fuzz{std::fmin(fuzz,1)}{}

bool MetalMaterial::scatter(const Ray& ray, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered){
    glm::dvec3 reflected = reflect(ray.dir, rec.normal);
    reflected = glm::normalize(reflected) + (fuzz * randomUnitVector());
    scattered = Ray(rec.point, reflected);
    attenuation = albedo;
    return (glm::dot(scattered.dir,rec.normal) > 0);
}