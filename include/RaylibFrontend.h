#pragma once
#include "raylib.h"
#include <string>
#include <vector>

class RaylibFrontend{
public:
    RaylibFrontend(int windowWidth = 800, int windowHeight = 400, const std::string& windowTitle = "Raylib");
    void start();

private:
    int windowWidth;
    int windowHeight;
    std::string windowTitle;

    std::vector<Color> framebuffer;
    Texture2D textureOnGpu;

    void input();
    void update();
    void render();
    void changePixel(int x, int y, Color color);
};