#include "onb.h"

FVector3 FONB::Local(double A, double B, double C) const
{
    return A * U + B * V + C * W;
}

FVector3 FONB::Local(const FVector3& A) const
{
    return A.X * U + A.Y * V + A.Z * W;
}

void FONB::BuildFrom(const FVector3& In)
{
    FVector3 UnitW = In.GetNormalized();
    FVector3 A = (fabs(UnitW.X) > 0.9) ? FVector3(0, 1, 0) : FVector3(1, 0, 0);
    V = Cross(UnitW, A);
    U = Cross(UnitW, V);
    W = UnitW;
}

FVector3 FONB::operator[](int i) const
{
    if (i == 1)
    {
        return V;
    }

    if (i == 2)
    {
        return W;
    }

    return U;
}

FVector3& FONB::operator[](int i)
{
    if (i == 1)
    {
        return V;
    }

    if (i == 2)
    {
        return W;
    }

    return U;
}