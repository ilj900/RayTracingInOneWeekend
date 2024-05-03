#ifndef BVH_H
#define BVH_H

#include "hittable_list.h"

class FBVHNode : public FHittable
{
public:
    FBVHNode(FHittableList HittableList);
    FBVHNode(std::vector<std::shared_ptr<FHittable>>& Hittables, size_t Start, size_t End);

    bool Hit(const FRay& Ray, FInterval RayInterval, FHitRecord& HitRecord) const override;

    FAABB BoundingBox() const override;

    static bool BoxCompare(const std::shared_ptr<FHittable> A, const std::shared_ptr<FHittable> B, int AxisIndex);
    static bool BoxXCompare(const std::shared_ptr<FHittable> A, const std::shared_ptr<FHittable> B);
    static bool BoxYCompare(const std::shared_ptr<FHittable> A, const std::shared_ptr<FHittable> B);
    static bool BoxZCompare(const std::shared_ptr<FHittable> A, const std::shared_ptr<FHittable> B);

private:
    std::shared_ptr<FHittable> Left;
    std::shared_ptr<FHittable> Right;
    FAABB BBox;
};

#endif // BVH_H