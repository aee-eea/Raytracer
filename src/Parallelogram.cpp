#include "Parallelogram.h"
#include <glm/glm.hpp>

using namespace rt;

Parallelogram::Parallelogram(glm::vec3 startPoint, glm::vec3 u, glm::vec3 v, MaterialHandle matHandle)
 :startPoint{startPoint}, u{u}, v{v}, matHandle{matHandle} {
    normal = glm::normalize(glm::cross(u,v));
    D = glm::dot(normal,startPoint);
    w = glm::cross(u,v) / glm::dot(glm::cross(u,v),glm::cross(u,v));

    AABB diagonal1 = AABB(startPoint, startPoint + u + v);
    AABB diagonal2 = AABB(startPoint + u, startPoint + v);
    bbox = AABB(diagonal1,diagonal2);
}
bool Parallelogram::hit(const Ray& ray, Interval rayT, HitRecord& record) const{
    float denom = glm::dot(normal, ray.dir);

    if(std::fabs(denom) < 1e-8){ return false;}
    float t = (D - glm::dot(normal,ray.orig)) / denom;
    if(!rayT.contains(t)){
        return false;
    }

    glm::vec3 intersection = ray.at(t);
    glm::vec3 planarHitPoint = intersection - startPoint;
    float alpha = glm::dot(w, glm::cross(planarHitPoint,v));
    float beta = glm::dot(w, glm::cross(u,planarHitPoint));

    Interval unit = Interval(0,1);
    if(!(unit.contains(alpha) && unit.contains(beta))){
        return false;
    }

    record.u = alpha;
    record.v = beta;
    record.t = t;
    record.point = intersection;
    record.material = matHandle;
    record.setFaceNormal(ray,normal);
    return true;
}
AABB Parallelogram::boundingBox() const{
    return bbox;
}