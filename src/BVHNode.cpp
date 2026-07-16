#include "BVHNode.h"
#include "World.h"
#include "IHit.h"
#include "VectorHelpers.h"
#include <algorithm>
#include <iostream>

using namespace rt;

static bool boxCompare(const std::unique_ptr<IHit>& a, const std::unique_ptr<IHit>& b, int axisIndex);
static bool boxXCompare(const std::unique_ptr<IHit>& a, const std::unique_ptr<IHit>& b);
static bool boxYCompare(const std::unique_ptr<IHit>& a, const std::unique_ptr<IHit>& b);
static bool boxZCompare(const std::unique_ptr<IHit>& a, const std::unique_ptr<IHit>& b);

BVHNode::BVHNode(World& world, HittableHandle start, HittableHandle end): parentWorld{world}, start{start}, end{end}{
    int axis = randomInt(0,2);

    auto comparator = (axis == 0) ? boxXCompare : (axis == 1) ? boxYCompare : boxZCompare;

    size_t span = end - start;
    if(span == 1){
        left = right = start;
        bbox = AABB(world.getObject(left).boundingBox(),world.getObject(right).boundingBox());
        isLeaf = true;
    } else if(span == 2){
        left = start;
        right = start+1;
        bbox = AABB(world.getObject(left).boundingBox(),world.getObject(right).boundingBox());
        isLeaf = true;
    }else{
        std::sort(std::begin(world.getObjects()) + start, std::begin(world.getObjects()) + end, comparator);
        size_t mid = start + span/2;

        leftNode = world.addBVHNode(start,mid);
        rightNode = world.addBVHNode(mid,end);
        bbox = AABB(world.getBVHNode(leftNode).boundingBox(),world.getBVHNode(rightNode).boundingBox());
    }
}

bool BVHNode::hit(const Ray& ray, Interval rayT, HitRecord& record) const{
    if(!bbox.hit(ray,rayT)){
        return false;
    }

    if(isLeaf){
        bool hitLeft = parentWorld.getObject(left).hit(ray,rayT,record);
        bool hitRight = parentWorld.getObject(right).hit(ray,Interval(rayT.min, hitLeft? record.t : rayT.max),record);
        return hitLeft || hitRight;
    }else{
        bool hitLeft = parentWorld.getBVHNode(leftNode).hit(ray,rayT,record);
        bool hitRight = parentWorld.getBVHNode(rightNode).hit(ray,Interval(rayT.min, hitLeft? record.t : rayT.max),record);
        return hitLeft || hitRight;
    }

    
}
AABB BVHNode::boundingBox() const{
    return bbox;
}

static bool boxCompare(const std::unique_ptr<IHit>& a, const std::unique_ptr<IHit>& b, int axisIndex){
    Interval aInterval = a->boundingBox().axis[axisIndex];
    Interval bInterval = b->boundingBox().axis[axisIndex];
    return aInterval.min < bInterval.min;
}

static bool boxXCompare(const std::unique_ptr<IHit>& a, const std::unique_ptr<IHit>& b){
    return boxCompare(a,b,0);
}
static bool boxYCompare(const std::unique_ptr<IHit>& a, const std::unique_ptr<IHit>& b){
    return boxCompare(a,b,1);
}
static bool boxZCompare(const std::unique_ptr<IHit>& a, const std::unique_ptr<IHit>& b){
    return boxCompare(a,b,2);
}