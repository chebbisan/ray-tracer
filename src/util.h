#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <fstream>

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

void write_to_file(const std::string& filename, const std::vector<std::string>& data,
int image_width, int image_height) {
    std::ofstream outfile(filename);

    if (!outfile) {
        std::cerr << "File couldn't open\n";
        return;
    }

    outfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";


    for (const auto& line : data) {
        outfile << line << std::endl;
    }

    outfile.close();
}


// headers

#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"


#endif