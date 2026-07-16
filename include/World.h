#pragma once
#include <vector>
#include <memory>
#include "IHit.h"
#include "BVHNode.h"
#include "AABB.h"
#include "Handles.h"
#include "unordered_map"

namespace rt{
class World : public IHit{
public:

    void clear();

    template<typename T, typename... Args>
    HittableHandle add(Args&&... args);
    std::vector<std::unique_ptr<IHit>>& getObjects();
    IHit& getObject(HittableHandle handle) const;

    bool hit(const Ray& ray, Interval rayT, HitRecord& record) const override;
    AABB boundingBox() const override;

    BVHHandle addBVHNode(size_t start, size_t end);
    const BVHNode& getBVHNode(BVHHandle handle) const;
    void buildBVH();

private:
    HittableHandle nextHit{0};
    std::vector<std::unique_ptr<IHit>> hittableObjects;
    BVHHandle nextBVH{0};
    std::vector<BVHNode> BVHNodes;
    AABB bbox;
    BVHHandle startingPoint{0};
};

template<typename T, typename... Args>
HittableHandle World::add(Args&&... args){
    static_assert(std::is_base_of_v<IHit, T>, "Object must inherit from IHit to add it to world");
    hittableObjects.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    bbox = AABB(bbox,hittableObjects.back()->boundingBox());
    return nextHit++;
}
}