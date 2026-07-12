#include "MetalMaterial.h"
#include "VectorHelpers.h"
#include "glm/glm.hpp"
#include "Ray.h"
#include "IHit.h"
#include "Handles.h"
#include "ITexture.h"
#include "Raytracer.h"

MetalMaterial::MetalMaterial(TextureHandle albedo, double fuzz): albedo{albedo}, fuzz{std::fmin(fuzz,1)}{}

bool MetalMaterial::scatter(const Ray& ray, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered, const Raytracer& env){
    glm::dvec3 reflected = reflect(ray.dir, rec.normal);
    reflected = glm::normalize(reflected) + (fuzz * randomUnitVector());
    scattered = Ray(rec.point, reflected);
    attenuation = env.getAssets().getTexture(albedo).color(0,0,{0,0,0});
    return (glm::dot(scattered.dir,rec.normal) > 0);
}