#pragma once
#include <cstdlib>
#include <glm/glm.hpp>

inline double randomDouble() {
    return std::rand() / (RAND_MAX + 1.0);
}
inline double randomDouble(double min, double max){
    return min + (max-min) * randomDouble();
}
inline glm::dvec3 randomVector(){
    return glm::dvec3{randomDouble(),randomDouble(),randomDouble()};
}
inline glm::dvec3 randomVector(double min, double max){
    return glm::dvec3{randomDouble(min,max),randomDouble(min,max),randomDouble(min,max)};
}
inline glm::dvec3 randomUnitVector(){
    while(true) {
        glm::dvec3 vector = randomVector(-1,1);
        double lengthsq = glm::dot(vector,vector); //this is the same as squared length
        if(1e-160 < lengthsq && lengthsq <= 1){
            return vector / sqrt(lengthsq);
        }
    }
}

inline glm::dvec3 randomUnitOnHemisphere(const glm::dvec3 normal){
    glm::dvec3 vector = randomUnitVector();

    if(glm::dot(vector,normal) > 0.0){
        return vector;
    }else{
        return -vector;
    }
}
inline glm::dvec3 reflect(const glm::dvec3 vector, const glm::dvec3 normal){
    return vector - 2 * glm::dot(vector,normal) * normal;
}
inline glm::dvec3 refract(const glm::dvec3 uv, const glm::dvec3 normal, double etaiEtat){
    double cosTheta = std::fmin(glm::dot(-uv,normal),1.0);
    glm::dvec3 rayOutPerp = etaiEtat * (uv + (cosTheta * normal));
    glm::dvec3 rayOutParallel = -glm::sqrt(std::fabs(1.0 - glm::dot(rayOutPerp,rayOutPerp))) * normal;
    return rayOutPerp + rayOutParallel;
}