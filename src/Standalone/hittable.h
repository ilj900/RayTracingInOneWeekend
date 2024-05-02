#ifndef HITTABLE_H
#define HITTABLE_H

#include "interval.h"
#include "ray.h"

class FHitRecord
{
public:
    FPoint3 Position;
    FVector3 Normal;
    float T;
    bool bFrontFace;

    void SetFaceNormal(const FRay& Ray, const FVector3& OutwardNormal);
};

class FHittable
{
public:
    virtual ~FHittable() = default;

    virtual bool Hit(const FRay& Ray, FInterval Interval, FHitRecord& HitRecordOut) const = 0;
};

#endif // HITTABLE_H