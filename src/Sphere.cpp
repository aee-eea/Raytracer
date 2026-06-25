#include "Sphere.h"
#include "IHit.h"
#include "Ray.h"

Sphere::Sphere(const glm::dvec3& center, double radius): center{center},radius{radius}{}

bool Sphere::hit(const Ray& ray, Interval rayT, HitRecord& record) const{
    glm::dvec3 oc = center - ray.origin();
    double a = glm::dot(ray.direction(), ray.direction());
    double b = -2.0 * glm::dot(ray.direction(), oc);
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
    return true;
}