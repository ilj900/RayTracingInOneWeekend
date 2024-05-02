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
}

bool FHittableList::Hit(const FRay& Ray, float TMin, float TMax, FHitRecord& HitRecordOut) const
{
    FHitRecord HitRecord{};
    bool bHitAnything = false;
    auto ClosestSoFar = TMax;

    for (const auto& Hittable : Hittables)
    {
        if (Hittable->Hit(Ray, TMin, TMax, HitRecord))
        {
            bHitAnything = true;
            ClosestSoFar = HitRecord.T;
            HitRecordOut = HitRecord;
        }
    }

    return bHitAnything;
}