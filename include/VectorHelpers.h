#pragma once
#include <cstdlib>
#include <glm/glm.hpp>

namespace rt{

inline float randomFloat(){
    thread_local static uint64_t state = 67172645466775252ull;

    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;

    return static_cast<float>((state >> 40) * (1.0f / 16777216.0f));    
}
inline float randomFloat(float min, float max){
    return min + (max-min) * randomFloat();
}
inline glm::vec3 randomVector(){
    return glm::vec3{randomFloat(),randomFloat(),randomFloat()};
}
inline glm::vec3 randomVector(float min, float max){
    return glm::vec3{randomFloat(min,max),randomFloat(min,max),randomFloat(min,max)};
}
inline glm::vec3 randomUnitVector(){
    while(true) {
        glm::vec3 vector = randomVector(-1,1);
        float lengthsq = glm::dot(vector,vector); //this is the same as squared length
        if(1e-160 < lengthsq && lengthsq <= 1){
            return vector / static_cast<float>(sqrt(lengthsq));
        }
    }
}

inline glm::vec3 randomUnitOnHemisphere(const glm::vec3 normal){
    glm::vec3 vector = randomUnitVector();

    if(glm::dot(vector,normal) > 0.0){
        return vector;
    }else{
        return -vector;
    }
}
inline glm::vec3 randomInUnitDisk(){
    while(true){
        glm::vec3 point = glm::vec3(randomFloat(-1,1),randomFloat(-1,1),0);
        if(glm::dot(point,point) < 1){
            return point;
        }
    }
}
inline glm::vec3 reflect(const glm::vec3 vector, const glm::vec3 normal){
    return vector - 2 * glm::dot(vector,normal) * normal;
}
inline glm::vec3 refract(const glm::vec3 uv, const glm::vec3 normal, float etaiEtat){
    float cosTheta = std::fmin(glm::dot(-uv,normal),1.0);
    glm::vec3 rayOutPerp = etaiEtat * (uv + (cosTheta * normal));
    glm::vec3 rayOutParallel = static_cast<float>(-glm::sqrt(std::fabs(1.0 - glm::dot(rayOutPerp,rayOutPerp)))) * normal;
    return rayOutPerp + rayOutParallel;
}
inline glm::vec3 sampleSquare(){
    return glm::vec3(randomFloat() - 0.5, randomFloat() - 0.5, 0);
}

inline uint8_t toByte(float value) {
    if (value <= 0.0){ return 0; }
    if( value >= 1.0){ return 255; }
    return static_cast<uint8_t>(256.0 * value);
}

inline int randomInt(int min, int max) {
    return int(randomFloat(min, max+1));
}
}