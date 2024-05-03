#include "bvh_node.h"
#include "rng.h"
#include <algorithm>

FBVHNode::FBVHNode(FHittableList HittableList) : FBVHNode(HittableList.Hittables, 0, HittableList.Hittables.size()) {};

FBVHNode::FBVHNode(std::vector<std::shared_ptr<FHittable>>& Hittables, size_t Start, size_t End)
{
    int Axis = RandomInt(0, 2);

    auto Comparator = (Axis == 0) ? BoxXCompare
                    : (Axis == 1) ? BoxYCompare
                                  : BoxZCompare;

    size_t ObjectSpan = End - Start;

    if (ObjectSpan == 1)
    {
        Left = Right = Hittables[Start];
    }
    else if (ObjectSpan == 2)
    {
        Left = Hittables[Start];
        Right = Hittables[Start + 1];
    }
    else
    {
        std::sort(Hittables.begin() + Start, Hittables.begin() + End, Comparator);
        auto Mid = Start + ObjectSpan / 2;

        Left = std::make_shared<FBVHNode>(Hittables, Start, Mid);
        Right = std::make_shared<FBVHNode>(Hittables, Mid, End);
    }

    BBox = FAABB(Left->BoundingBox(), Right->BoundingBox());
}

bool FBVHNode::Hit(const FRay& Ray, FInterval RayInterval, FHitRecord& HitRecord) const
{
    if (!BBox.Hit(Ray, RayInterval))
    {
        return false;
    }

    bool HitLeft = Left->Hit(Ray, RayInterval, HitRecord);
    bool HitRight = Right->Hit(Ray, RayInterval, HitRecord);

    return HitLeft || HitRight;
}

FAABB FBVHNode::BoundingBox() const
{
    return BBox;
}

bool FBVHNode::BoxCompare(const std::shared_ptr<FHittable> A, const std::shared_ptr<FHittable> B, int AxisIndex)
{
    auto AAxisInterval = A->BoundingBox().AxisInterval(AxisIndex);
    auto BAxisInterval = B->BoundingBox().AxisInterval(AxisIndex);
    return AAxisInterval.Min < BAxisInterval.Min;
}

bool FBVHNode::BoxXCompare(const std::shared_ptr<FHittable> A, const std::shared_ptr<FHittable> B)
{
    return  BoxCompare(A, B, 0);
}

bool FBVHNode::BoxYCompare(const std::shared_ptr<FHittable> A, const std::shared_ptr<FHittable> B)
{
    return  BoxCompare(A, B, 1);
}

bool FBVHNode::BoxZCompare(const std::shared_ptr<FHittable> A, const std::shared_ptr<FHittable> B)
{
    return  BoxCompare(A, B, 2);
}
