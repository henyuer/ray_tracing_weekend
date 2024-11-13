#pragma once

#include "vec3.h"
#include <iostream>

using namespace std;
using color = vec3;

inline double linear_to_gamma(double linear)
{
    if (linear < 0.0)
    {
        return 0.0;
    }
    return sqrt(linear);
}

void write_color(ostream &out, const color &pixel_color)
{
    out << static_cast<int>(255.999 * linear_to_gamma(pixel_color.x())) << ' '
        << static_cast<int>(255.999 * linear_to_gamma(pixel_color.y())) << ' '
        << static_cast<int>(255.999 * linear_to_gamma(pixel_color.z())) << '\n';
}