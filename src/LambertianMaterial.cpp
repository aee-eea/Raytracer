#include "Ray.h"
#include "LambertianMaterial.h"
#include "VectorHelpers.h"
#include <glm/glm.hpp>

LambertianMaterial::LambertianMaterial(glm::dvec3 albedo): albedo{albedo}{}

bool LambertianMaterial::scatter(const Ray& ray, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered){
    glm::dvec3 scatter_dir = rec.normal + randomUnitVector();

    //in case scatter_dir generates a dir that is exactly opposite the normal which would lead to weird things
    if(std::fabs(scatter_dir.x) < 1e-8 && std::fabs(scatter_dir.y) < 1e-8 && std::fabs(scatter_dir.z) < 1e-8){
        scatter_dir = rec.normal;
    }

    scattered = Ray(rec.point,scatter_dir);
    attenuation = albedo;
    return true;
}