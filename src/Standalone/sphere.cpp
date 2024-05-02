#include "sphere.h"

FSphere::FSphere(const FPoint3& CenterIn, float RadiusIn, std::shared_ptr<FMaterial> MaterialIn) : Center(CenterIn), Radius(RadiusIn), Material(MaterialIn) {};

bool FSphere::Hit(const FRay &Ray, FInterval Interval, FHitRecord& HitRecordOut) const
{
    FVector3 RayOriginToSphereCenterVector = Center - Ray.GetOrigin();

    float A = Ray.GetDirection().Length2();
    float H = Dot(Ray.GetDirection(), RayOriginToSphereCenterVector);
    float C = RayOriginToSphereCenterVector.Length2() - (Radius * Radius);
    float D = H * H - A * C;

    if (D < 0)
    {
        return false;
    }

    float SqrtD = sqrt(D);

    float Root = (H -SqrtD) / A;

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