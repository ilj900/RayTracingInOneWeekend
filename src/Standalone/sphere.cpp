#include "sphere.h"

FSphere::FSphere(const FPoint3& CenterIn, double RadiusIn, std::shared_ptr<FMaterial> MaterialIn) : Center(CenterIn), Radius(RadiusIn), Material(MaterialIn) {};

bool FSphere::Hit(const FRay &Ray, FInterval Interval, FHitRecord& HitRecordOut) const
{
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
    FVector3 OutwardNormal = (HitRecordOut.Position - Center) / Radius;
    HitRecordOut.SetFaceNormal(Ray, OutwardNormal);
    HitRecordOut.Material = Material;

    return true;
}