#pragma once
#include "ITexture.h"
#include <glm/glm.hpp>
#include "Handles.h"
#include "Raytracer.h"

namespace rt{

class ImageTexture: public ITexture{
public:
    ImageTexture(ImageHandle handle): handle{handle}{}

    glm::dvec3 color(double u, double v, glm::dvec3 point, const Raytracer& env) const override{
        u = Interval(0,1).clamp(u);
        v = 1.0 - Interval(0,1).clamp(v);

        int i = static_cast<int>(u * env.getAssets().getImage(handle).getWidth());
        int j = static_cast<int>(v * env.getAssets().getImage(handle).getHeight());

        return env.getAssets().getImage(handle).getColor(i,j);
    }

private:
    ImageHandle handle;
};
}