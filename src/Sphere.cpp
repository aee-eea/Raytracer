#include "Sphere.h"
#include "IHit.h"
#include "Ray.h"
#include "IMaterial.h"
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>

using namespace rt;

Sphere::Sphere(const glm::vec3& center, float radius, MaterialHandle materialHandle)
  : center{center} ,radius{std::fmax(0.0f,radius)}, matHandle{materialHandle}{
    glm::vec3 rvec{radius,radius,radius};
    bbox = AABB(center - rvec, center + rvec);
 }

bool Sphere::hit(const Ray& ray, Interval rayT, HitRecord& record) const{
    glm::vec3 oc = center - ray.orig;
    glm::vec3 dir = ray.dir;
    float a = glm::dot(dir,dir);
    float b = -2.0 * glm::dot(dir, oc);
    float c = glm::dot(oc,oc) - (radius * radius);

    float discriminant = b*b - 4*a*c;

    if(discriminant < 0){return false;}

    float sqrtd = glm::sqrt(discriminant);

    float root = (-b - sqrtd) / (2.0 * a);
    if(!rayT.surrounds(root)){
        root = (-b + sqrtd) / (2.0 * a);
        if(!rayT.surrounds(root)){ return false; }
    }
    
    record.t = root;
    record.point = ray.at(record.t);
    glm::vec3 outwardNormal = (record.point - center) / radius;
    record.setFaceNormal(ray, outwardNormal);
    record.material = matHandle;


    float theta = glm::acos(outwardNormal.y);
    float phi = glm::atan(-outwardNormal.z,outwardNormal.x) + glm::pi<float>();

    record.u = phi / (2 * glm::pi<float>());
    record.v = theta / glm::pi<float>();
    return true;
}

AABB Sphere::boundingBox() const{
    return bbox;
}