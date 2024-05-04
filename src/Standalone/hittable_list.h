#pragma once

#include "aabb.h"
#include "hittable.h"

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

private:
    FAABB BBox;
};
