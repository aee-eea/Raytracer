#include <glm/glm.hpp>
#include "Ray.h"
#include "IHit.h"

using namespace rt;

void HitRecord::setFaceNormal(const Ray& ray, const glm::dvec3 outwardNormal){
    front_face = dot(ray.dir, outwardNormal) < 0;
    normal = front_face ? outwardNormal : -outwardNormal;
}