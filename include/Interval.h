#pragma once
#include <limits>

constexpr float infinity = std::numeric_limits<float>::infinity();

namespace rt{
struct Interval{
    static const Interval empty;
    static const Interval universe;

    float min {infinity};
    float max {-infinity}; //interval is empty by default

    Interval() = default;
    Interval(float min, float max) : min{min},max{max}{}
    Interval(Interval a,Interval b){
        min = (a.min <= b.min) ? a.min : b.min;
        max = (a.max >= b.max) ? a.max : b.max;
    }

    float size() const{
        return max - min;
    }
    bool contains(float x) const{
        return min <= x && x <= max;
    }
    bool surrounds(float x) const {
        return min < x && x < max;
    }
    float clamp(float x) const {
        if(x < min) return min;
        if(x > max) return max;
        return x;
    }
    Interval expand(float delta) const{
        float padding = delta/2;
        return Interval(min - padding, max + padding);
    }
};

inline const Interval Interval::empty = Interval(infinity, -infinity);
inline const Interval Interval::universe = Interval(-infinity, infinity);
}