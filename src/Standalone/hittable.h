#pragma once

#include "aabb.h"
#include "interval.h"
#include "ray.h"

#include <memory>

class FMaterial;

class FHitRecord
{
public:
    FPoint3 Position;
    FVector3 Normal;
    std::shared_ptr<FMaterial> Material;
    double T;
    bool bFrontFace;

    void SetFaceNormal(const FRay& Ray, const FVector3& OutwardNormal);
};

class FHittable
{
public:
    virtual ~FHittable() = default;

    virtual bool Hit(const FRay& Ray, FInterval Interval, FHitRecord& HitRecordOut) const = 0;

    virtual FAABB BoundingBox() const = 0;
};
