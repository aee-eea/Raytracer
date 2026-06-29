#include <iostream>
#include "raylib.h"
#include "RaylibFrontend.h"
#include "Raytracer.h"
#include <chrono>

RaylibFrontend::RaylibFrontend(int windowWidth, int windowHeight,int renderWidth, int renderHeight, const std::string& windowTitle)
 : windowWidth{windowWidth},windowHeight{windowHeight},renderWidth{renderWidth},renderHeight{renderHeight},windowTitle{windowTitle}
{
    InitWindow(windowWidth,windowHeight,windowTitle.c_str());
    framebuffer.resize(renderWidth * renderHeight);
    textureOnGpu = LoadTextureFromImage(GenImageColor(renderWidth,renderHeight, BLUE));
    SetTargetFPS(60);
}
void RaylibFrontend::start(Raytracer& raytracer){
    if(!IsWindowReady()){
        std::cout << "Error during initialization of window" << std::endl;
    }

    while(!WindowShouldClose()){
        input();
        update();
        render(raytracer);
    }
}
void RaylibFrontend::input(){}
void RaylibFrontend::update(){
    if(IsWindowResized()){
        windowHeight = GetScreenHeight();
        windowWidth = GetScreenWidth();
    }
}
void RaylibFrontend::render(Raytracer& raytracer){
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    raytracer.render(*this);
    std::chrono::high_resolution_clock::time_point after = std::chrono::high_resolution_clock::now();
    std::cout << "frame took " << std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(after - now).count()) << std::endl;
    UpdateTexture(textureOnGpu,framebuffer.data());

    BeginDrawing();
    ClearBackground(RED);

    DrawTexturePro(
        textureOnGpu,
        Rectangle{0,0,static_cast<float>(renderWidth),static_cast<float>(renderHeight)},
        Rectangle{0,0,static_cast<float>(windowWidth),static_cast<float>(windowHeight)},
        Vector2{0,0},
        0.0f,
        WHITE);
    EndDrawing();
}

void RaylibFrontend::putPixel(int x, int y, PixColor color){
    framebuffer[x + (y * renderWidth)] = color;
}

int RaylibFrontend::getImageWidth() const{
    return renderWidth;
}
int RaylibFrontend::getImageHeight() const{
    return renderHeight;
}