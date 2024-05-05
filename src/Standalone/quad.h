#pragma once

#include "hittable.h"
#include "hittable_list.h"

class FQuad : public FHittable
{
public:
    FQuad(const FPoint3& QIn, const FVector3 UIn, const FVector3 VIn, std::shared_ptr<FMaterial> MaterialIn);

    virtual void SetBoundingBox();

    FAABB BoundingBox() const override;

    bool Hit(const FRay& Ray, FInterval Interval, FHitRecord& HitRecordOut) const override;

    bool IsInterior(double A, double B, FHitRecord& HitRecord) const;

private:
    FPoint3 Q;
    FVector3 U;
    FVector3 V;
    std::shared_ptr<FMaterial> Material;
    FAABB BBox;
    FVector3 Normal;
    double D;
    FVector3 W;
};

std::shared_ptr<FHittableList> Box(const FPoint3& A, const FPoint3& B, std::shared_ptr<FMaterial> Material);
