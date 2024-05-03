#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class FSphere : public FHittable
{
public:
    FSphere(const FPoint3& CenterIn, double RadiusIn, std::shared_ptr<FMaterial> MaterialIn);
    FSphere(const FPoint3& Center1In, const FPoint3& Center2In, double RadiusIn, std::shared_ptr<FMaterial> MaterialIn);

    bool Hit(const FRay& Ray, FInterval Interval, FHitRecord& HitRecordOut) const override;

private:
    FPoint3 SphereCenter(double Time) const;
    FPoint3 Center1;
    double Radius;
    std::shared_ptr<FMaterial> Material;
    bool bIsMooving;
    FVector3 CenterDirection;
};

#endif // SPHERE_H