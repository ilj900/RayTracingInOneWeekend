#ifndef AABB_H
#define AABB_H

#include "interval.h"
#include "ray.h"
#include "vector3.h"

class FAABB
{
public:
    FAABB() = default;
    FAABB(const FInterval& XIn, const FInterval& YIn, const FInterval& ZIn);
    FAABB(const FPoint3& A, const FPoint3& B);
    FAABB(const FAABB& A, const FAABB& B);

    const FInterval& AxisInterval(uint32_t Axis) const;
    bool Hit(const FRay& Ray, FInterval RayInterval) const;

    FInterval X;
    FInterval Y;
    FInterval Z;
};

#endif // AABB_H