#include "DielectricMaterial.h"
#include "Ray.h"
#include "VectorHelpers.h"
#include "IHit.h"
#include "Raytracer.h"
#include <glm/glm.hpp>

using namespace rt;

static double reflectance(double cosine, double refractionIndex);

DielectricMaterial::DielectricMaterial(double refractionIndex) : refractionIndex{refractionIndex}{}

bool DielectricMaterial::scatter(const Ray& ray, const HitRecord& rec, glm::dvec3& attenuation, Ray& scattered,const Raytracer& env) const{
    attenuation = glm::dvec3{1.0,1.0,1.0};
    double ri = rec.front_face ? (1.0/refractionIndex) : refractionIndex;

    double cosTheta = std::fmin(glm::dot(-glm::normalize(ray.dir),rec.normal),1.0);
    double sinTheta = glm::sqrt(1.0 - cosTheta*cosTheta);

    glm::dvec3 direction{0,0,0};
    if(ri * sinTheta > 1.0 || reflectance(cosTheta,ri) > randomDouble()){
        direction = reflect(glm::normalize(ray.dir),rec.normal);
    }else{
        direction = refract(glm::normalize(ray.dir),rec.normal,ri);
    }

    scattered = Ray(rec.point,direction);

    return true;
}

static double reflectance(double cosine, double refractionIndex){
    double r0 = (1 - refractionIndex) / (1+ refractionIndex);
    r0 = r0 * r0;
    return r0 + (1-r0) * std::pow((1-cosine),5);
}