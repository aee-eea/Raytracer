#pragma once
#include <limits>

constexpr double infinity = std::numeric_limits<double>::infinity();

struct Interval{
    double min {infinity};
    double max {-infinity}; //interval is empty by default

    Interval(double min, double max) : min{min},max{max}{}

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
    static const Interval empty;
    static const Interval universe;
};