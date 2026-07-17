#include "World.h"
#include "AABB.h"
#include "IHit.h"
#include <unordered_map>

using namespace rt;

void World::clear(){
    hittableObjects.clear();
}
  
bool World::hit(const Ray& ray, Interval rayT, HitRecord& record) const {
    if(getBVHNode(startingPoint).hit(ray,rayT,record)){
        return true;
    }
    return false;
}

std::vector<std::unique_ptr<IHit>>& World::getObjects(){
    return hittableObjects;
}

IHit& World::getObject(HittableHandle handle) const{
    assert( handle < nextHit && "Invalid hittable handle");
    return *hittableObjects[handle];
}

BVHHandle World::addBVHNode(size_t start, size_t end){
    BVHNodes.emplace_back(BVHNode(*this,start,end));
    return nextBVH++;
}
const BVHNode& World::getBVHNode(BVHHandle handle) const{
    assert( handle < nextBVH && "Invalid bvh handle");
    return BVHNodes[handle];
}

void World::buildBVH(){
    BVHNodes.clear();
    nextBVH = 0;
    startingPoint = addBVHNode(0,hittableObjects.size());
}

AABB World::boundingBox() const{
    return bbox;
}