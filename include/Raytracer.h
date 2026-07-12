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
#include <shared_mutex>

namespace rt{
struct Tile{
    int startX;
    int startY;
    int width;
    int height;
    int samples{0};
    int frame{0};
    std::vector<glm::dvec3> colors;
    std::atomic<bool> isOccupied{false};

    int localPosition(int x, int y) const{
        return x + (y * width);
    }
    void acummulatePixel(int x, int y, glm::dvec3 color){
        colors[localPosition(x,y)] += color;
    }
};

struct TileProgressRecord{
    int samplesCollected{0};
    int frame{0};
};

class Raytracer{
public:
    Raytracer(int renderWidth=400, int renderHeight=800, int maxSamples = 10, int maxDepth = 50, int threads = 4);
    ~Raytracer();

    void launch();
    bool isFrameDone() const;
    void invalideRender();

    const std::vector<Color>& getCurrentFrameBuffer();
    const std::vector<std::unique_ptr<IHit>>& getHittables() const;

    void addPosCamera(glm::dvec3 add);
    void addRotCamera(double deltaYaw,double deltaPitch,double deltaRoll);
    glm::dvec3 getCamForwardVec() const;
    glm::dvec3 getCamUpVec() const;
    glm::dvec3 getCamRightVec() const;

    const AssetManager& getAssets() const;

    int getRenderWidth() const;
    int getRenderHeight() const;
private:
    std::vector<std::unique_ptr<IHit>> hittableObjects;

    std::unique_ptr<Tile[]> tiles; //tiles are non movable so it cant be a vector
    std::vector<TileProgressRecord> progressOfTiles;
    std::vector<Color> framebuffer;

    RaytracerCamera cam;
    std::shared_mutex camMutex;

    AssetManager assets;
    const int renderWidth;
    const int renderHeight;
    const int tileSize{64};
    const int maxSamples;
    const int maxDepth;
    const int threads;
    int tileCount;

    std::atomic<bool> isRunning{true};
    std::vector<std::thread> workers;
    std::atomic<uint64_t> nextTile{0};
    std::atomic<uint64_t> curFrame{0};

    void putPixelInBuffer(int x,int y,Color color);
    void renderTileWorker();
};
}