#pragma once
#include <glm/glm.hpp>

namespace rt{
class ITexture {
public:
    virtual ~ITexture() = default;

    virtual glm::dvec3 color(double u, double v, glm::dvec3 point) const = 0;
};
}