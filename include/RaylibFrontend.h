#pragma once
#include "raylib.h"
#include "Raytracer.h"
#include <string>
#include <vector>
#include <chrono>

class RaylibFrontend{
public:
    RaylibFrontend(Raytracer& raytracer,int windowWidth = 800, int windowHeight = 400, const std::string& windowTitle = "Raylib");
    void start();

private:
    int windowWidth;
    int windowHeight;
    std::string windowTitle;
    Raytracer& raytracer;
    Texture2D textureOnGpu;
    std::chrono::time_point<std::chrono::steady_clock> now;

    void input();
    void update();
    void render();
};