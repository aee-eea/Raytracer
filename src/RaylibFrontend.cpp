#include <iostream>
#include "raylib.h"
#include "RaylibFrontend.h"
#include "Raytracer.h"
#include <chrono>

RaylibFrontend::RaylibFrontend(Raytracer& raytracer,int windowWidth, int windowHeight, const std::string& windowTitle)
 : windowWidth{windowWidth},windowHeight{windowHeight},windowTitle{windowTitle},raytracer{raytracer}
{
    InitWindow(windowWidth,windowHeight,windowTitle.c_str());
    SetTargetFPS(120);

}
void RaylibFrontend::start(){
    if(!IsWindowReady()){
        std::cout << "Error during initialization of window" << std::endl;
    }

    raytracer.launch();
    textureOnGpu = LoadTextureFromImage(GenImageColor(raytracer.getRenderWidth(),raytracer.getRenderHeight(), BLUE));
    now = std::chrono::steady_clock::now();
    while(!WindowShouldClose()){
        input();
        update();
        render();
    }
}
void RaylibFrontend::input(){}
void RaylibFrontend::update(){
    if(IsWindowResized()){
        windowHeight = GetScreenHeight();
        windowWidth = GetScreenWidth();
    }
}
void RaylibFrontend::render(){

    UpdateTexture(textureOnGpu,raytracer.getCurrentFrameBuffer().data());
    BeginDrawing();
    ClearBackground(RED);


    DrawTexturePro(
        textureOnGpu,
        Rectangle{0,0,static_cast<float>(raytracer.getRenderWidth()),static_cast<float>(raytracer.getRenderHeight())},
        Rectangle{0,0,static_cast<float>(windowWidth),static_cast<float>(windowHeight)},
        Vector2{0,0},
        0.0f,
        WHITE);
    EndDrawing();

    if(raytracer.isFrameDone()){
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
        std::cout << "Frame took: " << duration << std::endl;
        CloseWindow();
    }
}