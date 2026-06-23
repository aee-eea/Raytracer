#include <iostream>
#include "raylib.h"
#include "RaylibFrontend.h"

RaylibFrontend::RaylibFrontend(int windowWidth, int windowHeight, const std::string& windowTitle)
 : windowWidth{windowWidth},windowHeight{windowHeight},windowTitle{windowTitle}
{
    InitWindow(windowWidth,windowHeight,windowTitle.c_str());
    framebuffer.resize(windowWidth * windowHeight);
    textureOnGpu = LoadTextureFromImage(GenImageColor(windowWidth, windowHeight, BLUE));
}
void RaylibFrontend::start(){
    if(!IsWindowReady()){
        std::cout << "Error during initialization of window" << std::endl;
    }

    while(!WindowShouldClose()){
        input();
        update();
        render();
    }
}
void RaylibFrontend::input(){}
void RaylibFrontend::update(){
}
void RaylibFrontend::render(){
    UpdateTexture(textureOnGpu,framebuffer.data());

    BeginDrawing();
    ClearBackground(RED);

    DrawTexturePro(
        textureOnGpu,
        Rectangle{0,0,static_cast<float>(windowWidth),static_cast<float>(-windowHeight)},
        Rectangle{0,0,static_cast<float>(windowWidth),static_cast<float>(windowHeight)},
        Vector2{0,0},
        0.0f,
        WHITE);
    EndDrawing();
}

void RaylibFrontend::changePixel(int x, int y, Color color){
    framebuffer[x + (y * windowWidth)] = color;
}