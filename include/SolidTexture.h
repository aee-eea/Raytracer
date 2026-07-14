#pragma once
#include <glm/glm.hpp>
#include "ITexture.h"

namespace rt{
class SolidTexture : public ITexture{
public:
    SolidTexture(glm::dvec3 albedo) : albedo{albedo}{}

    glm::dvec3 color(double, double, glm::dvec3,const Raytracer&) const override{
        return albedo;
    }

private:
    glm::dvec3 albedo;
};
}