#include "interval.h"

#include "hittable_list.h"

FHittableList::FHittableList(std::shared_ptr<FHittable> Hittable)
{
    Add(Hittable);
}

void FHittableList::Clear()
{
    Hittables.clear();
}

void FHittableList::Add(std::shared_ptr<FHittable> Hittable)
{
    Hittables.push_back(Hittable);
    BBox = FAABB(BBox, Hittable->BoundingBox());
}

bool FHittableList::Hit(const FRay& Ray, FInterval Interval, FHitRecord& HitRecordOut) const
{
    FHitRecord HitRecord{};
    bool bHitAnything = false;
    auto ClosestSoFar = Interval.Max;

    for (const auto& Hittable : Hittables)
    {
        if (Hittable->Hit(Ray, {Interval.Min, ClosestSoFar}, HitRecord))
        {
            bHitAnything = true;
            ClosestSoFar = HitRecord.T;
            HitRecordOut = HitRecord;
        }
    }

    return bHitAnything;
}

FAABB FHittableList::BoundingBox() const
{
    return BBox;
}

FHittablePDF::FHittablePDF(const FHittable& ObjectsIn, const FPoint3& OriginIn) : Objects(ObjectsIn), Origin(OriginIn) {};

double FHittablePDF::Value(const FVector3 Direction) const
{
    return Objects.PDFValue(Origin, Direction);
}

FVector3 FHittablePDF::Generate() const
{
    return Objects.Random(Origin);
}
