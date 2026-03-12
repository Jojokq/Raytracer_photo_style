#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H

#include <cmath>
#include <random>
#include <iostream>
#include <limits>
#include <memory>
#include <iomanip>

//C++ Std usings

using std::shared_ptr;
using std::make_shared;

//Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Utility functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

//Common headers

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif