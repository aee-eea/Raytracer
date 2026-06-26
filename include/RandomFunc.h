#pragma once
#include <cstdlib>

inline double randomDouble() {
    return std::rand() / (RAND_MAX + 1.0);
}
inline double randomDouble(double min, double max){
    return min + (max-min) * randomDouble();
}