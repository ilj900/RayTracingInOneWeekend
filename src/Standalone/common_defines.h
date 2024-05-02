#ifndef COMMON_DEFINES_H
#define COMMON_DEFINES_H

#include <limits>

const float Infinity = std::numeric_limits<float>::infinity();
const float M_PI = 3.14159265359;

inline float DegreesToRadians(float Degrees)
{
    return Degrees * M_PI / 180.f;
}

inline float RadiansToDegrees(float Radians)
{
    return Radians * 180.f / M_PI;
}

#endif // COMMON_DEFINES_H