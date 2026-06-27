#pragma once
#include "IFrontend.h"
#include <glm/glm.hpp>
#include "IHit.h"
#include "Camera.h"
#include "AssetManager.h"

class Raytracer{
public:
    Raytracer(const IFrontend& frontend);
    void render(IFrontend& frontend);
    const std::vector<std::unique_ptr<IHit>>& getHittables() const;
    const AssetManager& getAssets() const;
private:
    std::vector<std::unique_ptr<IHit>> hittableObjects;
    RaytracerCamera cam;
    AssetManager assets;
};