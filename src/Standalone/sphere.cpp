#include "common_defines.h"

#include "sphere.h"

FSphere::FSphere(const FPoint3& CenterIn, double RadiusIn, std::shared_ptr<FMaterial> MaterialIn) : Center1(CenterIn), Radius(RadiusIn), Material(MaterialIn), bIsMooving(false)
{
    auto RadiusVector = FVector3(Radius, Radius, Radius);
    BBox = FAABB(Center1 - RadiusVector, Center1 + RadiusVector);
};

FSphere::FSphere(const FPoint3& Center1In, const FPoint3& Center2In, double RadiusIn, std::shared_ptr<FMaterial> MaterialIn) : Center1(Center1In), Radius(RadiusIn), Material(MaterialIn), bIsMooving(true)
{
    auto RadiusVector = FVector3(Radius, Radius, Radius);
    auto BBox1 = FAABB(Center1 - RadiusVector, Center1 + RadiusVector);
    auto BBox2 = FAABB(Center2In - RadiusVector, Center2In + RadiusVector);
    BBox = FAABB(BBox1, BBox2);

    CenterDirection = Center2In - Center1In;
};

bool FSphere::Hit(const FRay &Ray, FInterval Interval, FHitRecord& HitRecordOut) const
{
    FPoint3 Center = bIsMooving ? SphereCenter(Ray.GetTime()) : Center1;
    FVector3 RayOriginToSphereCenterVector = Center - Ray.GetOrigin();

    double A = Ray.GetDirection().Length2();
    double H = Dot(Ray.GetDirection(), RayOriginToSphereCenterVector);
    double C = RayOriginToSphereCenterVector.Length2() - (Radius * Radius);
    double D = H * H - A * C;

    if (D < 0)
    {
        return false;
    }

    double SqrtD = sqrt(D);

    double Root = (H -SqrtD) / A;

    if (!Interval.Surrounds(Root))
    {
        Root = (H + SqrtD) / A;

        if (!Interval.Surrounds(Root))
        {
            return false;
        }
    }

    HitRecordOut.T = Root;
    HitRecordOut.Position = Ray.At(Root);
    FVector3 OutwardNormal = (HitRecordOut.Position - Center1) / Radius;
    HitRecordOut.SetFaceNormal(Ray, OutwardNormal);
    auto [U, V] = GetSphereUV(OutwardNormal);
    HitRecordOut.U = U;
    HitRecordOut.V = V;
    HitRecordOut.Material = Material;

    return true;
}

FAABB FSphere::BoundingBox() const
{
    return BBox;
}

std::tuple<double, double> FSphere::GetSphereUV(const FPoint3& Point)
{
    auto Theta = acos(-Point.Y);
    auto Phi = atan2(-Point.Z, Point.X) + M_PI;

    return std::make_tuple(Phi * M_PI_INV * 0.5, Theta * M_PI_INV);
}

FPoint3 FSphere::SphereCenter(double Time) const
{
    return Center1 + Time * CenterDirection;
}
