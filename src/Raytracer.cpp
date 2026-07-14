#include "Raytracer.h"
#include "Ray.h"
#include "Sphere.h"
#include <limits>
#include <thread>
#include "Camera.h"
#include "PixColor.h"
#include "Handles.h"
#include "LambertianMaterial.h"
#include "MetalMaterial.h"
#include "DielectricMaterial.h"
#include "SolidTexture.h"
#include "CheckeredTexture.h"
#include "ImageTexture.h"
#include <iostream>
#include <algorithm>
#include <mutex>
#include <atomic>

using namespace rt;

static double linearToGamma(double linear);

Raytracer::Raytracer(int renderWidth,int renderHeight,int maxSamples,int maxDepth,int threads)
    : cam(renderWidth,renderHeight), renderWidth{renderWidth}, renderHeight{renderHeight}, maxSamples{maxSamples}, maxDepth{maxDepth}, threads{threads}{

        int tilesX = (renderWidth  + tileSize - 1) / tileSize;   // ceil division
        int tilesY = (renderHeight + tileSize - 1) / tileSize;
        tileCount = tilesX * tilesY;
        tiles = std::make_unique<Tile[]>(tileCount);

        for (int y = 0; y < tilesY; ++y) {
            for (int x = 0; x < tilesX; ++x) {
                size_t i = y * tilesX + x;

                tiles[i].startX = x * tileSize;
                tiles[i].startY = y * tileSize;
                tiles[i].width  = std::min(tileSize, renderWidth  - tiles[i].startX);
                tiles[i].height = std::min(tileSize, renderHeight - tiles[i].startY);
                tiles[i].colors.resize(tiles[i].width * tiles[i].height);
            }
        }

        progressOfTiles.resize(tileCount);
        framebuffer.resize(renderHeight * renderWidth);

        ImageHandle earth = assets.addImage("resources/earthmap.jpg");

        TextureHandle redColor = assets.addTexture<SolidTexture>(glm::dvec3{1,0,0});
        TextureHandle greenColor = assets.addTexture<SolidTexture>(glm::dvec3{0,1,0});
        TextureHandle blueColor = assets.addTexture<SolidTexture>(glm::dvec3{0,0,1});
        TextureHandle grayColor = assets.addTexture<SolidTexture>(glm::dvec3{0.7,0.7,0.7});
        TextureHandle goldenColor = assets.addTexture<SolidTexture>(glm::dvec3{0.8,0.8,0});

        TextureHandle checkered = assets.addTexture<CheckeredTexture>(0.5,redColor,blueColor);
        TextureHandle earthTex = assets.addTexture<ImageTexture>(earth);

        MaterialHandle red = assets.addMaterial<LambertianMaterial>(redColor);
        MaterialHandle green = assets.addMaterial<LambertianMaterial>(greenColor);
        MaterialHandle blue = assets.addMaterial<LambertianMaterial>(blueColor);
        MaterialHandle whiteMetal = assets.addMaterial<MetalMaterial>(grayColor,0.2);
        MaterialHandle yellowMetal = assets.addMaterial<MetalMaterial>(goldenColor,0.1);

        MaterialHandle checkeredMat = assets.addMaterial<LambertianMaterial>(checkered);

        MaterialHandle earthMat = assets.addMaterial<LambertianMaterial>(earthTex);

        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{0,1,0.5},1,red));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{2,1,-1},0.8,blue));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{-2,1,-2},1.2,green));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{1,0.5,-3},0.5,yellowMetal));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{-1,0.5,-3},0.5,whiteMetal));

        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{0,2,-4},1.5,blue));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{3,1,-5},1.0,red));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{-3,1,-5},1.0,green));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{0,-0.5,-2},0.3,yellowMetal));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{2,-0.5,-2},0.3,whiteMetal));

        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{-2,-0.5,-2},0.3,red));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{1.5,1.5,-6},1.0,blue));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{-1.5,1.5,-6},1.0,green));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{0,3,-7},1.8,yellowMetal));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{2,3,-7},1.8,whiteMetal));

        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{-2,3,-7},1.8,red));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{4,1,-3},0.7,blue));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{-4,1,-3},0.7,green));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{0,0,-8},2.0,yellowMetal));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{0,1,-10},1.5,whiteMetal));
        
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{-2,5,1},2,checkeredMat));
        hittableObjects.push_back(std::make_unique<Sphere>(glm::dvec3{0,3,0},0.3,earthMat));

}

void Raytracer::launch(){
    for(int i = 0; i < threads; i++){
        workers.push_back(std::thread(&Raytracer::renderTileWorker,this));
    }
}

const std::vector<Color>& Raytracer::getCurrentFrameBuffer(){
    static const Interval intensity(0.000,0.999);

    for(int i = 0; i < tileCount; i ++){
        Tile& tile = tiles[i];
        Tile snapshot;
        uint64_t thisFrame = curFrame.load(std::memory_order_acquire);
        bool expected = false;
        if(tile.isOccupied.compare_exchange_weak(expected,true)){
            if(tile.samples == 0){ tile.isOccupied.store(false,std::memory_order_relaxed); continue;}

            snapshot.startX = tile.startX;
            snapshot.startY = tile.startY;
            snapshot.width = tile.width;
            snapshot.height = tile.height;
            snapshot.samples = tile.samples;
            snapshot.colors = tile.colors;
            snapshot.frame = tile.frame;
            tile.isOccupied.store(false,std::memory_order_release);
        } else{ continue; }

        if(snapshot.frame == thisFrame){
            progressOfTiles[i].samplesCollected = snapshot.samples;
            progressOfTiles[i].frame = snapshot.frame;
        }else{
            progressOfTiles[i].samplesCollected = 0;
            progressOfTiles[i].frame = thisFrame;
            continue;
        }

        for(int y = snapshot.startY; y < snapshot.height + snapshot.startY; y++){
            for(int x = snapshot.startX; x < snapshot.width + snapshot.startX; x++){
                int localX = x - snapshot.startX;
                int localY = y - snapshot.startY;
                glm::dvec3 pixelColor = snapshot.colors[snapshot.localPosition(localX,localY)];
                pixelColor = pixelColor / static_cast<double>(snapshot.samples);
                pixelColor.x = linearToGamma(pixelColor.x);
                pixelColor.y = linearToGamma(pixelColor.y);
                pixelColor.z = linearToGamma(pixelColor.z);
                pixelColor.x = intensity.clamp(pixelColor.x);
                pixelColor.y = intensity.clamp(pixelColor.y);
                pixelColor.z = intensity.clamp(pixelColor.z);
                putPixelInBuffer(x,y,Color{.r = toByte(pixelColor.x),
                                            .g = toByte(pixelColor.y),
                                            .b = toByte(pixelColor.z),
                                            .a = 255});
            }

        }
    }

    return framebuffer;
}

const std::vector<std::unique_ptr<IHit>>& Raytracer::getHittables() const{
    return hittableObjects;
}
const AssetManager& Raytracer::getAssets() const{
    return assets;
}
int Raytracer::getRenderWidth() const{
    return renderWidth;
}
int Raytracer::getRenderHeight() const{
    return renderHeight;
}
void Raytracer::putPixelInBuffer(int x,int y,Color color){
    framebuffer[x + (y * renderWidth)] = color;
}

Raytracer::~Raytracer(){
    isRunning.store(false,std::memory_order_relaxed);
    for(std::thread& thread : workers){
        thread.join();
    }
}

void Raytracer::renderTileWorker(){
    while(isRunning.load(std::memory_order_relaxed)){
        uint64_t tileToRender = nextTile.fetch_add(1,std::memory_order_relaxed);

        Tile& tile = tiles[tileToRender % tileCount];

        bool expected = false;
        if(tile.isOccupied.compare_exchange_strong(expected,true)){
            if(tile.samples >= maxSamples && curFrame.load(std::memory_order_relaxed) == tile.frame){
            tile.isOccupied.store(false,std::memory_order_relaxed); continue;}
        }else{continue;}

        
        for(int y = tile.startY; y < tile.height + tile.startY; y++){
            for(int x = tile.startX; x < tile.width + tile.startX; x++){
                if(tile.frame < curFrame.load(std::memory_order_relaxed)){goto restartTile;}
                if(!isRunning.load(std::memory_order_relaxed)){tile.isOccupied.store(false,std::memory_order_release); goto killThread;}
                int localX = x - tile.startX;
                int localY = y - tile.startY;
                camMutex.lock_shared();
                Ray rayToFollow = cam.generateRayForPixel(x,y);
                camMutex.unlock_shared();

                tile.acummulatePixel(localX,localY,rayToFollow.rayColor(*this,Interval{0,infinity},maxDepth));
            }
        }
        tile.samples++;

        if(tile.frame < curFrame.load(std::memory_order_relaxed)){
            restartTile:
            tile.samples = 0;
            std::fill(tile.colors.begin(),tile.colors.end(),glm::dvec3(0,0,0));
            tile.frame = curFrame.load(std::memory_order_relaxed);
        }

        tile.isOccupied.store(false,std::memory_order_release);
        
    }
    killThread:;
}

bool Raytracer::isFrameDone() const{
    int sumOfWork = 0;
    int expectedWork = maxSamples * tileCount;
    uint64_t forFrame = curFrame.load(std::memory_order_relaxed);
    for(int i = 0; i < tileCount; i++){
        if(progressOfTiles[i].frame == forFrame){
            sumOfWork += progressOfTiles[i].samplesCollected;
        }
    }
    return (sumOfWork == expectedWork) ? true : false;
}
void Raytracer::invalideRender(){
    curFrame.fetch_add(1,std::memory_order_release);
}

void Raytracer::addPosCamera(glm::dvec3 add){
    camMutex.lock();
    cam.addPosition(add);
    camMutex.unlock();
}
void Raytracer::addRotCamera(double deltaYaw,double deltaPitch,double deltaRoll){
    camMutex.lock();
    cam.addRotation(deltaYaw,deltaPitch,deltaRoll);
    camMutex.unlock();
}
glm::dvec3 Raytracer::getCamForwardVec() const{
    return cam.getForward();
}
glm::dvec3 Raytracer::getCamUpVec() const{
    return cam.getUp();
}
glm::dvec3 Raytracer::getCamRightVec() const{
    return cam.getRight();
}

static double linearToGamma(double linear){
    if(linear > 0){
        return glm::sqrt(linear);
    }
    return 0;
}