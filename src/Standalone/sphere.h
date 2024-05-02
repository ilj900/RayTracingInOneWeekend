#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class FSphere : public FHittable
{
public:
    FSphere(const FPoint3& CenterIn, float RadiusIn);

    bool Hit(const FRay& Ray, FInterval Interval, FHitRecord& HitRecordOut) const override;

private:
    FPoint3 Center;
    float Radius;
};

#endif // SPHERE_H