#include "ray.h"

FRay::FRay() {};

FRay::FRay(const FVector3& OriginIn, const FVector3& DirectionIn) : Origin(OriginIn), Direction(DirectionIn) {};

const FPoint3& FRay::GetOrigin() const
{
    return Origin;
}
const FVector3& FRay::GetDirection() const
{
    return Direction;
}

FPoint3 FRay::At(float T) const
{
    return (Origin + Direction * T);
}