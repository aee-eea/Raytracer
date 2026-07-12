#pragma once
#include "raylib.h"
#include <string>
#include <vector>
#include <chrono>

namespace rt{

class Raytracer;

class RaylibFrontend{
public:
    RaylibFrontend(Raytracer& raytracer,int windowWidth = 800, int windowHeight = 400, const std::string& windowTitle = "Raylib");
    ~RaylibFrontend();
    void start();

private:
    int windowWidth;
    int windowHeight;
    std::string windowTitle;
    Raytracer& raytracer;
    Texture2D textureOnGpu;
    std::chrono::time_point<std::chrono::steady_clock> now;
    double cameraMovSpeed{10.0};
    double cameraRotSpeed{30.0};
    bool notifyAboutFrameCompletion{true};

    void input();
    void update();
    void render();
};
}