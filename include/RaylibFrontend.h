#pragma once
#include "raylib.h"
#include "IFrontend.h"
#include "Raytracer.h"
#include <string>
#include <vector>

class RaylibFrontend : public IFrontend{
public:
    RaylibFrontend(int windowWidth = 800, int windowHeight = 400, int renderWidth = 800, int renderHeight = 400, const std::string& windowTitle = "Raylib");
    void start(Raytracer& raytracer);
    void putPixel(int x, int y, PixColor color) override;
    int getImageWidth() const override;
    int getImageHeight() const override;

private:
    int windowWidth;
    int windowHeight;
    int renderWidth;
    int renderHeight;
    std::string windowTitle;

    std::vector<PixColor> framebuffer;
    Texture2D textureOnGpu;

    void input();
    void update();
    void render(Raytracer& raytracer);
};