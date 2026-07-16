#pragma once
#include <limits>

constexpr double infinity = std::numeric_limits<double>::infinity();

namespace rt{
struct Interval{
    double min {infinity};
    double max {-infinity}; //interval is empty by default

    Interval() = default;
    Interval(double min, double max) : min{min},max{max}{}
    Interval(Interval a,Interval b){
        min = (a.min <= b.min) ? a.min : b.min;
        max = (a.max >= b.max) ? a.max : b.max;
    }

    double size() const{
        return max - min;
    }
    bool contains(double x) const{
        return min <= x && x <= max;
    }
    bool surrounds(double x) const {
        return min < x && x < max;
    }
    double clamp(double x) const {
        if(x < min) return min;
        if(x > max) return max;
        return x;
    }
    Interval expand(const Interval interval,double delta) const{
        double padding = delta/2;
        return Interval(interval.min - padding, interval.max + padding);
    }
    static const Interval empty;
    static const Interval universe;
};
}