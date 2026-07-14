#pragma once
#include "ITexture.h"
#include <glm/glm.hpp>
#include "Handles.h"
#include "Raytracer.h"

namespace rt{

class CheckeredTexture: public ITexture{
public:
    CheckeredTexture(double scale, TextureHandle even, TextureHandle odd): inverseScale{1.0/scale}, even{even}, odd{odd}{}

    glm::dvec3 color(double u, double v, glm::dvec3 point, const Raytracer& env) const override{
        int xInt = static_cast<int>(glm::floor(inverseScale * point.x));
        int yInt = static_cast<int>(glm::floor(inverseScale * point.y));
        int zInt = static_cast<int>(glm::floor(inverseScale * point.z));

        bool isEven = (xInt + yInt + zInt) % 2 == 0;

        return isEven ? env.getAssets().getTexture(even).color(u,v,point,env) : env.getAssets().getTexture(odd).color(u,v,point,env);
    }
private:

    double inverseScale;
    TextureHandle even;
    TextureHandle odd;
};
}