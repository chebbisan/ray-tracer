#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

using std::shared_ptr;
using std::make_shared;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// util func

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180;
}



inline double random_double(double min = 0., double max = 1.) {
    static std::uniform_real_distribution<double> dist(min, max);
    static std::mt19937 gen;
    return dist(gen);
}


// headers

#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"


#endif