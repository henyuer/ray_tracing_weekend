#pragma once

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}
inline double random_double()
{
    // returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}
inline double random_double(double min, double max)
{
    // returns a random real in [min,max).
    return min + (max - min) * random_double();
}

class interval
{
public:
    double min, max;

    interval() : min(infinity), max(-infinity) {};
    interval(double min, double max) : min(min), max(max) {};

    double size() const { return max - min; }
    bool contains(double x) const { return x >= min && x <= max; }
    bool surrounds(double x) const { return x > min && x < max; }
    double clamp(double x) const { return x < min ? min : x > max ? max
                                                                  : x; }

    static const interval empty, universe;
};

const interval interval::empty = interval(infinity, -infinity);
const interval interval::universe = interval(-infinity, infinity);

#include "ray.h"
#include "vec3.h"
#include "color.h"