#include <iostream>
#include "raylib.h"
#include "RaylibFrontend.h"
#include "Raytracer.h"
#include <chrono>

RaylibFrontend::RaylibFrontend(Raytracer& raytracer,int windowWidth, int windowHeight, const std::string& windowTitle)
 : windowWidth{windowWidth},windowHeight{windowHeight},windowTitle{windowTitle},raytracer{raytracer}
{
    InitWindow(windowWidth,windowHeight,windowTitle.c_str());
    SetTraceLogLevel(TraceLogLevel::LOG_NONE);
    SetTargetFPS(120);

}
void RaylibFrontend::start(){
    if(!IsWindowReady()){
        std::cout << "Error during initialization of window" << std::endl;
    }

    raytracer.launch();
    textureOnGpu = LoadTextureFromImage(GenImageColor(raytracer.getRenderWidth(),raytracer.getRenderHeight(), ::BLUE));
    now = std::chrono::steady_clock::now();
    while(!WindowShouldClose()){
        input();
        update();
        render();
    }
}
void RaylibFrontend::input(){
    bool needToInvalide = false;
    glm::vec3 deltaPos(0,0,0);
    glm::vec3 deltaRot(0,0,0); //yaw pitch roll
    float dt = GetFrameTime();
        if (IsKeyDown(KEY_D)) {deltaPos += dt * cameraMovSpeed * raytracer.getCamRightVec(); needToInvalide = true;}
        if (IsKeyDown(KEY_A)) {deltaPos -= dt * cameraMovSpeed * raytracer.getCamRightVec(); needToInvalide = true;}
        if (IsKeyDown(KEY_W)) {deltaPos += dt * cameraMovSpeed * raytracer.getCamForwardVec(); needToInvalide = true;}
        if (IsKeyDown(KEY_S)) {deltaPos -= dt * cameraMovSpeed * raytracer.getCamForwardVec(); needToInvalide = true;}
        if (IsKeyDown(KEY_LEFT_SHIFT)) {deltaPos -= dt * cameraMovSpeed * raytracer.getCamUpVec(); needToInvalide = true;}
        if (IsKeyDown(KEY_SPACE)) {deltaPos += dt * cameraMovSpeed * raytracer.getCamUpVec(); needToInvalide = true;}
        if (IsKeyDown(KEY_RIGHT)) {deltaRot -= dt * cameraRotSpeed * glm::vec3(1.0,0,0); needToInvalide = true;}
        if (IsKeyDown(KEY_LEFT)) {deltaRot += dt * cameraRotSpeed * glm::vec3(1.0,0,0); needToInvalide = true;}
        if (IsKeyDown(KEY_UP)) {deltaRot += dt * cameraRotSpeed * glm::vec3(0,1.0,0); needToInvalide = true;}
        if (IsKeyDown(KEY_DOWN)) {deltaRot -= dt * cameraRotSpeed * glm::vec3(0,1.0,0); needToInvalide = true;}
        if(needToInvalide){
            raytracer.addRotCamera(deltaRot.x,deltaRot.y,deltaRot.z);
            raytracer.addPosCamera(deltaPos);
            raytracer.invalideRender();
            now = std::chrono::steady_clock::now();
            notifyAboutFrameCompletion = true;
        }
}
void RaylibFrontend::update(){
    if(IsWindowResized()){
        windowHeight = GetScreenHeight();
        windowWidth = GetScreenWidth();
    }
}
void RaylibFrontend::render(){

    UpdateTexture(textureOnGpu,raytracer.getCurrentFrameBuffer().data());
    BeginDrawing();
    ClearBackground(::RED);


    DrawTexturePro(
        textureOnGpu,
        Rectangle{0,0,static_cast<float>(raytracer.getRenderWidth()),static_cast<float>(raytracer.getRenderHeight())},
        Rectangle{0,0,static_cast<float>(windowWidth),static_cast<float>(windowHeight)},
        Vector2{0,0},
        0.0f,
        ::WHITE);
    EndDrawing();

    if(raytracer.isFrameDone() && notifyAboutFrameCompletion){
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
        std::cout << "Frame took: " << duration << std::endl;
        uint64_t rays = raytracer.countRays();
        std::cout << "MRays per sec: " << static_cast<double>(rays / std::chrono::duration_cast<std::chrono::duration<double>>(end - now).count() / 1'000'000.0) << std::endl;
        now = std::chrono::steady_clock::now();
        notifyAboutFrameCompletion = false;
    }
}

RaylibFrontend::~RaylibFrontend(){
    UnloadTexture(textureOnGpu);
}