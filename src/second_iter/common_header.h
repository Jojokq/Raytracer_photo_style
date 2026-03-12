#ifndef COMMON_HEADER_H
#define COMMON_HEADER_H

#include <cmath>
#include <random>
#include <iostream>
#include <limits>
#include <memory>
#include <iomanip>
#include <chrono>
//#include <stdlib.h>
#include <string>

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

inline int random_int(int min, int max) {
    return int(random_double(min, max + 1));
}


//Some time measurement magic function
// Source - https://stackoverflow.com/a
// Posted by Nikos Athanasiou, modified by community.
// Retrieved 2026-01-28, License - CC BY-SA 4.0

template <
    class result_t   = std::chrono::seconds,
    class clock_t    = std::chrono::steady_clock,
    class duration_t = std::chrono::seconds
>
auto since(std::chrono::time_point<clock_t, duration_t> const& start)
{
    return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}


//Common headers

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif