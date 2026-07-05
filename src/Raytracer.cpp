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
#include <iostream>
#include <algorithm>
#include <mutex>
#include <atomic>

static double linearToGamma(double linear);

Raytracer::Raytracer(int renderWidth,int renderHeight,int maxSamples,int maxDepth,int threads)
    : cam(renderWidth,renderHeight), renderWidth{renderWidth}, renderHeight{renderHeight}, maxSamples{maxSamples}, maxDepth{maxDepth}, threads{threads}{
        for (int y = 0; y < renderHeight; y += tileSize) {
            for (int x = 0; x < renderWidth; x += tileSize) {

                Tile tile{
                    .startX = x,
                    .startY = y,
                    .width  = std::min(tileSize, renderWidth - x),
                    .height = std::min(tileSize, renderHeight - y),
                };

                tile.colors.resize(tile.width * tile.height, glm::dvec3{0,0,0});

                tile.isOccupied = std::make_unique<std::atomic<bool>>();
                tile.isOccupied->store(false,std::memory_order_relaxed);

                tiles.push_back(std::move(tile));
            }
        }
        tileCount = tiles.size();

        progressOfTiles.resize(tileCount);
        framebuffer.resize(renderHeight * renderWidth);

        MaterialHandle ground = assets.addMaterial<LambertianMaterial>(glm::dvec3{0.2,0.2,0.2});
        MaterialHandle red = assets.addMaterial<LambertianMaterial>(glm::dvec3{1,0,0});
        MaterialHandle green = assets.addMaterial<LambertianMaterial>(glm::dvec3{0,1,0});
        MaterialHandle blue = assets.addMaterial<LambertianMaterial>(glm::dvec3{0,0,1});
        MaterialHandle purple = assets.addMaterial<LambertianMaterial>(glm::dvec3{0.5,0,0.5});
        MaterialHandle left = assets.addMaterial<DielectricMaterial>(1.50);
        MaterialHandle bubble = assets.addMaterial<DielectricMaterial>(1.00/1.50);
        MaterialHandle whiteMetal = assets.addMaterial<MetalMaterial>(glm::dvec3{0.8,0.8,0.8},0.2);
        MaterialHandle yellowMetal = assets.addMaterial<MetalMaterial>(glm::dvec3{0.8,0.8,0},0.1);

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

}

void Raytracer::launch(){
    for(int i = 0; i < threads; i++){
        workers.push_back(std::thread(&Raytracer::renderTileWorker,this));
    }
}

const std::vector<PixColor>& Raytracer::getCurrentFrameBuffer(){
    static const Interval intensity(0.000,0.999);

    for(int i = 0; i < tileCount; i ++){
        Tile& tile = tiles[i];
        Tile snapshot;
        bool expected = false;
        if(tile.isOccupied->compare_exchange_weak(expected,true)){
            if(tile.Samples == 0){ tile.isOccupied->store(false); continue;}
            snapshot.startX = tile.startX;
            snapshot.startY = tile.startY;
            snapshot.width = tile.width;
            snapshot.height = tile.height;
            snapshot.Samples = tile.Samples;
            snapshot.colors = tile.colors;
            tile.isOccupied->store(false);
        } else{ continue; }

        for(int y = snapshot.startY; y < snapshot.height + snapshot.startY; y++){
            for(int x = snapshot.startX; x < snapshot.width + snapshot.startX; x++){
                int localX = x - snapshot.startX;
                int localY = y - snapshot.startY;
                glm::dvec3 pixelColor = snapshot.colors[snapshot.localPosition(localX,localY)];
                pixelColor = pixelColor / static_cast<double>(snapshot.Samples);
                pixelColor.x = linearToGamma(pixelColor.x);
                pixelColor.y = linearToGamma(pixelColor.y);
                pixelColor.z = linearToGamma(pixelColor.z);
                pixelColor.x = intensity.clamp(pixelColor.x);
                pixelColor.y = intensity.clamp(pixelColor.y);
                pixelColor.z = intensity.clamp(pixelColor.z);
                putPixelInBuffer(x,y,PixColor{.r = static_cast<uint8_t>(255 * pixelColor.x),
                                            .g = static_cast<uint8_t>(255 * pixelColor.y),
                                            .b = static_cast<uint8_t>(255 * pixelColor.z),
                                            .a = 255});
            }
        }
        progressOfTiles[i] = snapshot.Samples;
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
void Raytracer::putPixelInBuffer(int x,int y,PixColor color){
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
        if(tile.isOccupied->compare_exchange_weak(expected,true)){
            if(tile.Samples >= maxSamples){ tile.isOccupied->store(false); continue;}
        }else{continue;}

        for(int y = tile.startY; y < tile.height + tile.startY; y++){
            for(int x = tile.startX; x < tile.width + tile.startX; x++){
                if(!isRunning.load(std::memory_order_relaxed)){goto killThread;}
                int localX = x - tile.startX;
                int localY = y - tile.startY;
                tile.acummulatePixel(localX,localY,cam.generateRayForPixel(x,y).rayColor(*this,Interval{0,infinity},maxDepth));
            }
        }
        tile.Samples++;
        tile.isOccupied->store(false);
        
    }
    killThread:;
}

bool Raytracer::isFrameDone() const{
    int sumOfWork = 0;
    int expectedWork = maxSamples * tiles.size();
    for(int i = 0; i < tileCount; i++){
        sumOfWork += progressOfTiles[i];
    }
    return (sumOfWork == expectedWork) ? true : false;
}

static double linearToGamma(double linear){
    if(linear > 0){
        return glm::sqrt(linear);
    }
    return 0;
}