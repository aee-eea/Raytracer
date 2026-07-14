#include "Sphere.h"
#include "IHit.h"
#include "Ray.h"
#include "IMaterial.h"
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>

using namespace rt;

Sphere::Sphere(const glm::dvec3& center, double radius, MaterialHandle materialHandle)
 :  center{center},radius{std::fmax(0,radius)}, matHandle{materialHandle}{}

bool Sphere::hit(const Ray& ray, Interval rayT, HitRecord& record) const{
    glm::dvec3 oc = center - ray.orig;
    glm::dvec3 dir = ray.dir;
    double a = glm::dot(dir,dir);
    double b = -2.0 * glm::dot(dir, oc);
    double c = glm::dot(oc,oc) - (radius * radius);

    double discriminant = b*b - 4*a*c;

    if(discriminant < 0){return false;}

    double sqrtd = glm::sqrt(discriminant);

    double root = (-b - sqrtd) / (2.0 * a);
    if(!rayT.surrounds(root)){
        root = (-b + sqrtd) / (2.0 * a);
        if(!rayT.surrounds(root)){ return false; }
    }
    
    record.t = root;
    record.point = ray.at(record.t);
    glm::dvec3 outwardNormal = (record.point - center) / radius;
    record.setFaceNormal(ray, outwardNormal);
    record.material = matHandle;


    double theta = glm::acos(outwardNormal.y);
    double phi = glm::atan(-outwardNormal.z,outwardNormal.x) + glm::pi<double>();

    record.u = phi / (2 * glm::pi<double>());
    record.v = theta / glm::pi<double>();
    return true;
}