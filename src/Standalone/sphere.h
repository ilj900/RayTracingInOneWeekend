#pragma once

#include "hittable.h"

class FSphere : public FHittable
{
public:
    FSphere(const FPoint3& CenterIn, double RadiusIn, std::shared_ptr<FMaterial> MaterialIn);
    FSphere(const FPoint3& Center1In, const FPoint3& Center2In, double RadiusIn, std::shared_ptr<FMaterial> MaterialIn);

    bool Hit(const FRay& Ray, FInterval Interval, FHitRecord& HitRecordOut) const override;
    FAABB BoundingBox() const override;

    double PDFValue(const FPoint3& Origin, const FVector3& Direction) const override;

    FVector3 Random(const FPoint3& Origin) const override;

    static std::tuple<double, double> GetSphereUV(const FPoint3& Point);

    static FVector3 RandomToSphere(double Radius, double Distance2);

private:
    FPoint3 SphereCenter(double Time) const;
    FPoint3 Center1;
    double Radius;
    std::shared_ptr<FMaterial> Material;
    bool bIsMooving;
    FVector3 CenterDirection;
    FAABB BBox;
};
