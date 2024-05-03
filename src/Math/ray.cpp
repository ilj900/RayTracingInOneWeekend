#include "ray.h"

FRay::FRay() {};

FRay::FRay(const FVector3& OriginIn, const FVector3& DirectionIn, double TimeIn) : Origin(OriginIn), Direction(DirectionIn), Time(TimeIn) {};

const FPoint3& FRay::GetOrigin() const
{
    return Origin;
}
const FVector3& FRay::GetDirection() const
{
    return Direction;
}

const double FRay::GetTime() const
{
    return Time;
}

FPoint3 FRay::At(double T) const
{
    return (Origin + Direction * T);
}