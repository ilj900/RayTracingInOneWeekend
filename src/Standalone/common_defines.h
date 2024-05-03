#ifndef COMMON_DEFINES_H
#define COMMON_DEFINES_H

#include <limits>

const double Infinity = std::numeric_limits<double>::infinity();
const double M_PI = 3.14159265359;

inline double DegreesToRadians(double Degrees)
{
    return Degrees * M_PI / 180.0;
}

inline double RadiansToDegrees(double Radians)
{
    return Radians * 180.0 / M_PI;
}

#endif // COMMON_DEFINES_H