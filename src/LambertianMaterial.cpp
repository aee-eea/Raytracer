#include "Ray.h"
#include "LambertianMaterial.h"
#include "VectorHelpers.h"
#include "IHit.h"
#include "Raytracer.h"
#include "ITexture.h"
#include <glm/glm.hpp>

using namespace rt;

LambertianMaterial::LambertianMaterial(TextureHandle albedo): albedo{albedo}{}

bool LambertianMaterial::scatter(const Ray& ray, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered, const Raytracer& env) const{
    glm::dvec3 scatter_dir = rec.normal + randomUnitVector();

    if(std::fabs(scatter_dir.x) < 1e-8 && std::fabs(scatter_dir.y) < 1e-8 && std::fabs(scatter_dir.z) < 1e-8){
        scatter_dir = rec.normal;
    }

    scattered = Ray(rec.point,scatter_dir);
    attenuation = env.getAssets().getTexture(albedo).color(rec.u,rec.v,rec.point,env);
    return true;
}