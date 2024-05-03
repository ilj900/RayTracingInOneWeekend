#include "aabb.h"

FAABB::FAABB(const FInterval& XIn, const FInterval& YIn, const FInterval& ZIn) : X(XIn), Y(YIn), Z(ZIn) {};

FAABB::FAABB(const FPoint3& A, const FPoint3& B)
{
    X = (A.X <= B.X) ? FInterval(A.X, B.X) : FInterval(B.X, A.X);
    Y = (A.Y <= B.Y) ? FInterval(A.Y, B.Y) : FInterval(B.Y, A.Y);
    Z = (A.Z <= B.Z) ? FInterval(A.Z, B.Z) : FInterval(B.Z, A.Z);
}

FAABB::FAABB(const FAABB& A, const FAABB& B)
{
    X = FInterval(A.X, B.X);
    Y = FInterval(A.Y, B.Y);
    Z = FInterval(A.Z, B.Z);
}

const FInterval& FAABB::AxisInterval(uint32_t Axis) const
{
    if (Axis == 1)
    {
        return Y;
    }
    if (Axis == 2)
    {
        return Z;
    }
    return X;
}

bool FAABB::Hit(const FRay& Ray, FInterval RayInterval) const
{
    const FPoint3& RayOrigin = Ray.GetOrigin();
    const FVector3& RayDirection = Ray.GetDirection();

    for (int Axis = 0; Axis < 3; ++Axis)
    {
        const FInterval& Ax = AxisInterval(Axis);
        const double AdInv = 1. / RayDirection[Axis];

        auto T0 = (Ax.Min - RayOrigin[Axis]) * AdInv;
        auto T1 = (Ax.Max - RayOrigin[Axis]) * AdInv;

        if (T0 < T1)
        {
            if (T0 > RayInterval.Min)
            {
                RayInterval.Min = T0;
            }
            if (T1 < RayInterval.Min)
            {
                RayInterval.Max = T1;
            }
        }
        else
        {
            if (T1 > RayInterval.Min)
            {
                RayInterval.Min = T1;
            }
            if (T0 < RayInterval.Min)
            {
                RayInterval.Max = T0;
            }
        }

        if (RayInterval.Max <= RayInterval.Min)
        {
            return false;
        }
    }

    return true;
}