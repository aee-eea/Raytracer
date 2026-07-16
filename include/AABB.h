#pragma once
#include "Interval.h"
#include "Ray.h"
#include <glm/glm.hpp>

namespace rt{

class AABB{
public:
    Interval axis[3];

    AABB(){}
    AABB(Interval x, Interval y, Interval z){ axis[0]=x; axis[1]=y; axis[2]=z;}
    AABB(glm::dvec3 a, glm::dvec3 b){
        axis[0] = (a.x <= b.x) ? Interval(a.x,b.x) : Interval(b.x,a.x);
        axis[1] = (a.y <= b.y) ? Interval(a.y,b.y) : Interval(b.y,a.y);
        axis[2] = (a.z <= b.z) ? Interval(a.z,b.z) : Interval(b.z,a.z);
    }
    AABB(const AABB& a, const AABB& b){
        axis[0] = Interval(a.axis[0],b.axis[0]);
        axis[1] = Interval(a.axis[1],b.axis[1]);
        axis[2] = Interval(a.axis[2],b.axis[2]);
    }
    bool hit(Ray ray, Interval rayT) const{
        const glm::dvec3 rayOrigin = ray.orig;
        const glm::dvec3 rayDir = ray.dir;

        for(int selectedAxis = 0; selectedAxis < 3; selectedAxis++){
            Interval ax = axis[selectedAxis];
            double adinv = 1.0 / rayDir[selectedAxis];
            double rayAxis = rayOrigin[selectedAxis];

            double t0 = (ax.min - rayAxis) * adinv;
            double t1 = (ax.max - rayAxis) * adinv;

            if(t0 < t1) {
                if(t0 > rayT.min){rayT.min = t0;}
                if(t1 < rayT.max){rayT.max = t1;}
            }else{
                if(t1 > rayT.min){rayT.min = t1;}
                if(t0 < rayT.max){rayT.max = t0;}
            }

            if(rayT.max <= rayT.min){
                return false;
            }
        }
        return true;
    }
};

}