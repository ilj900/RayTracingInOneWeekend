#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class FSphere : public FHittable
{
public:
    FSphere(const FPoint3& CenterIn, float RadiusIn, std::shared_ptr<FMaterial> MaterialIn);

    bool Hit(const FRay& Ray, FInterval Interval, FHitRecord& HitRecordOut) const override;

private:
    FPoint3 Center;
    float Radius;
    std::shared_ptr<FMaterial> Material;
};

#endif // SPHERE_H