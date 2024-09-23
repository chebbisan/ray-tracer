#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// util func

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180;
}

// headers

#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"


#endif