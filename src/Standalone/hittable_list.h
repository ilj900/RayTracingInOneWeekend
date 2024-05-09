#pragma once

#include "aabb.h"
#include "hittable.h"
#include "pdf.h"

#include <memory>
#include <vector>

class FHittableList : public FHittable
{
public:
    std::vector<std::shared_ptr<FHittable>> Hittables;

    FHittableList() = default;
    FHittableList(std::shared_ptr<FHittable> Hittable);

    void Clear();
    void Add(std::shared_ptr<FHittable> Hittable);

    bool Hit(const FRay& Ray, FInterval Interval, FHitRecord& HitRecordOut) const override;

    FAABB BoundingBox() const override;

    double PDFValue(const FPoint3& Origin, const FVector3& Direction) const override;

    FVector3 Random(const FPoint3& Origin) const override;

private:
    FAABB BBox;
};

class FHittablePDF : public FPDF
{
public:
    FHittablePDF(const FHittable& ObjectsIn, const FPoint3& OriginIn);
    double Value(const FVector3 Direction) const override;
    FVector3 Generate() const override;

private:
    const FHittable& Objects;
    FPoint3 Origin;
};
