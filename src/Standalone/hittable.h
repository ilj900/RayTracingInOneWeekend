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
    double U;
    double V;
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

class FTranslate : public FHittable
{
public:
    FTranslate(std::shared_ptr<FHittable> ObjectIn, const FVector3& OffsetIn);
    bool Hit(const FRay& Ray, FInterval RayInterval, FHitRecord& HitRecord) const override;
    FAABB BoundingBox() const override;

private:
    std::shared_ptr<FHittable> Object;
    FVector3 Offset;
    FAABB BBox;
};

class FRotateY : public FHittable
{
public:
    FRotateY(std::shared_ptr<FHittable> ObjectIn, double Angle);
    bool Hit(const FRay& Ray, FInterval RayInterval, FHitRecord& HitRecord) const override;
    FAABB BoundingBox() const override;

private:
    std::shared_ptr<FHittable> Object;
    double SinTheta;
    double CosTheta;
    FAABB BBox;
};
