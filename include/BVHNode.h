#pragma once
#include <memory>
#include <vector>
#include "AABB.h"
#include "Handles.h"
#include <iostream>

namespace rt{
class IHit;
class World;
class HitRecord;

class BVHNode{
public:
    BVHNode(World& world, HittableHandle start, HittableHandle end);

    bool hit(const Ray& ray, Interval rayT, HitRecord& record) const;
    AABB boundingBox() const;

    void printDebug() const {
        std::cout << "Left: " << left << " Right: " << right << " isLeaf: " << isLeaf << " leftNode: " << leftNode << " RightNode: " << rightNode << " Start: " << start << " End: " << end << std::endl; 
    }

private:
    HittableHandle left{0};
    HittableHandle right{0};
    bool isLeaf{false};
    BVHHandle leftNode{0};
    BVHHandle rightNode{0};

    World& parentWorld;
    const size_t start;
    const size_t end;
    AABB bbox;
};
}