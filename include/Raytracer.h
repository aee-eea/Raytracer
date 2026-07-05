#pragma once
#include <glm/glm.hpp>
#include "IHit.h"
#include "Camera.h"
#include "AssetManager.h"
#include "PixColor.h"
#include <atomic>
#include <cstdint>
#include <memory>
#include <thread>
#include <vector>
#include <queue>

struct Tile{
    int startX;
    int startY;
    int width;
    int height;
    int Samples{0};
    int Frame{0};
    std::vector<glm::dvec3> colors;
    std::unique_ptr<std::atomic<bool>> isOccupied;

    int localPosition(int x, int y) const{
        return x + (y * width);
    }
    void acummulatePixel(int x, int y, glm::dvec3 color){
        colors[localPosition(x,y)] += color;
    }
};

class Raytracer{
public:
    Raytracer(int renderWidth=400,int renderHeight=800,int maxSamples = 10);
    ~Raytracer();
    void launch();
    const std::vector<PixColor>& getCurrentFrameBuffer();
    const std::vector<std::unique_ptr<IHit>>& getHittables() const;
    const AssetManager& getAssets() const;
    bool isFrameDone() const;
    int getRenderWidth() const;
    int getRenderHeight() const;
    int maxDepth;
private:
    std::vector<std::unique_ptr<IHit>> hittableObjects;

    std::vector<Tile> tiles;
    std::vector<int> progressOfTiles;
    std::vector<PixColor> framebuffer;

    RaytracerCamera cam;
    AssetManager assets;
    const int renderWidth;
    const int renderHeight;
    const int tileSize{128};
    int maxSamples;

    std::thread rendererThread;
    std::atomic<bool> isRunning{true};
    std::vector<std::thread> workers;
    std::atomic<uint64_t> nextTile{0};
    std::atomic<uint64_t> curFrame{0};

    void putPixelInBuffer(std::vector<PixColor>& buffer,int x,int y,PixColor color);
    void renderTileWorker();
};