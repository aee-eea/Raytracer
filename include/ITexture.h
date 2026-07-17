#pragma once
#include <glm/glm.hpp>

namespace rt{
class Raytracer;

class ITexture {
public:
    virtual ~ITexture() = default;

    virtual glm::dvec3 color(float u, float v, glm::vec3 point, const Raytracer& env) const = 0;
};
}