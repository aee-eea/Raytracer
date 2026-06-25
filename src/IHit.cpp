#include <glm/glm.hpp>
#include "Ray.h"

void HitRecord::setFaceNormal(const Ray& ray, const glm::dvec3 outwardNormal){
    front_face = dot(ray.direction(), outwardNormal) < 0;
    normal = front_face ? outwardNormal : -outwardNormal;
}