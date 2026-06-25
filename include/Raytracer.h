#pragma once
#include "IFrontend.h"
#include <glm/glm.hpp>
#include "IHit.h"
#include "Camera.h"

class Raytracer{
public:
    Raytracer(const IFrontend& frontend);
    void render(IFrontend& frontend);

private:
    std::vector<std::unique_ptr<IHit>> hittableObjects;
    RaytracerCamera cam;
};